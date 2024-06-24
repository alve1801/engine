#ifndef AVES_FRAMEWORK
#define AVES_FRAMEWORK

#include <SDL2/SDL.h>
#include <time.h> // for sleep()
#include <math.h> // for log10, so we know how long integers are XXX consider removing
#include <string.h> // for memset/memcpy that we use for keymaps
#define _max(x,y) ((x)>(y)?(x):(y))
#define _min(x,y) ((x)<(y)?(x):(y))

int mod(int a,int b){a%=b;return(a>=0?a:b+a);}
int mdiv(int a,int b){return (a+(a<0))/b-(a<0);}

struct img{ // ppm format
	int sx=0,sy=0;
	short*data=0;
	img():sx(0),sy(0),data(0){} //~img(){if(data){free(data);data=0;}}
	img(int x,int y,short*d=0):sx(x),sy(y){if(d)data=d;else data=(short*)malloc(x*y*sizeof(short));}
	// ppm pgm pbm

	img(const char*filename){load(filename);}

	void clear(short col=0){for(int i=0;i<sx*sy;i++)data[i]=col;}

	void load(const char*filename){
		if(data)free(data);data=0,sx=sy=0;
		//printf("filename %s\n",filename);

		char a;
		FILE*f=fopen(filename,"r");
		if(!f)return;
		for(int i=0;i<3;i++)getc(f); // P6\n
		while((a=getc(f))!=' ')sx=sx*10+a-48; // <X> 
		while((a=getc(f))!=10)sy=sy*10+a-48; // <Y>\n

		//printf("  %i %i\n",sx,sy);

		data=(short*)malloc(sx*sy*sizeof(short));
		while((a=getc(f))!=10); // 255\n

		for(int i=0;i<sx*sy;i++)
			data[i]=
				(getc(f)>>4)<<8|
				(getc(f)>>4)<<4|
				(getc(f)>>4)<<0; // fancy!
		fclose(f);
	}

	void save(const char*filename){
		FILE*f=fopen(filename,"w");
		fprintf(f,"P6\n%i %i\n255\n",sx,sy);
		for(int i=0;i<sx*sy;i++)
			//putc((data[i]&0xf00)>>4,f),
			//putc((data[i]&0x0f0)   ,f),
			//putc((data[i]&0x00f)<<4,f);

			putc(((data[i]&0xf00)>>4)|((data[i]&0xf00)>>8),f),
			putc((data[i]&0xf0)|((data[i]&0xf0)>>4),f),
			putc(((data[i]&0xf)<<4)|(data[i]&0xf),f);
		fclose(f);
	}

	// XXX p?

	short&operator[](int i){return data[i];}
};

struct Framework{
	const char fontdata[]="@@@@@@@@@@@@@@@@GN8A:E8A?O=K?OGNGN?O=K?O?O>G?OGNFL?N?N?NGLCHA@@@A@CHGL?NGLCHA@@@A@CHCH?N>N>NA@GLA@CHGL?N?NGLA@GL@@@@CLGNGNCL@@@@?O?O<C8A8A<C?O?O@@CHGLDDDDGLCH@@?O<G8C;K;K8C<G?OCN@NCJGH<L<L<LGHCLFFFFFFCLAHGNAHCOCCCOC@C@G@?@>@GOFCGOFCFCFG>F<@AH=KCL>G>GCL=KAHD@G@GLGOGLG@D@@@@B@NCN?NCN@N@B@@AHCLGNAHAHGNCLAHFFFFFFFFFF@@FF@@GO=K=KGKAKAKAK@@CLFFCHFLFLCH<LGH@@@@@@@@GNGNGN@@AHCLGNAHGNCLAHGNAHCLGNAHAHAHAH@@AHAHAHAHGNCLAH@@@@AH@L?N@LAH@@@@@@C@F@?NF@C@@@@@@@@@<@<@<@?N@@@@@@BDFF?OFFBD@@@@A@A@CHCHGLGL?N@@@@?NGLGLCHCHA@A@@@@@@@@@@@@@@@@@AHCLCLAHAH@@AH@@FLFLBH@@@@@@@@@@FLFL?NFL?NFLFL@@AHCNF@CL@FGLAH@@@@<F<LAHC@FF<F@@CHFLFLCIFNFFCK@@C@C@F@@@@@@@@@@@@LAHC@C@C@AH@L@@C@AH@L@L@LAHC@@@@@FFCL?OCLFF@@@@@@AHAHGNAHAH@@@@@@@@@@@@@@CHCHF@@@@@@@GN@@@@@@@@@@@@@@@@@@C@C@@@@F@LAHC@F@<@8@@@GL<F<N=N?F>FGL@@@HAHGHAHAHAHGN@@CLFF@FALC@FFGN@@CLFF@FAL@FFFCL@@ALCLFL<L?N@LAN@@GNF@F@GL@FFFCL@@ALC@F@GLFFFFCL@@GNFF@F@LAHAHAH@@CLFFGFCLFNFFCL@@CLFFFFCN@F@LCH@@@@@@C@C@@@C@C@@@@@@@C@C@@@G@C@F@@F@LAHC@AH@L@F@@@@@@GN@@GN@@@@@@F@C@AH@LAHC@F@@@CLFF@F@LAH@@AH@@GL<F=N=N=N<@GL@@AHCLFFFFGNFFFF@@?LFFFFGLFFFF?L@@CLFF<@<@<@FFCL@@?HFLFFFFFFFL?H@@?NFBFHGHFHFB?N@@?NFBFHGHFHF@?@@@CLFF<@<@<NFFCN@@FFFFFFGNFFFFFF@@CLAHAHAHAHAHCL@@AN@L@L@L<L<LGH@@>FFFFLGHFLFF>F@@?@F@F@F@FBFF?N@@<F>N?N=F<F<F<F@@<F>F?F=N<N<F<F@@CHFL<F<F<FFLCH@@?LFFFFGLF@F@?@@@CLFFFFFFFNCL@N@@?LFFFFGLFLFF>F@@CLFFF@CL@NFFCL@@GNEJAHAHAHAHCL@@FFFFFFFFFFFFCL@@FFFFFFFFFFCLAH@@<F<F<F=F?NFLFL@@FFFFCLAHCLFFFF@@FFFFFFCLAHAHCL@@?N<L9HC@FB<F?N@@CLC@C@C@C@C@CL@@<@F@C@AH@L@F@B@@CL@L@L@L@L@LCL@@A@CHFL<F@@@@@@@@@@@@@@@@@@@@?O@@C@C@AH@@@@@@@@@@@@@@GH@LGL<LGF@@>@F@GLFFFFFF=L@@@@@@CLFFF@FFCL@@AL@LGL<L<L<LGF@@@@@@CLFFGNF@CL@@ALCFC@GLC@C@GH@@@@@@GF<L<LGL@L?H>@F@FLGFFFFF>F@@AH@@CHAHAHAHCL@@@L@@CL@L@L@L<LGH>@F@FFFLGHFL>F@@GHAHAHAHAHAHGN@@@@@@>L?N=F=F<F@@@@@@GLFFFFFFFF@@@@@@CLFFFFFFCL@@@@@@=LFFFFGLF@?@@@@@GF<L<LGL@LAN@@@@>LGFFFF@?@@@@@@@CNF@CL@FGL@@A@C@GNC@C@CFAL@@@@@@<L<L<L<LGF@@@@@@FFFFFFCLAH@@@@@@<F<F=F?NFL@@@@@@<FFLCHFL<F@@@@@@FFFFFFCN@FGL@@@@GNDLAHCBGN@@@NAHAHG@AHAH@N@@AHAHAH@@AHAHAH@@G@AHAH@NAHAHG@@@GF=L@@@@@@@@@@@@@@A@CHFL<F<F?N@@CLFF<@<@FFCLAHG@@@<L@@<L<L<LGF@@@NAHCLFFGNF@CL@@GN<CCL@FCNFFCK@@<L@@GH@LGL<LGF@@>@C@GH@LGL<LGF@@AHBDGH@LGL<LGF@@@@CLFFF@FFCLAHG@GN<CCLFFGNF@CL@@FF@@CLFFGNF@CL@@G@AHCLFFGNF@CL@@FF@@CHAHAHAHCL@@GL<FCHAHAHAHCL@@G@AHCHAHAHAHCL@@FFAHCLFFFFGNFF@@AHBDCLFFFFGNFF@@ALC@GLF@GHF@GL@@@@@@?NAKGO=H>O@@AOCLFLGOFLFLFO@@CLFF@@CLFFFFCL@@@@FF@@CLFFFFCL@@G@AH@@CLFFFFCL@@GH<L@@<L<L<LGF@@>@C@@@<L<L<LGF@@@@FF@@FFFFCN@FGL<FCHFL<F<FFLCH@@FF@@FFFFFFFFCL@@AHAHGN<@<@GNAHAHCLFFF@?HF@<@?N@@FFFFCLGNAHGNAH@@?@8H8H?@8H9L8H@N@NAKAHGNAHAH=HG@ALC@GH@LGL<LGF@@ALC@CHAHAHAHCL@@@NAH@@CLFFFFCL@@ALC@@@<L<L<LGF@@@@CFFLGLFFFFFF@@CFFLFFGFGNFNFF@@CLFLFLCN@@GN@@@@CLFFFFCL@@GN@@@@AH@@AHC@F@FFCL@@@@@@@@GNF@F@@@@@@@@@@@GN@F@F@@@@<F<L=HCNFC<F8LAO<F<L=H?CFG<K9O@C@@AH@@AHAHCLCLAH@@CCFF<LFFCC@@@@@@<LFFCCFF<L@@@@BB8HBB8HBB8HBB8HEE:JEE:JEE:JEE:J=MGG=MGG=MGG=MGGAHAHAHAHAHAHAHAHAHAHAHAH?HAHAHAHAHAH?HAH?HAHAHAHCFCFCFCF?FCFCFCF@@@@@@@@?NCFCFCF@@@@?HAH?HAHAHAHCFCF?F@F?FCFCFCFCFCFCFCFCFCFCFCF@@@@?N@F?FCFCFCFCFCF?F@F?N@@@@@@CFCFCFCF?N@@@@@@AHAH?HAH?H@@@@@@@@@@@@@@?HAHAHAHAHAHAHAHAO@@@@@@AHAHAHAH?O@@@@@@@@@@@@@@?OAHAHAHAHAHAHAHAOAHAHAH@@@@@@@@?O@@@@@@AHAHAHAH?OAHAHAHAHAHAOAHAOAHAHAHCFCFCFCFCGCFCFCFCFCFCGC@CO@@@@@@@@@@COC@CGCFCFCFCFCF?G@@?O@@@@@@@@@@?O@@?GCFCFCFCFCFCGC@CGCFCFCF@@@@?O@@?O@@@@@@CFCF?G@@?GCFCFCFAHAH?O@@?O@@@@@@CFCFCFCF?O@@@@@@@@@@?O@@?OAHAHAH@@@@@@@@?OCFCFCFCFCFCFCFCO@@@@@@AHAHAOAHAO@@@@@@@@@@AOAHAOAHAHAH@@@@@@@@COCFCFCFCFCFCFCF?GCFCFCFAHAH?O@@?OAHAHAHAHAHAHAH?H@@@@@@@@@@@@@@AOAHAHAH?O?O?O?O?O?O?O?O@@@@@@@@?O?O?O?O?@?@?@?@?@?@?@?@@O@O@O@O@O@O@O@O?O?O?O?O@@@@@@@@@@@@GF=L<H=LGF@@@@CLFFFLFFGLF@C@@@GNFFF@F@F@F@@@@@?NFLFLFLFLFF@@GNFFC@AHC@FFGN@@@@@@GN<H<L<LGH@@@@FFFFFFFFGKF@<@@@GF=LAHAHAH@N@@GNAHCLFFFFCLAHGNCLFFFFGNFFFFCL@@GL<F<F<FFLFL>N@@ANC@AHCLFFFFCL@@@@@@GF=K=KFN@@@@@F@LGN=K=KGNF@<@ANC@F@GNF@C@AN@@@@CLFFFFFFFFFF@@@@GN@@GN@@GN@@@@AHAHGNAHAH@@GN@@C@AH@LAHC@@@GN@@@LAHC@AH@L@@GN@@@NAKAKAHAHAHAHAHAHAHAHAHAH=H=HG@AHAH@@GN@@AHAH@@@@GB9L@@GB9L@@@@CHFLFLCH@@@@@@@@@@@@@@AHAH@@@@@@@@@@@@@@AH@@@@@@@G@D@DDDFDCDAL@LFLCFCFCFCF@@@@@@GH@LAHC@GL@@@@@@@@@@CLCLCLCL@@@@CLFF<C=K=K<CFFCL";
	const char minifontdata[]="@@@C@D@F@K@NAAADAEAGAIALAOB@BCBDBGBJBLBOCBCECHCKCNDADDDEDFDHDJDLDOECEFEIELEOFBFEFHFKFLFOGBGEGJGNHAHDHGHJHMI@ICIFIKINJAJDJFJIJKJNKAKBKEKHKJKML@LBLELHLILKLNM@MEMHMKMNNANDNFNHNKNNOCOFOIOKONOO@B@D@@H@CKHFOJNJBKOKEKBFECEECMELHLDNAADEALLAHDA@HHLIG@MEHIIOJKHLEEAIOBFGDMJNJFM@HGJJDJEFJGCDEADJBIABJJG@KAAJIOOLJOLJDEOEJCOKCEONABELLLIOOLAOEOOHFDGO@HKOB@NBGODAKNHKNNAGNAJBOODELKIJLB@O@OON@GMOHC@LKNDMOFN@BCMGOINCB@CHGNAA@DHDM@B@HJCODEHHBBOJIIJEKNLHNJCNLAAJKFHONLHCILG@HBCILAHHBBEOHDBHINBCDBMEJC@LGB@FAIHC@HCBHJBHDEDDEKNBNBHOHNA@LAAD";
	// https://simplifier.neocities.org/4x4 - posted under the "Feel free to use it for whatever you like" "license"
	const char midfontdata[]="@@@@NND@JJ@@JDN@FNL@HJB@FLN@DD@@BDB@HDH@@D@@DND@@@DD@N@@@@D@BDH@NJN@DDN@BLN@NFN@JNB@NLB@HJN@NBB@NNN@NJB@D@D@D@DDFHF@N@N@LBL@NFD@NHJ@FNJ@LNN@NHN@NJL@NLN@NLH@LJN@JNJ@NDN@NDL@JLJ@HHN@NNJ@NJJ@FJN@NNH@NJF@NLJ@FDL@NDD@JJN@JJD@JNN@JDJ@JND@LDF@FDF@HDB@LDL@DJ@@@@N@HD@@@FN@HNN@@NL@BNN@@LN@FLD@@NJFHNJ@@DN@@FBLHLJ@LDN@@NJ@@LJ@@NN@@FNH@LNB@NH@@FL@DLF@@JF@@JL@@JN@@JJ@@JNF@LF@FLF@DDD@LFL@BNH@@@@";

	int _sx,_sy,_ss; // width, height, scale
	short*newmap,*oldmap; // XXX rewrite these to be imgs?
	unsigned char font[1<<11],minifont[260],midfont[192];
	bool(*userfunc)(Framework*);
	//bool keypress; // XXX tells us whether a new key was pressed last frame - please figure out a better way to handle this
	//char key; // what key was pressed XXX make this an int, bc some are not ascii
	const unsigned char*keystate;unsigned char*last_keystate; // references/caches from sdl2
	int numkeys; // how many keys sdl2 is working with, iow how long the above arrays are
	struct{bool left,right;int x=0,y=0;}mouse;

	struct audios{
		SDL_AudioSpec want;
		SDL_AudioDeviceID dev;
		int stamp=0;
		unsigned char(*ostfunk)(int);
	}audio;

	static void audiowrapper(void*data,unsigned char*stream,int len){
		audios*a=(audios*)data;
		for(int i=0;i<len;i++)
			stream[i]=a->ostfunk(a->stamp++);
	}

	SDL_Window*window;
	SDL_Renderer*renderer;
	SDL_Surface*surface;
	SDL_Event event;

	Framework(int x,int y,int s=1,unsigned char(*ostfunk)(int)=0):_sx(x),_sy(y),_ss(s>0?s:1),userfunc(0){ // XXX add argument for userfunc?
		newmap=(short*)malloc(x*y*sizeof(short));
		oldmap=(short*)malloc(x*y*sizeof(short));
		for(int i=0;i<2048;i++)font[i]=(fontdata[2*i]<<4)|(fontdata[2*i+1]-'@');
		for(int i=0;i<260;i++)minifont[i]=(minifontdata[2*i]<<4)|(minifontdata[2*i+1]-'@');
		for(int i=0;i<192;i++)midfont[i]=(midfontdata[2*i]<<4)|(midfontdata[2*i+1]-'@');

		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
		//SDL_CreateWindowAndRenderer(_sx*_ss,_sy*_ss,SDL_WINDOW_RESIZABLE,&window,&renderer);
		SDL_CreateWindowAndRenderer(_sx*_ss,_sy*_ss,0,&window,&renderer);
		surface=SDL_GetWindowSurface(window);
		//SDL_CreateWindowAndRenderer(_sx*_ss,_sy*_ss,0,&window,&renderer);
		if(ostfunk){
			audio.ostfunk = ostfunk;
			audio.want.freq = 8000;
			audio.want.format = AUDIO_U8;
			audio.want.channels = 1;
			audio.want.samples = 512;
			audio.want.callback = &audiowrapper;
			audio.want.userdata = &(this->audio);
			audio.dev = SDL_OpenAudioDevice(0,0,&audio.want,0,SDL_AUDIO_ALLOW_FORMAT_CHANGE); // actually, we dont rlly want to allow any change...
			SDL_PauseAudioDevice(audio.dev,0);
		}

		keystate=SDL_GetKeyboardState(&numkeys); // GetKeyState also exists, and seems to do keystates instead
		last_keystate=(unsigned char*)malloc(numkeys);
		memset(last_keystate,0,numkeys);
	}

	~Framework(){
		free(newmap);
		free(oldmap);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		if(audio.ostfunk)
			SDL_CloseAudioDevice(audio.dev);
		SDL_Quit();
	}

	void clear(short c=0){for(int i=0;i<_sx*_sy;i++)newmap[i]=c;}

	//void p(short c,int x,int y){newmap[y*_sx+x]=c;}

	short transparent=0xf000;

	void p(unsigned short c,int x,int y){
		if(x<0||y<0||x>=_sx||y>=_sy||c==transparent)return;

		short n=newmap[y*_sx+x];
		char a=c>>12,r,g,b;
		if(a==0xf)return;
		if(a==0){newmap[y*_sx+x]=c;return;} // XXX skip calculating the exact value. not sure if it helps tho.

		r=((n&0xf00)>>8)*a + ((c&0xf00)>>8)*(0xf-a);
		g=((n&0x0f0)>>4)*a + ((c&0x0f0)>>4)*(0xf-a);
		b=((n&0x00f)   )*a + ((c&0x00f)   )*(0xf-a);

		//r/=0xf,g/=0xf,b/=0xf;
		r>>=4;g>>=4;b>>=4;
		newmap[y*_sx+x]=(r<<8)|(g<<4)|b;
	}

	void pc(char c,int cx,int cy,short fgc=0xfff,short bgc=0,bool abs=0){
		if(!abs)cx*=8,cy*=8;
		if(c)for(int x=0;x<8;x++)for(int y=0;y<8;y++)
		p(((char)font[(unsigned char)c*8+y]&(char)(128>>x))?fgc:bgc,cx+x,cy+y);
	}

	int ps(const char*c,int sx,int sy,short fgc=0xfff,short bgc=0,bool abs=0){
		int x=0,y=0,l=0;
		for(int i=0;c[i];i++){l++;
			if(c[i]==9)x=((x>>3)+1)<<3;else
			pc(c[i]==10?0:c[i],sx+x,sy+y,fgc,bgc,abs);
			x+=abs?8:1;
			if(sx+x==_sx||c[i]==10)x=0,y++;
		}
		return l;
	}

	int pcm(char c,int cx,int cy,short fgc=0xfff,short bgc=0){
		if(c<32||c>127)return 0;c-=32;
		int s=minifont[(int)c+1]-minifont[(int)c],i=minifont[(int)c]*5;
		if(s<0)return 0; // temp
		for(int x=0;x<s;x++)for(int y=0;y<5;y++){
			p((minifont[96+i/8]&(1<<(i%8)))?fgc:bgc,cx+x,cy+y);
			i++;
		}
		return s;
	}

	int psm(const char*c,int cx,int cy,short fgc=0xfff,short bgc=0){
		int x=0,y=0,i=0;
		for(;c[i];i++){
			if(c[i]==10)x=0,y+=6;
			else x+=pcm(c[i],cx+x,cy+y,fgc,bgc)+1;
		}
		return i; // or width? then, measured how?
	}

	void pcmid(char c,int cx,int cy,short fgc=0xfff,short bgc=0){
		if(c<32||c>127)return;c-=32; // cant predouble c bc signed
		for(int i=0;i<8;i++)
		p(midfont[2*c  ]&(128>>i)?fgc:bgc,cx+(i%4),cy+i/4  ),
		p(midfont[2*c+1]&(128>>i)?fgc:bgc,cx+(i%4),cy+i/4+2);
	}

	int psmid(const char*c,int cx,int cy,short fgc=0xfff,short bgc=0){
		int x=0,y=0,i=0;
		for(;c[i];i++){
			if(c[i]==10)x=0,y+=4;
			else pcmid(c[i],cx+x,cy+y,fgc,bgc),x+=4;
		}return i;
	}

	int pi(int i,int sx,int sy,short fgc=0xfff,short bgc=0){
		// returns length of number
		// somewhat different approach than what we're used to
		if(!i){pc('0',sx,sy,fgc,bgc);return 1;}
		if(i<0){
			pc('-',sx,sy,fgc,bgc);
			return pi(-1*i,sx+1,sy,fgc,bgc)+1;
		}

		int len=(int)(log10(i)+1);
		char s[len+1];
		sprintf(s,"%i",i);
		ps(s,sx,sy,fgc,bgc);
		return len;
	}


	int pis(int i,int sx,int sy,short fgc=0xfff,short bgc=0){
		// haha piss
		static unsigned char mininum[]={0x99,0xf0,0xbd,0xf9,0xe3,0xdb,0xdf,0xf1,0xff,0xfb,0x44};

		if(i<0){ // XXX feels like this should be easier...
			for(int x=0;x<4;x++){
				p((mininum[10]>>x)&0x01?fgc:bgc,sx+0,sy+x);
				p((mininum[10]>>x)&0x10?fgc:bgc,sx+1,sy+x);
				p(bgc,sx+2,sy+x);
			}
			return pis(-i,sx+3,sy,fgc,bgc)+1;
		}

		int len=(int)(log10(i)+1);
		if(len<0)len=1; // for 0
		for(int j=len;j--;i/=10)
			for(int x=0;x<4;x++){
				p((mininum[i%10]>>x)&0x01?fgc:bgc,sx+0+j*3,sy+x);
				p((mininum[i%10]>>x)&0x10?fgc:bgc,sx+1+j*3,sy+x);
				p(bgc,sx+2+j*3,sy+x);
			}

		return len;
	}
	// recurses if not 0
	// displays result of recursion %10
	// returns input/10

	void pi(short*img,int cx,int cy,int sx,int sy){ // blip a whole image
		if(cx>_sx||cy>_sy||cx+sx<0||cy+sy<0)return; // dont iterate at all if image is out of bounds
		// XXX optimise to only iterate over visible part of image
		for(int x=0;x<sx;x++)
		for(int y=0;y<sy;y++)
		if(cx+x<_sx && cy+y<_sy)
		p(img[y*sx+x],cx+x,cy+y);
	}

	// the following are utility and not necessary, remove them for the 1173 version

	/*
	void line(unsigned short c,int x1,int x2,int y1,int y2,int skip=1){
		// XXX rewrite this to use bresenhams
		// XXX skip doesnt work
		int x,y,i;
		float d=abs(abs(x1-x2)>abs(y1-y2)?x1-x2:y1-y2);
		for(i=0;i<(int)d;i++){
			x=(int)((x1*i+x2*(d-i))/d);
			y=(int)((y1*i+y2*(d-i))/d);
			if(i&~skip)p(c,x,y);
		}
	}
	*/

	void pi(img a,int cx,int cy){pi(a.data,cx,cy,a.sx,a.sy);}

	/* XXX not sure if i like defining start+offset instead of start+end...
	da c2 c4 bf
	c3 c5    b4
	b3
	c0 c1    d9
	*/

	void phl(int sx,int y,int ex,short fgc=0xfff,short bgc=0){for(int x=sx;x<sx+ex;x++)pc(0xc4,x,y,fgc,bgc);}

	void pvl(int x,int sy,int ey,short fgc=0xfff,short bgc=0){for(int y=sy;y<sy+ey;y++)pc(0xb3,x,y,fgc,bgc);}

	void pbox(int sx,int sy,int ex=1,int ey=1,short fgc=0xfff,short bgc=0){
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
		printf("Screenshot taken.\n");
	}

	// end 1173

	//bool operator[](char c){return SDL_GetKeyboardState(NULL)[c];}

	// XXX indexes are sdl_scancodes, which are kinda fucky to work w/. see if we cant hardcode some of the math? how do scancodes even work
	// good news is that scancodes represent physical keys and not virtual keys, so its layout independent :3
	bool key(int key){return keystate[key];}
	bool keyh(int key){return keystate[key]&&last_keystate[key];}
	bool keyp(int key){return keystate[key]&&(!last_keystate[key]);}
	bool keyr(int key){return (!keystate[key])&&last_keystate[key];}

	void run(bool(*ufunc)(Framework*)=0){
		if(ufunc)userfunc=ufunc;
		for(;;){
			int time = SDL_GetTicks();
			clear(); // XXX

			memcpy(last_keystate,keystate,numkeys);
			SDL_PumpEvents();

			int mstate=SDL_GetMouseState(&mouse.x,&mouse.y);
			mouse.left=mstate&SDL_BUTTON(1),mouse.right=mstate&SDL_BUTTON(3);
			mouse.x/=_ss,mouse.y/=_ss;

			bool ret=userfunc&&!userfunc(this);

			// blit screenbuf to screen
			for(int x=0;x<_sx;x++)for(int y=0;y<_sy;y++)
			if(newmap[y*_sx+x]!=oldmap[y*_sx+x]){
				// calculate pixel color XXX i still have a feeling this can be done better...
				unsigned int color=oldmap[y*_sx+x]=newmap[y*_sx+x];
				color = ((color&0xf00)<<8)|((color&0xf0)<<4)|((color&0xf));
				color |= color<<4;
				for(int scx=0;scx<_ss;scx++)for(int scy=0;scy<_ss;scy++)
				((unsigned int*)surface->pixels)[(y*_ss+scy)*_sx*_ss + x*_ss+scx]=color; // XXX thisll have to be shifted
			}

			SDL_UpdateWindowSurface(window);
			if(ret)return; // no frame delay if were quitting anyway
			time = (1000/24) + time - SDL_GetTicks();
			//printf("frame %i\n",time);
			if(time>0)SDL_Delay(time);
		}
	}
};

/*void compress(char*data,int dlen=0){ // for new fonts etc
	// entries done row first
	for(int i=0;i<dlen;i++)
	putchar((data[i]>>4)+'@'),
	putchar((data[i]&0xf)+'@');
}*/

/*
keys
	key update
	keysym
	plz make an enum for checking, this is currently a nightmare
	modifiers (shift ctrl alt etc)
fix delay
	disable key repeat?
optional sound

transparency - for backwards compatibility / ease of use, inverted the alpha channel

treat newmap and oldmap as regular imgs, make functions operate on imgs and defaulting to newmap?

scancodes should refer to the keyboard keys and not their values (eg pressing the 'a' key on an azerty keyboard gives us a 'q' scancode and an 'a' keycode)

have the pc function take font as an argument instead of having separate functions for all possible fonts?

maybe include some basic lzw?

*/

#endif
