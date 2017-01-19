/*
 *	Copyright (C) 2016 Richard Jansson 
 *	
 *	This file is part of Veta.
 *	
 *	Veta is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	Veta is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with Veta.  If not, see <http://www.gnu.org/licenses/>.
*/
#include<SDL/SDL.h>
#include<math.h>

#include"swan.h"
#include "3dmath.h"
#include "primitives.h"

SDL_Surface *screen;
SDL_Event ev;

#define SPEED 20
#define ZOOM_SPEED 100
#define N_X 100
#define N_Y 100
#define NORMLENGTH 80
#define PANSPEED 1.0


int point_in_triangle(vect_t t[],vect_t P){
}

int sameside(vect_t p1,vect_t p2,vect_t a,vect_t b){
	vect_t c1=v_cross(v_sub(b,a),v_sub(p1,a));
	vect_t c2=v_cross(v_sub(b,a),v_sub(p2,a));
	return v_dot(c1,c2) > 0.0;
}

void putpixel_c(int x,int y,rgb c){
	unsigned char *pixels=(unsigned char *)screen->pixels;
	int o=(WIDTH*y+x)*3;
	
	if( y < 0 || y > HEIGHT) return;
	pixels[o]=c.b;
	pixels[o+1]=c.g;
	pixels[o+2]=c.r;
}
void putpixel(int x,int y){
	unsigned char *pixels=(unsigned char *)screen->pixels;
	int o=(WIDTH*y+x)*3;
	if( x < 0 || x > WIDTH) return;
	if( y < 0 || y > HEIGHT) return;
	pixels[o]=0;
	pixels[o+1]=255;
	pixels[o+2]=0;
}

int main(){
	vect_t P={0,0,0};
	SDL_Init(SDL_INIT_VIDEO);


//	md2_openfile("shotgun.md2");
	md2_openfile("player.md2");

	screen=SDL_SetVideoMode(WIDTH,HEIGHT,24,0);
	int running=1;
	int frame=0;
	vect_t dir={0,0,0};
	vect_t rdir={0,0,0};

	double cdistchange=0;
	movecamera(dir);
	rotatecamera(rdir);
	int draw_zbuffer=0;
	int pause=0;
	init_zbuffer();
	while(running){
//		SL_Delay(200);
		if(0 == (frame%10)){
			printf("fps=%2.2f\n",1000*frame/(float)SDL_GetTicks());
			print_camera();
		}

//		if( frame == MAX_FRAME ) running=0;

		if(!pause) frame++;

		SDL_FillRect(screen,NULL,0xffffff);

		clear_zbuffer();

#if 1 
		renderframe((frame/3)%100,(vect_t){0,0,0},(vect_t){frame*M_PI/200.0,0,0},red);
		if(draw_zbuffer){
			render_zbuffer();
		}
#endif 

		SDL_UpdateRect(screen,0,0,0,0);

		movecamera(dir);
		rotatecamera(rdir);
		cdist+=cdistchange;
		if(cdist < 0 ) cdist = 0;

		while(SDL_PollEvent(&ev)){
		switch(ev.type){
			case SDL_MOUSEMOTION:
				P.x=ev.motion.x;
				P.y=ev.motion.y;

				break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym){
						case SDLK_UP:
							rdir.x= M_PI*PANSPEED/180;
							break;
						case SDLK_DOWN:
							rdir.x=- M_PI*PANSPEED/180;
							break;
						case SDLK_LEFT:
							rdir.y= M_PI*PANSPEED/180;
							break;
						case SDLK_RIGHT:
							rdir.y=- M_PI*PANSPEED/180;
							break;
						case SDLK_q:
							running=0;
							break;
						/*
						case SDLK_a:
							dir.x=-10.0;
							break;
						case SDLK_d:
							dir.x=10;
							break;
						case SDLK_w:
							dir.z=10;
							break;
						case SDLK_s:
							dir.z=-10;
							break;
							*/
						case SDLK_r:
							cdistchange=ZOOM_SPEED;
							break;
						case SDLK_f:
							cdistchange=-ZOOM_SPEED;
							break;
						// t,g,y,h,u,j x,y,z up and down
						case SDLK_t:
							dir.x=SPEED;
							break;
						case SDLK_g:
							dir.x=-SPEED;
							break;
						case SDLK_y:
							dir.y=SPEED;
							break;
						case SDLK_h:
							dir.y=-SPEED;
							break;
						case SDLK_u:
							dir.z=SPEED;
							break;
						case SDLK_j:
							dir.z=-SPEED;
							break;
						case SDLK_z:
							draw_zbuffer=1;
							break;
						case SDLK_p:
							pause=1;
							break;
					}
					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym){
						case SDLK_z:
							draw_zbuffer=0;
							break;
						case SDLK_u:
						case SDLK_j:
							dir.z=0;
							break;
						case SDLK_y:
						case SDLK_h:
							dir.y=0;
							break;
						case SDLK_t:
						case SDLK_g:
							dir.x=0;
							break;
						case SDLK_UP:
						case SDLK_DOWN:
							rdir.x=0.0;
							break;
						case SDLK_LEFT:
						case SDLK_RIGHT:
							rdir.y=0.0;
							break;
						case SDLK_a:
						case SDLK_d:
							dir.x=0.0;
							break;
						case SDLK_w:
						case SDLK_s:
							dir.z=0.0;
						break;
						case SDLK_f:
						case SDLK_r:
							cdistchange=0;
							break;
						case SDLK_p:
							pause=0;
							break;
					}
					break;
		}
		}
	}
	SDL_Quit();
}
