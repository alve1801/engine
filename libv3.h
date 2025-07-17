#ifndef AVES_FRAMEWORK
#define AVES_FRAMEWORK

#include <SDL2/SDL.h>
#include <time.h> // for sleep()
#include <string.h> // for memset/memcpy that we use for keymaps
#undef max
#undef min
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
#undef mod

#define color unsigned short // aka uint16_t

int mod(int a,int b){if(b==0)return 0;a%=b;return(a>=0?a:b+a);}
int mdiv(int a,int b){if(b==0)return 0;return (a+(a<0))/b-(a<0);}

struct img{ // ppm format (ppm pgm pbm)
	int sx=0,sy=0;color*data=0;
	img():sx(0),sy(0),data(0){} ~img(){if(data){free(data);data=0;}}
	img(int x,int y,color*d=0):sx(x),sy(y){if(d)data=d;else
		data=(color*)malloc(x*y*sizeof(color));}
	img(const char*filename){load(filename);}
	// and, because stdlib suck ass
	img(const img&a){cpy(a);}img operator=(const img&a){cpy(a);return*this;}
	void cpy(const img&a){sx=a.sx,sy=a.sy;
		data=(color*)malloc(sx*sy*sizeof(color));
		for(int i=0;i<sx*sy;i++)data[i]=a.data[i];}
	color&operator[](int i){return data[i];}
	color&at(int x,int y){return data[y*sx+x];}
	void clear(color c=0){for(int i=0;i<sx*sy;i++)data[i]=c;}
	void save(const char*filename){
		FILE*f=fopen(filename,"w");
		fprintf(f,"P6\n%i %i\n255\n",sx,sy);
		for(int i=0;i<sx*sy;i++)
			putc(((data[i]&0xf00)>>4)|((data[i]&0xf00)>>8),f),
			putc((data[i]&0xf0)|((data[i]&0xf0)>>4),f),
			putc(((data[i]&0xf)<<4)|(data[i]&0xf),f);
		fclose(f);
	}void load(const char*filename){char a;FILE*f;
		if(data)free(data);data=0,sx=sy=0;
		if(!(f=fopen(filename,"r")))return;
		for(int i=0;i<3;i++)getc(f); // P6\n
		while((a=getc(f))!=' ')sx=sx*10+a-48; // <X> 
		while((a=getc(f))!=10)sy=sy*10+a-48; // <Y>\n
		data=(color*)malloc(sx*sy*sizeof(color));
		while((a=getc(f))!=10); // 255\n
		for(int i=0;i<sx*sy;i++)
			data[i]=(getc(f)>>4)<<8|(getc(f)>>4)<<4|(getc(f)>>4)<<0;
		fclose(f);
	}
};

// yes these should be class variables, but then id have to give their
//  exact length, but then gcc still isnt happy for some reason
// yes theyre also const and static, but then i couldnt do a forward decl.
// fontdata encodes codepage 437, minifontdata is my custom tiny
//  variable-width font, midfontdata is simplifier.neocities.org/4x4
// also add https://gitlab.com/warningnonpotablewater/star-platinum
extern char fontdata[],minifontdata[],midfontdata[];

// lookup tables for the box characters
const char ref_box[]=" \xc4\xb3\xda\xc2\xbf\xc3\xc5\xb4\xc0\xc1\xd9",
	ref_boxbuff[]="";

struct Framework{
	int _sx,_sy,_ss;bool(*userfunc)(Framework*);
	unsigned char font[1<<11],minifont[260],midfont[192];
	const unsigned char*keystate;unsigned char*last_keystate;
	// keystate references/caches from sdl2
	int numkeys; // size of sdl_keystate
	struct{bool lh,rh,lp,rp;int x=0,y=0;}mouse;
	struct audios{
		SDL_AudioSpec want;SDL_AudioDeviceID dev;
		int stamp=0;unsigned char(*ostfunk)(int);}audio;
	static void audiowrapper(void*data,unsigned char*stream,int len){
		audios*a=(audios*)data;
		for(int i=0;i<len;i++)stream[i]=a->ostfunk(a->stamp++);
	}

	SDL_Window*window;SDL_Renderer*renderer;SDL_Event event;
	SDL_Texture*texture;color*newmap;

	Framework(int x,int y,int s=1,unsigned char(*ostfunk)(int)=0)
	:_sx(x),_sy(y),_ss(s>0?s:1),userfunc(0){
		for(int i=0;i<2048;i++)font[i]=(fontdata[2*i]<<4)|(fontdata[2*i+1]-'@');
		for(int i=0;i<260;i++)minifont[i]=
			(minifontdata[2*i]<<4)|(minifontdata[2*i+1]-'@');
		for(int i=0;i<192;i++)midfont[i]=
			(midfontdata[2*i]<<4)|(midfontdata[2*i+1]-'@');

		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
		SDL_CreateWindowAndRenderer(_sx*_ss,_sy*_ss,0,&window,&renderer);
		SDL_RenderSetScale(renderer,s,s);
		SDL_SetRenderDrawColor(renderer,0,0,0,255); // not really necessary
		texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB4444,1,_sx,_sy);
		SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);

		if(ostfunk){
			audio.ostfunk = ostfunk;
			audio.want.freq = 8000;
			audio.want.format = AUDIO_U8;
			audio.want.channels = 1;
			audio.want.samples = 512;
			audio.want.callback = &audiowrapper;
			audio.want.userdata = &(this->audio);
			audio.dev = SDL_OpenAudioDevice(0,0,&audio.want,0,
				SDL_AUDIO_ALLOW_FORMAT_CHANGE); // actually, no change?
			SDL_PauseAudioDevice(audio.dev,0);
		}

		keystate=SDL_GetKeyboardState(&numkeys);
		// GetKeyState also exists, and seems to do keystates instead
		last_keystate=(unsigned char*)malloc(numkeys);
		memset(last_keystate,0,numkeys);
	}

	~Framework(){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		if(audio.ostfunk)SDL_CloseAudioDevice(audio.dev);
		SDL_Quit();
	}

	// is there an sdl routine we could use for this?
	void clear(color c=0){for(int i=0;i<_sx*_sy;i++)newmap[i]=c|0xf000;}

	color transparent=0xf000;

	void p(color c,int x,int y){
		if(x<0||y<0||x>=_sx||y>=_sy||c==transparent)return;
		char alpha=0xf-(c>>12); // XXX blending must happen here
		newmap[y*_sx+x]=(c&0xfff)|(alpha<<12);
	}

	void pc(char c,int cx,int cy,color fgc=0xfff,color bgc=0,bool abs=0){
		if(!abs)cx*=8,cy*=8;
		if(c)for(int x=0;x<8;x++)for(int y=0;y<8;y++)
		p(((char)font[(unsigned char)c*8+y]&(char)(128>>x))?fgc:bgc,cx+x,cy+y);
	}

	int pstest(const char*c,int sx,int sy,int off=0,int moff=8){
		int x=0,y=0,l=0;
		for(int i=0;c[i];i++){l++;
			if(c[i]==9)x=((x>>3)+1)<<3;else
			pc(c[i]==10?0:c[i],sx+x,sy+y,0xfff,0,1);
			x+=8;
			if(sx+x>=_sx||c[i]==10)x=(x+off)%moff,y+=8;
		}return l;
	}

	int ps(const char*c,int sx,int sy,color fgc=0xfff,color bgc=0,bool abs=0){
		int x=0,y=0,l=0;
		for(int i=0;c[i];i++){l++;
			if(c[i]==9)x=((x>>3)+1)<<3;else
			pc(c[i]==10?0:c[i],sx+x,sy+y,fgc,bgc,abs);
			x+=abs?8:1;
			if((sx+x)*(abs?1:8)>=_sx||c[i]==10)x=0,y+=abs?8:1;
		}return l;
	}

	int pcm(char c,int cx,int cy,color fgc=0xfff,color bgc=0){
		if(c<32||c>127)return 0;c-=32;
		int s=minifont[(int)c+1]-minifont[(int)c],i=minifont[(int)c]*5;
		if(s<0)return 0; // temp
		for(int x=0;x<s;x++)for(int y=0;y<5;y++){
			p((minifont[96+i/8]&(1<<(i%8)))?fgc:bgc,cx+x,cy+y);
			i++;
		}return s;
	}

	int psm(const char*c,int cx,int cy,color fgc=0xfff,color bgc=0){
		int x=0,y=0,i=0;
		for(;c[i];i++){
			if(c[i]==10)x=0,y+=6;
			else x+=pcm(c[i],cx+x,cy+y,fgc,bgc)+1;
		}return i; // or width? then, measured how?
	}

	void pcmid(char c,int cx,int cy,color fgc=0xfff,color bgc=0){
		if(c<32||c>127)return;c-=32; // cant predouble c bc signed
		for(int i=0;i<8;i++)
		p(midfont[2*c  ]&(128>>i)?fgc:bgc,cx+(i%4),cy+i/4  ),
		p(midfont[2*c+1]&(128>>i)?fgc:bgc,cx+(i%4),cy+i/4+2);
	}

	int psmid(const char*c,int cx,int cy,color fgc=0xfff,color bgc=0){
		int x=0,y=0,i=0;
		for(;c[i];i++){
			if(c[i]==10)x=0,y+=4;
			else pcmid(c[i],cx+x,cy+y,fgc,bgc),x+=4;
		}return i;
	}

	// XXX might wanna remake this

	// recurses if not 0
	// displays result of recursion %10
	// returns input/10
	int pi(int i,int sx,int sy,color fgc=0xfff,color bgc=0){
		// returns length of number
		// somewhat different approach than what we're used to
		if(!i){pc('0',sx,sy,fgc,bgc);return 1;}
		if(i<0){
			pc('-',sx,sy,fgc,bgc);
			return pi(-1*i,sx+1,sy,fgc,bgc)+1;
		}

		//int len=(int)(log10(i)+1);
		int len=1,j=1;while(i>j)len++,j*=10;

		char s[len+1];
		sprintf(s,"%i",i);
		ps(s,sx,sy,fgc,bgc);
		return len;
	}

	int pis(int i,int sx,int sy,color fgc=0xfff,color bgc=0){
		// haha piss
		static unsigned char mininum[]=
			{0x99,0xf0,0xbd,0xf9,0xe3,0xdb,0xdf,0xf1,0xff,0xfb,0x44};

		if(i<0){ // XXX feels like this should be easier...
			for(int x=0;x<4;x++){
				p((mininum[10]>>x)&0x01?fgc:bgc,sx+0,sy+x);
				p((mininum[10]>>x)&0x10?fgc:bgc,sx+1,sy+x);
				p(bgc,sx+2,sy+x);
			}return pis(-i,sx+3,sy,fgc,bgc)+1;
		}

		//int len=(int)(log10(i)+1);
		int len=1,j=1;while(i>j)len++,j*=10;
		if(len<0)len=1; // for 0
		for(int j=len;j--;i/=10)
			for(int x=0;x<4;x++){
				p((mininum[i%10]>>x)&0x01?fgc:bgc,sx+0+j*3,sy+x);
				p((mininum[i%10]>>x)&0x10?fgc:bgc,sx+1+j*3,sy+x);
				p(bgc,sx+2+j*3,sy+x);
			}
		return len;
	}

	void pi(color*img,int cx,int cy,int sx,int sy){ // blip a whole image
		// dont iterate at all if image is out of bounds
		if(cx>_sx||cy>_sy||cx+sx<0||cy+sy<0)return;
		// XXX optimise to only iterate over visible part of image
		//for(int x=0;x<sx;x++)for(int y=0;y<sy;y++)
		//if(cx+x<_sx && cy+y<_sy)p(img[y*sx+x],cx+x,cy+y);
		int ax=max(0,cx)-cx, ay=max(0,cy)-cy,
			bx=min(cx+sx,_sx)-cx, by=min(cy+sy,_sy)-cy;
		for(int x=ax;x<bx;x++)for(int y=ay;y<by;y++)
		p(img[y*sx+x],cx+x,cy+y);
	}

	// the following are utility and not necessary, remove for the 1173 version

	void line(color c,int ax,int ay,int bx,int by){
		int // adapted from wikipedia/bresenham
			dx =  abs(bx-ax), sx = ax<bx?1:-1,
			dy = -abs(by-ay), sy = ay<by?1:-1,
			error = dx+dy, e2;
		while(1){
			p(c,ax,ay);e2=error<<1;
			if(ax==bx && ay==by)return;
			if(e2>=dy)error+=dy,ax+=sx;
			if(e2<=dx)error+=dx,ay+=sy;
		}
	}

	void pi(img a,int cx,int cy){pi(a.data,cx,cy,a.sx,a.sy);}

	/* XXX not sure if i like defining start+offset instead of start+end...
	da c2 c4 bf
	c3 c5    b4
	b3
	c0 c1    d9 */

	void phl(int sx,int y,int ex,color fgc=0xfff,color bgc=0){
		for(int x=sx;x<sx+ex;x++)pc(0xc4,x,y,fgc,bgc);}

	void pvl(int x,int sy,int ey,color fgc=0xfff,color bgc=0){
		for(int y=sy;y<sy+ey;y++)pc(0xb3,x,y,fgc,bgc);}

	void pbox(int sx,int sy,int ex=1,int ey=1,color fgc=0xfff,color bgc=0){
		pc(0xda,sx -1,sy -1,fgc,bgc),
		pc(0xbf,sx+ex,sy -1,fgc,bgc),
		pc(0xc0,sx -1,sy+ey,fgc,bgc),
		pc(0xd9,sx+ex,sy+ey,fgc,bgc);
		phl(sx,sy-1,ex,fgc,bgc),phl(sx,sy+ey,ex,fgc,bgc);
		pvl(sx-1,sy,ey,fgc,bgc),pvl(sx+ex,sy,ey,fgc,bgc);
	}

	void snap(const char*fname=0){
		FILE*f;
		if(fname)f=fopen(fname,"w");
		else f=fopen("snap.ppm","w");
		fprintf(f,"P6\n%i %i\n255\n",_sx,_sy);
		for(int i=0;i<_sx*_sy;i++)
			putc((newmap[i]&0xf00)>>4,f),
			putc((newmap[i]&0x0f0)   ,f),
			putc((newmap[i]&0x00f)<<4,f);
		fclose(f);
		printf("Screenshot <%s> taken.\n",fname?fname:"snap.ppm");
	}

	// end 1173

	// XXX indexes are sdl_scancodes, which are kinda fucky to work w/
	//  see if we cant hardcode some of the math? how do scancodes even work
	// good news is that scancodes represent physical and not virtual keys,
	//  so it (should be) layout independent :3
	bool key(int key){return keystate[key];}
	bool operator[](int key){return keystate[key];} // ideally, char
	bool keyh(int key){return keystate[key]&&last_keystate[key];}
	bool keyp(int key){return keystate[key]&&(!last_keystate[key]);}
	bool keyr(int key){return (!keystate[key])&&last_keystate[key];}

	void run(bool(*ufunc)(Framework*)=0){
		if(ufunc)userfunc=ufunc;
		for(;;){int ticks=SDL_GetTicks();
			memcpy(last_keystate,keystate,numkeys);
			SDL_PumpEvents();

			int mstate=SDL_GetMouseState(&mouse.x,&mouse.y);
			//mouse.left=mstate&SDL_BUTTON(1),mouse.right=mstate&SDL_BUTTON(3);
			mouse.lp=(!mouse.lh)&&(mstate&SDL_BUTTON(1));
			mouse.rp=(!mouse.rh)&&(mstate&SDL_BUTTON(3));
			mouse.lh=mstate&SDL_BUTTON(1);
			mouse.rh=mstate&SDL_BUTTON(3);

			mouse.x/=_ss,mouse.y/=_ss;

			SDL_LockTexture(texture,0,(void**)&newmap,&mstate);
			clear(); // dont care abt texture pitch
			bool ret=userfunc&&!userfunc(this);
			SDL_UnlockTexture(texture);
			SDL_RenderClear(renderer); // otherwise we muck up the alpha channel
			SDL_RenderCopy(renderer,texture,0,0);
			SDL_RenderPresent(renderer);

			// handle timing
			if(ret)return; // no frame delay if were quitting anyway
			ticks = (1000/24) + ticks - SDL_GetTicks();
			//printf("Frame in %ims\n",42-ticks);
			if(ticks>0)SDL_Delay(ticks);
		}
	}
};

/*void compress(char*data,int dlen=0){ // for new fonts etc
	// entries done row first
	for(int i=0;i<dlen;i++)
	putchar((data[i]>>4)+'@'),
	putchar((data[i]&0xf)+'@');
}*/

char fontdata[]=
	"@@@@@@@@@@@@@@@@GN8A:E8A?O=K?OGNGN?O=K?O?O>G?OGNFL?N?N?NGLCHA@@@"
	"A@CHGL?NGLCHA@@@A@CHCH?N>N>NA@GLA@CHGL?N?NGLA@GL@@@@CLGNGNCL@@@@"
	"?O?O<C8A8A<C?O?O@@CHGLDDDDGLCH@@?O<G8C;K;K8C<G?OCN@NCJGH<L<L<LGH"
	"CLFFFFFFCLAHGNAHCOCCCOC@C@G@?@>@GOFCGOFCFCFG>F<@AH=KCL>G>GCL=KAH"
	"D@G@GLGOGLG@D@@@@B@NCN?NCN@N@B@@AHCLGNAHAHGNCLAHFFFFFFFFFF@@FF@@"
	"GO=K=KGKAKAKAK@@CLFFCHFLFLCH<LGH@@@@@@@@GNGNGN@@AHCLGNAHGNCLAHGN"
	"AHCLGNAHAHAHAH@@AHAHAHAHGNCLAH@@@@AH@L?N@LAH@@@@@@C@F@?NF@C@@@@@"
	"@@@@<@<@<@?N@@@@@@BDFF?OFFBD@@@@A@A@CHCHGLGL?N@@@@?NGLGLCHCHA@A@"
	"@@@@@@@@@@@@@@@@AHCLCLAHAH@@AH@@FLFLBH@@@@@@@@@@FLFL?NFL?NFLFL@@"
	"AHCNF@CL@FGLAH@@@@<F<LAHC@FF<F@@CHFLFLCIFNFFCK@@C@C@F@@@@@@@@@@@"
	"@LAHC@C@C@AH@L@@C@AH@L@L@LAHC@@@@@FFCL?OCLFF@@@@@@AHAHGNAHAH@@@@"
	"@@@@@@@@@@CHCHF@@@@@@@GN@@@@@@@@@@@@@@@@@@C@C@@@@F@LAHC@F@<@8@@@"
	"GL<F<N=N?F>FGL@@@HAHGHAHAHAHGN@@CLFF@FALC@FFGN@@CLFF@FAL@FFFCL@@"
	"ALCLFL<L?N@LAN@@GNF@F@GL@FFFCL@@ALC@F@GLFFFFCL@@GNFF@F@LAHAHAH@@"
	"CLFFGFCLFNFFCL@@CLFFFFCN@F@LCH@@@@@@C@C@@@C@C@@@@@@@C@C@@@G@C@F@"
	"@F@LAHC@AH@L@F@@@@@@GN@@GN@@@@@@F@C@AH@LAHC@F@@@CLFF@F@LAH@@AH@@"
	"GL<F=N=N=N<@GL@@AHCLFFFFGNFFFF@@?LFFFFGLFFFF?L@@CLFF<@<@<@FFCL@@"
	"?HFLFFFFFFFL?H@@?NFBFHGHFHFB?N@@?NFBFHGHFHF@?@@@CLFF<@<@<NFFCN@@"
	"FFFFFFGNFFFFFF@@CLAHAHAHAHAHCL@@AN@L@L@L<L<LGH@@>FFFFLGHFLFF>F@@"
	"?@F@F@F@FBFF?N@@<F>N?N=F<F<F<F@@<F>F?F=N<N<F<F@@CHFL<F<F<FFLCH@@"
	"?LFFFFGLF@F@?@@@CLFFFFFFFNCL@N@@?LFFFFGLFLFF>F@@CLFFF@CL@NFFCL@@"
	"GNEJAHAHAHAHCL@@FFFFFFFFFFFFCL@@FFFFFFFFFFCLAH@@<F<F<F=F?NFLFL@@"
	"FFFFCLAHCLFFFF@@FFFFFFCLAHAHCL@@?N<L9HC@FB<F?N@@CLC@C@C@C@C@CL@@"
	"<@F@C@AH@L@F@B@@CL@L@L@L@L@LCL@@A@CHFL<F@@@@@@@@@@@@@@@@@@@@?O@@"
	"C@C@AH@@@@@@@@@@@@@@GH@LGL<LGF@@>@F@GLFFFFFF=L@@@@@@CLFFF@FFCL@@"
	"AL@LGL<L<L<LGF@@@@@@CLFFGNF@CL@@ALCFC@GLC@C@GH@@@@@@GF<L<LGL@L?H"
	">@F@FLGFFFFF>F@@AH@@CHAHAHAHCL@@@L@@CL@L@L@L<LGH>@F@FFFLGHFL>F@@"
	"GHAHAHAHAHAHGN@@@@@@>L?N=F=F<F@@@@@@GLFFFFFFFF@@@@@@CLFFFFFFCL@@"
	"@@@@=LFFFFGLF@?@@@@@GF<L<LGL@LAN@@@@>LGFFFF@?@@@@@@@CNF@CL@FGL@@"
	"A@C@GNC@C@CFAL@@@@@@<L<L<L<LGF@@@@@@FFFFFFCLAH@@@@@@<F<F=F?NFL@@"
	"@@@@<FFLCHFL<F@@@@@@FFFFFFCN@FGL@@@@GNDLAHCBGN@@@NAHAHG@AHAH@N@@"
	"AHAHAH@@AHAHAH@@G@AHAH@NAHAHG@@@GF=L@@@@@@@@@@@@@@A@CHFL<F<F?N@@"
	"CLFF<@<@FFCLAHG@@@<L@@<L<L<LGF@@@NAHCLFFGNF@CL@@GN<CCL@FCNFFCK@@"
	"<L@@GH@LGL<LGF@@>@C@GH@LGL<LGF@@AHBDGH@LGL<LGF@@@@CLFFF@FFCLAHG@"
	"GN<CCLFFGNF@CL@@FF@@CLFFGNF@CL@@G@AHCLFFGNF@CL@@FF@@CHAHAHAHCL@@"
	"GL<FCHAHAHAHCL@@G@AHCHAHAHAHCL@@FFAHCLFFFFGNFF@@AHBDCLFFFFGNFF@@"
	"ALC@GLF@GHF@GL@@@@@@?NAKGO=H>O@@AOCLFLGOFLFLFO@@CLFF@@CLFFFFCL@@"
	"@@FF@@CLFFFFCL@@G@AH@@CLFFFFCL@@GH<L@@<L<L<LGF@@>@C@@@<L<L<LGF@@"
	"@@FF@@FFFFCN@FGL<FCHFL<F<FFLCH@@FF@@FFFFFFFFCL@@AHAHGN<@<@GNAHAH"
	"CLFFF@?HF@<@?N@@FFFFCLGNAHGNAH@@?@8H8H?@8H9L8H@N@NAKAHGNAHAH=HG@"
	"ALC@GH@LGL<LGF@@ALC@CHAHAHAHCL@@@NAH@@CLFFFFCL@@ALC@@@<L<L<LGF@@"
	"@@CFFLGLFFFFFF@@CFFLFFGFGNFNFF@@CLFLFLCN@@GN@@@@CLFFFFCL@@GN@@@@"
	"AH@@AHC@F@FFCL@@@@@@@@GNF@F@@@@@@@@@@@GN@F@F@@@@<F<L=HCNFC<F8LAO"
	"<F<L=H?CFG<K9O@C@@AH@@AHAHCLCLAH@@CCFF<LFFCC@@@@@@<LFFCCFF<L@@@@"
	"BB8HBB8HBB8HBB8HEE:JEE:JEE:JEE:J=MGG=MGG=MGG=MGGAHAHAHAHAHAHAHAH"
	"AHAHAHAH?HAHAHAHAHAH?HAH?HAHAHAHCFCFCFCF?FCFCFCF@@@@@@@@?NCFCFCF"
	"@@@@?HAH?HAHAHAHCFCF?F@F?FCFCFCFCFCFCFCFCFCFCFCF@@@@?N@F?FCFCFCF"
	"CFCF?F@F?N@@@@@@CFCFCFCF?N@@@@@@AHAH?HAH?H@@@@@@@@@@@@@@?HAHAHAH"
	"AHAHAHAHAO@@@@@@AHAHAHAH?O@@@@@@@@@@@@@@?OAHAHAHAHAHAHAHAOAHAHAH"
	"@@@@@@@@?O@@@@@@AHAHAHAH?OAHAHAHAHAHAOAHAOAHAHAHCFCFCFCFCGCFCFCF"
	"CFCFCGC@CO@@@@@@@@@@COC@CGCFCFCFCFCF?G@@?O@@@@@@@@@@?O@@?GCFCFCF"
	"CFCFCGC@CGCFCFCF@@@@?O@@?O@@@@@@CFCF?G@@?GCFCFCFAHAH?O@@?O@@@@@@"
	"CFCFCFCF?O@@@@@@@@@@?O@@?OAHAHAH@@@@@@@@?OCFCFCFCFCFCFCFCO@@@@@@"
	"AHAHAOAHAO@@@@@@@@@@AOAHAOAHAHAH@@@@@@@@COCFCFCFCFCFCFCF?GCFCFCF"
	"AHAH?O@@?OAHAHAHAHAHAHAH?H@@@@@@@@@@@@@@AOAHAHAH?O?O?O?O?O?O?O?O"
	"@@@@@@@@?O?O?O?O?@?@?@?@?@?@?@?@@O@O@O@O@O@O@O@O?O?O?O?O@@@@@@@@"
	"@@@@GF=L<H=LGF@@@@CLFFFLFFGLF@C@@@GNFFF@F@F@F@@@@@?NFLFLFLFLFF@@"
	"GNFFC@AHC@FFGN@@@@@@GN<H<L<LGH@@@@FFFFFFFFGKF@<@@@GF=LAHAHAH@N@@"
	"GNAHCLFFFFCLAHGNCLFFFFGNFFFFCL@@GL<F<F<FFLFL>N@@ANC@AHCLFFFFCL@@"
	"@@@@GF=K=KFN@@@@@F@LGN=K=KGNF@<@ANC@F@GNF@C@AN@@@@CLFFFFFFFFFF@@"
	"@@GN@@GN@@GN@@@@AHAHGNAHAH@@GN@@C@AH@LAHC@@@GN@@@LAHC@AH@L@@GN@@"
	"@NAKAKAHAHAHAHAHAHAHAHAHAH=H=HG@AHAH@@GN@@AHAH@@@@GB9L@@GB9L@@@@"
	"CHFLFLCH@@@@@@@@@@@@@@AHAH@@@@@@@@@@@@@@AH@@@@@@@G@D@DDDFDCDAL@L"
	"FLCFCFCFCF@@@@@@GH@LAHC@GL@@@@@@@@@@CLCLCLCL@@@@CLFF<C=K=K<CFFCL"

	,minifontdata[]=
	"@@@C@D@F@K@NAAADAEAGAIALAOB@BCBDBGBJBLBOCBCECHCKCNDADDDEDFDHDJDL"
	"DOECEFEIELEOFBFEFHFKFLFOGBGEGJGNHAHDHGHJHMI@ICIFIKINJAJDJFJIJKJN"
	"KAKBKEKHKJKML@LBLELHLILKLNM@MEMHMKMNNANDNFNHNKNNOCOFOIOKONOO@B@D"
	"@@H@CKHFOJNJBKOKEKBFECEECMELHLDNAADEALLAHDA@HHLIG@MEHIIOJKHLEEAI"
	"OBFGDMJNJFM@HGJJDJEFJGCDEADJBIABJJG@KAAJIOOLJOLJDEOEJCOKCEONABEL"
	"LLIOOLAOEOOHFDGO@HKOB@NBGODAKNHKNNAGNAJBOODELKIJLB@O@OON@GMOHC@L"
	"KNDMOFN@BCMGOINCB@CHGNAA@DHDM@B@HJCODEHHBBOJIIJEKNLHNJCNLAAJKFHO"
	"NLHCILG@HBCILAHHBBEOHDBHINBCDBMEJC@LGB@FAIHC@HCBHJBHDEDDEKNBNBHO"
	"HNA@LAAD",midfontdata[]=
	"@@@@NND@JJ@@JDN@FNL@HJB@FLN@DD@@BDB@HDH@@D@@DND@@@DD@N@@@@D@BDH@"
	"NJN@DDN@BLN@NFN@JNB@NLB@HJN@NBB@NNN@NJB@D@D@D@DDFHF@N@N@LBL@NFD@"
	"NHJ@FNJ@LNN@NHN@NJL@NLN@NLH@LJN@JNJ@NDN@NDL@JLJ@HHN@NNJ@NJJ@FJN@"
	"NNH@NJF@NLJ@FDL@NDD@JJN@JJD@JNN@JDJ@JND@LDF@FDF@HDB@LDL@DJ@@@@N@"
	"HD@@@FN@HNN@@NL@BNN@@LN@FLD@@NJFHNJ@@DN@@FBLHLJ@LDN@@NJ@@LJ@@NN@"
	"@FNH@LNB@NH@@FL@DLF@@JF@@JL@@JN@@JJ@@JNF@LF@FLF@DDD@LFL@BNH@@@@";

/*
keys
	key update
	keysym
	plz make an enum for checking, this is currently a nightmare
	modifiers (shift ctrl alt etc)
fix delay
	disable key repeat?
optional sound

transparency - for backwards compatibility / ease of use,
 inverted the alpha channel

have the pc function take font as an argument instead of having separate
 functions for all possible fonts?

maybe include some basic lzw?

fonts are bytepacked incorrectly (sign issues)
convert to base64 for interoperability?

changed a buncha stuff regarding how sdl2 is called - should improve speed
 a bunch via native routines, while still being mostly a drop-in replacement
 for libv2.h

alpha is still broken. seems like well have to do it in software after all

*/

#endif // AVES_FRAMEWORK include guard
