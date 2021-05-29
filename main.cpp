#include <stdio.h>
#include "lib.h"

// i used the doom logo during testing, but i cant put that on github. format your image as ppm and youre good to go
//img doom=loadimg("doom.ppm");

int i=0;

int main(){
	Framework window=Framework(640,480,2);

	window.userfunc=[](Framework*window){
		if(window->key==0x1b)return false;

		//window->pi(doom,10,10);

		for(int x=0;x<640/8;x++){
			window->pc('@',x,0,0xfff,0);
			window->pc('@',x,480/8-1,0xfff,0);
		}

		for(int y=0;y<480/8;y++){
			window->pc('@',0,y,0xfff,0);
			window->pc('@',640/8-1,y,0xfff,0);
		}


		for(int x=0;x<256;x++)
		for(int y=0;y<256;y++)
		window->p((x^y)+i,x+640/2-128,y+480/2-128);
		i++;

		window->ps(" all your bases \nare belong to us",640/16-8,480/8-8,0xf00,0x400);

		return true;
	};

	window.run();
	return 0;
}
