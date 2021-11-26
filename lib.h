#include <SDL2/SDL.h>
#include <time.h>
#define _max(x,y) ((x)>(y)?(x):(y))

int mod(int a,int b){a%=b;return(a>=0?a:b+a);}
int mdiv(int a,int b){return a/b-(a<0);}

const char fontdata[]="@@@@@@@@@@@@@@@@GN8A:E8A?O=K?OGNGN?O=K?O?O>G?OGNFL?N?N?NGLCHA@@@A@CHGL?NGLCHA@@@A@CHCH?N>N>NA@GLA@CHGL?N?NGLA@GL@@@@CLGNGNCL@@@@?O?O<C8A8A<C?O?O@@CHGLDDDDGLCH@@?O<G8C;K;K8C<G?OCN@NCJGH<L<L<LGHCLFFFFFFCLAHGNAHCOCCCOC@C@G@?@>@GOFCGOFCFCFG>F<@AH=KCL>G>GCL=KAHD@G@GLGOGLG@D@@@@B@NCN?NCN@N@B@@AHCLGNAHAHGNCLAHFFFFFFFFFF@@FF@@GO=K=KGKAKAKAK@@CLFFCHFLFLCH<LGH@@@@@@@@GNGNGN@@AHCLGNAHGNCLAHGNAHCLGNAHAHAHAH@@AHAHAHAHGNCLAH@@@@AH@L?N@LAH@@@@@@C@F@?NF@C@@@@@@@@@<@<@<@?N@@@@@@BDFF?OFFBD@@@@A@A@CHCHGLGL?N@@@@?NGLGLCHCHA@A@@@@@@@@@@@@@@@@@AHCLCLAHAH@@AH@@FLFLBH@@@@@@@@@@FLFL?NFL?NFLFL@@AHCNF@CL@FGLAH@@@@<F<LAHC@FF<F@@CHFLFLCIFNFFCK@@C@C@F@@@@@@@@@@@@LAHC@C@C@AH@L@@C@AH@L@L@LAHC@@@@@FFCL?OCLFF@@@@@@AHAHGNAHAH@@@@@@@@@@@@@@CHCHF@@@@@@@GN@@@@@@@@@@@@@@@@@@C@C@@@@F@LAHC@F@<@8@@@GL<F<N=N?F>FGL@@@HAHGHAHAHAHGN@@CLFF@FALC@FFGN@@CLFF@FAL@FFFCL@@ALCLFL<L?N@LAN@@GNF@F@GL@FFFCL@@ALC@F@GLFFFFCL@@GNFF@F@LAHAHAH@@CLFFGFCLFNFFCL@@CLFFFFCN@F@LCH@@@@@@C@C@@@C@C@@@@@@@C@C@@@G@C@F@@F@LAHC@AH@L@F@@@@@@GN@@GN@@@@@@F@C@AH@LAHC@F@@@CLFF@F@LAH@@AH@@GL<F=N=N=N<@GL@@AHCLFFFFGNFFFF@@?LFFFFGLFFFF?L@@CLFF<@<@<@FFCL@@?HFLFFFFFFFL?H@@?NFBFHGHFHFB?N@@?NFBFHGHFHF@?@@@CLFF<@<@<NFFCN@@FFFFFFGNFFFFFF@@CLAHAHAHAHAHCL@@AN@L@L@L<L<LGH@@>FFFFLGHFLFF>F@@?@F@F@F@FBFF?N@@<F>N?N=F<F<F<F@@<F>F?F=N<N<F<F@@CHFL<F<F<FFLCH@@?LFFFFGLF@F@?@@@CLFFFFFFFNCL@N@@?LFFFFGLFLFF>F@@CLFFF@CL@NFFCL@@GNEJAHAHAHAHCL@@FFFFFFFFFFFFCL@@FFFFFFFFFFCLAH@@<F<F<F=F?NFLFL@@FFFFCLAHCLFFFF@@FFFFFFCLAHAHCL@@?N<L9HC@FB<F?N@@CLC@C@C@C@C@CL@@<@F@C@AH@L@F@B@@CL@L@L@L@L@LCL@@A@CHFL<F@@@@@@@@@@@@@@@@@@@@?O@@C@C@AH@@@@@@@@@@@@@@GH@LGL<LGF@@>@F@GLFFFFFF=L@@@@@@CLFFF@FFCL@@AL@LGL<L<L<LGF@@@@@@CLFFGNF@CL@@ALCFC@GLC@C@GH@@@@@@GF<L<LGL@L?H>@F@FLGFFFFF>F@@AH@@CHAHAHAHCL@@@L@@CL@L@L@L<LGH>@F@FFFLGHFL>F@@GHAHAHAHAHAHGN@@@@@@>L?N=F=F<F@@@@@@GLFFFFFFFF@@@@@@CLFFFFFFCL@@@@@@=LFFFFGLF@?@@@@@GF<L<LGL@LAN@@@@>LGFFFF@?@@@@@@@CNF@CL@FGL@@A@C@GNC@C@CFAL@@@@@@<L<L<L<LGF@@@@@@FFFFFFCLAH@@@@@@<F<F=F?NFL@@@@@@<FFLCHFL<F@@@@@@FFFFFFCN@FGL@@@@GNDLAHCBGN@@@NAHAHG@AHAH@N@@AHAHAH@@AHAHAH@@G@AHAH@NAHAHG@@@GF=L@@@@@@@@@@@@@@A@CHFL<F<F?N@@CLFF<@<@FFCLAHG@@@<L@@<L<L<LGF@@@NAHCLFFGNF@CL@@GN<CCL@FCNFFCK@@<L@@GH@LGL<LGF@@>@C@GH@LGL<LGF@@AHBDGH@LGL<LGF@@@@CLFFF@FFCLAHG@GN<CCLFFGNF@CL@@FF@@CLFFGNF@CL@@G@AHCLFFGNF@CL@@FF@@CHAHAHAHCL@@GL<FCHAHAHAHCL@@G@AHCHAHAHAHCL@@FFAHCLFFFFGNFF@@AHBDCLFFFFGNFF@@ALC@GLF@GHF@GL@@@@@@?NAKGO=H>O@@AOCLFLGOFLFLFO@@CLFF@@CLFFFFCL@@@@FF@@CLFFFFCL@@G@AH@@CLFFFFCL@@GH<L@@<L<L<LGF@@>@C@@@<L<L<LGF@@@@FF@@FFFFCN@FGL<FCHFL<F<FFLCH@@FF@@FFFFFFFFCL@@AHAHGN<@<@GNAHAHCLFFF@?HF@<@?N@@FFFFCLGNAHGNAH@@?@8H8H?@8H9L8H@N@NAKAHGNAHAH=HG@ALC@GH@LGL<LGF@@ALC@CHAHAHAHCL@@@NAH@@CLFFFFCL@@ALC@@@<L<L<LGF@@@@CFFLGLFFFFFF@@CFFLFFGFGNFNFF@@CLFLFLCN@@GN@@@@CLFFFFCL@@GN@@@@AH@@AHC@F@FFCL@@@@@@@@GNF@F@@@@@@@@@@@GN@F@F@@@@<F<L=HCNFC<F8LAO<F<L=H?CFG<K9O@C@@AH@@AHAHCLCLAH@@CCFF<LFFCC@@@@@@<LFFCCFF<L@@@@BB8HBB8HBB8HBB8HEE:JEE:JEE:JEE:J=MGG=MGG=MGG=MGGAHAHAHAHAHAHAHAHAHAHAHAH?HAHAHAHAHAH?HAH?HAHAHAHCFCFCFCF?FCFCFCF@@@@@@@@?NCFCFCF@@@@?HAH?HAHAHAHCFCF?F@F?FCFCFCFCFCFCFCFCFCFCFCF@@@@?N@F?FCFCFCFCFCF?F@F?N@@@@@@CFCFCFCF?N@@@@@@AHAH?HAH?H@@@@@@@@@@@@@@?HAHAHAHAHAHAHAHAO@@@@@@AHAHAHAH?O@@@@@@@@@@@@@@?OAHAHAHAHAHAHAHAOAHAHAH@@@@@@@@?O@@@@@@AHAHAHAH?OAHAHAHAHAHAOAHAOAHAHAHCFCFCFCFCGCFCFCFCFCFCGC@CO@@@@@@@@@@COC@CGCFCFCFCFCF?G@@?O@@@@@@@@@@?O@@?GCFCFCFCFCFCGC@CGCFCFCF@@@@?O@@?O@@@@@@CFCF?G@@?GCFCFCFAHAH?O@@?O@@@@@@CFCFCFCF?O@@@@@@@@@@?O@@?OAHAHAH@@@@@@@@?OCFCFCFCFCFCFCFCO@@@@@@AHAHAOAHAO@@@@@@@@@@AOAHAOAHAHAH@@@@@@@@COCFCFCFCFCFCFCF?GCFCFCFAHAH?O@@?OAHAHAHAHAHAHAH?H@@@@@@@@@@@@@@AOAHAHAH?O?O?O?O?O?O?O?O@@@@@@@@?O?O?O?O?@?@?@?@?@?@?@?@@O@O@O@O@O@O@O@O?O?O?O?O@@@@@@@@@@@@GF=L<H=LGF@@@@CLFFFLFFGLF@C@@@GNFFF@F@F@F@@@@@?NFLFLFLFLFF@@GNFFC@AHC@FFGN@@@@@@GN<H<L<LGH@@@@FFFFFFFFGKF@<@@@GF=LAHAHAH@N@@GNAHCLFFFFCLAHGNCLFFFFGNFFFFCL@@GL<F<F<FFLFL>N@@ANC@AHCLFFFFCL@@@@@@GF=K=KFN@@@@@F@LGN=K=KGNF@<@ANC@F@GNF@C@AN@@@@CLFFFFFFFFFF@@@@GN@@GN@@GN@@@@AHAHGNAHAH@@GN@@C@AH@LAHC@@@GN@@@LAHC@AH@L@@GN@@@NAKAKAHAHAHAHAHAHAHAHAHAH=H=HG@AHAH@@GN@@AHAH@@@@GB9L@@GB9L@@@@CHFLFLCH@@@@@@@@@@@@@@AHAH@@@@@@@@@@@@@@AH@@@@@@@G@D@DDDFDCDAL@LFLCFCFCFCF@@@@@@GH@LAHC@GL@@@@@@@@@@CLCLCLCL@@@@CLFF<C=K=K<CFFCL";

struct img{
	int sx=0,sy=0;
	short*data;
	img():sx(0),sy(0),data(0){}
	img(int x,int y):sx(x),sy(y){data=(short*)malloc(x*y*sizeof(short));}
	img(const char*filename){load(filename);}

	void load(const char*filename){
		printf("filename %s\n",filename);

	  char a;
	  FILE*f=fopen(filename,"r");
	  for(int i=0;i<3;i++)getc(f); // P6\n
	  while((a=getc(f))!=' ')sx=sx*10+a-48; // <X> 
	  while((a=getc(f))!=10)sy=sy*10+a-48; // <Y>\n

		printf("%s %i %i\n",filename,sx,sy);

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
			putc((data[i]&0xf00)>>4,f),
			putc((data[i]&0x0f0)   ,f),
			putc((data[i]&0x00f)<<4,f);
		fclose(f);
	}

	short operator[](int i){return data[i];}
};

struct Framework{
	int _sx,_sy,_ss; // width, height - in that order!
	short*newmap,*oldmap;
	char font[1<<11];
	clock_t timeframe,currentframe;
	bool(*userfunc)(Framework*);
	bool keypress; // XXX tells us whether a new key was pressed last frame - please figure out a better way to handle this
	char key; // what key was pressed
	struct{
		bool left,right;
		int x=0,y=0;
	}mouse;

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
	SDL_Event event;

	enum keymap{
		NONE=0,
		BACKSPACE=8,
		ENTER=13,
		ESC=27
	}; // XXX

	Framework(int x,int y,int s=1,unsigned char(*ostfunk)(int)=0):_sx(x),_sy(y),_ss(s?s:1),userfunc(0){ // XXX add argument for userfunc?
		newmap=(short*)malloc(x*y*sizeof(short));
		oldmap=(short*)malloc(x*y*sizeof(short));
		timeframe=clock();
		//FILE*f=fopen("font","r");for(int i=0;i<2048;i++)*(font+i)=getc(f);fclose(f);
		for(int i=0;i<2048;i++)font[i]=(fontdata[2*i]<<4)|fontdata[2*i+1]-'@';

		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
		SDL_CreateWindowAndRenderer(_sx*_ss,_sy*_ss,SDL_WINDOW_RESIZABLE,&window,&renderer);
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

	//void p(short c,int x,int y){newmap[y*_sx+x]=c;}

	void p(unsigned short c,int x,int y){
		if(x<0||y<0||x>_sx||y>_sy)return;

		short n=newmap[y*_sx+x];
		char a=c>>12,r,g,b;
		if(a==0xf)return;
		if(a==0){newmap[y*_sx+x]=c;return;} // XXX this is kinda useless

		r=((n&0xf00)>>8)*a + ((c&0xf00)>>8)*(0xf-a);
		g=((n&0x0f0)>>4)*a + ((c&0x0f0)>>4)*(0xf-a);
		b=((n&0x00f)   )*a + ((c&0x00f)   )*(0xf-a);

		r/=0xf,g/=0xf,b/=0xf;

		newmap[y*_sx+x]=(r<<8)|(g<<4)|b;

	}

	void pc(char c,int cx,int cy,short fgc=0xfff,short bgc=0){
		if(c)for(int x=0;x<8;x++)for(int y=0;y<8;y++)
		p(((char)font[(unsigned char)c*8+y]&(char)(128>>x))?fgc:bgc,cx*8+x,cy*8+y);
	}

	void ps(const char*c,int sx,int sy,short fgc=0xfff,short bgc=0){
		int x=0,y=0;
		for(int i=0;c[i];i++){
			pc(c[i]==10?0:c[i],sx+x++,sy+y,fgc,bgc);
			if(sx+x==_sx||c[i]==10)
				x=0,y++;
		}
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

	void pi(short*img,int cx,int cy,int sx,int sy){ // blip a whole image
		for(int x=0;x<sx;x++)
		for(int y=0;y<sy;y++)
		if(cx+x<_sx && cy+y<_sy)
		p(img[y*sx+x],cx+x,cy+y);
	}

	void pi(img a,int cx,int cy){pi(a.data,cx,cy,a.sx,a.sy);}

	void clear(){for(int i=0;i<_sx*_sy;i++)newmap[i]=0;}

	void snap(){
		FILE*f=fopen("snap.ppm","w");
		fprintf(f,"P6\n%i %i\n255\n",_sx,_sy);
		for(int i=0;i<_sx*_sy;i++)
			putc((newmap[i]&0xf00)>>4,f),
			putc((newmap[i]&0x0f0)   ,f),
			putc((newmap[i]&0x00f)<<4,f);
		fclose(f);
		printf("Screenshot taken.\n");
	}

	//bool operator[](char c){return SDL_GetKeyboardState(NULL)[c];}

	void run(){
		for(;;){
			clear(); // XXX

			// update
			if(userfunc&&!userfunc(this))return;

			// blit screenbuf to screen
			for(int x=0;x<_sx;x++)
			for(int y=0;y<_sy;y++)
			if(newmap[y*_sx+x]!=oldmap[y*_sx+x]){
				// calculate pixel color
				short color=oldmap[y*_sx+x]=newmap[y*_sx+x];
				int r = (color&0x0f00)>>4;
				int g = (color&0x00f0);
				int b = (color&0x000f)<<4;
				r=r|r>>4;g=g|g>>4;b=b|b>>4;

				SDL_SetRenderDrawColor(renderer,r,g,b,255);
				for(int scx=0;scx<_ss;scx++)
				for(int scy=0;scy<_ss;scy++)
				SDL_RenderDrawPoint(renderer,x*_ss+scx,y*_ss+scy);

			}

			// delay XXX this is gonna need more conversion
			currentframe=clock();
			//printf("%i %i\n",(currentframe-timeframe),CLOCKS_PER_SEC);
			//SDL_Delay((currentframe-timeframe)/CLOCKS_PER_SEC);

			SDL_Delay(20);
			//usleep(20);
			timeframe=currentframe;
			SDL_RenderPresent(renderer);

			mouse.left=mouse.right=key=0;

			// also, event.key.repeat tells us how many times it was repeated (we only want it if its once)
			while(keypress=SDL_PollEvent(&event)){ // returns 1 if it changed
				if(event.type==SDL_KEYDOWN){
					printf("k:%x <%c>\n",event.key.keysym.sym,event.key.keysym.sym&0xff); // 0xef?
					key=event.key.keysym.sym;
				}
				if(event.type==SDL_MOUSEBUTTONDOWN){
					if(event.button.button==SDL_BUTTON_LEFT)
						mouse.left=1;
					if(event.button.button==SDL_BUTTON_RIGHT)
						mouse.right=1;
				}
				if(event.type==SDL_MOUSEMOTION){
					mouse.x=event.motion.x/_ss;
					mouse.y=event.motion.y/_ss;
				}
				if(event.type==SDL_QUIT)return;
			}
		}
	}

};

/*
bool mouse_left_isheld(){return SDL_GetMouseState(0,0)&SDL_BUTTON(SDL_BUTTON_LEFT);}
SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]
*/
