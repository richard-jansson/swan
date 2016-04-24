#include <float.h>

#include "swan.h"
#include "3dmath.h"
#include"primitives.h"

double z_buffer[WIDTH*HEIGHT],zeroed_buffer[WIDTH*HEIGHT];

rgb red=(rgb){255,0,0};
rgb green=(rgb){0,255,0};
rgb blue=(rgb){0,0,255};
rgb black=(rgb){0,0,0};
rgb white=(rgb){ 255, 255, 255};


#define MAX_DIST 5000.0
float clipfloat(float f,float min,float max){
	if( f < min ) f = 0.0;
	if( f > max ) f = max;
	return f;
}

void render_zbuffer(){
	double max=-DBL_MAX,min=DBL_MAX;
	for(int i=0;i<HEIGHT*WIDTH;i++){
		if(z_buffer[i]==DBL_MAX) continue;
		if(z_buffer[i]>max) max=z_buffer[i];
		if(z_buffer[i]<min) min=z_buffer[i];
	}
	printf("[%f,%f]\n",min,max);
	for(int y=0;y<HEIGHT;y++){
		for(int x=0;x<WIDTH;x++){
			double v=z_buffer[y*WIDTH+x];
			rgb c;
			if( v == DBL_MAX) c = red;
			else{
				v=255*(v-min)/(max-min);
//				printf("%f\n",v);
//				c.g=v > 255 ? 255 : v;
				c.b=c.r=c.g= c.g=v > 255 ? 255 : v;
			}
			putpixel_c(x,y,c);
		}
	}
}

// Doing this instead of running a loop increased frame rate from 16 fps to 22 fps! 
// that is memcpying the correct buffer instead of looping trough it!
void init_zbuffer(){
	for(int i=0;i<WIDTH*HEIGHT;i++) zeroed_buffer[i]=DBL_MAX;
}
void clear_zbuffer(){
	mempcpy(z_buffer,zeroed_buffer,sizeof(double)*WIDTH*HEIGHT);
}

int max(int a,int b){
	if(a>b) return a;
	return b;
}
int min(int a,int b){
	if(a<b) return a;
	return b;
}

void draw_line_color(int x0,int y0,int x1,int y1,rgb c){
	int xstart=min(x0,x1),xend=max(x0,x1);
	int ystart=min(y0,y1),yend=max(y0,y1);

	if( x1 == x0 ){
		for(int y=max(ystart,0);y<min(yend,HEIGHT);y++) putpixel_c(x0,y,c);
		return;
	}

	if( y1 == y0 ){
		for(int x=max(xstart,0);x<min(xend,WIDTH);x++) putpixel_c(x,y0,c);
		return;
	}
	
	double k=(y1-y0)/(float)(x1-x0); 
	int d=y1-k*x1;

	if( k < 1.0 && k > -1.0 ){
		for(int x=max(xstart,0);x<min(xend,WIDTH);x++){
			int y = k*x + d;
			putpixel_c(x,y,c);
		}
	}else{
		for(int y=max(ystart,0);y<min(yend,HEIGHT);y++){
			int x = (y-d)/k;
			putpixel_c(x,y,c);
		}
	}
}
void draw_line(int x0,int y0,int x1,int y1){
	int xstart=min(x0,x1),xend=max(x0,x1);
	int ystart=min(y0,y1),yend=max(y0,y1);

	if( x1 == x0 ){
		for(int y=max(ystart,0);y<min(yend,HEIGHT);y++) putpixel(x0,y);
		return;
	}

	if( y1 == y0 ){
		for(int x=max(xstart,0);x<min(xend,WIDTH);x++) putpixel(x,y0);
		return;
	}
	
	double k=(y1-y0)/(float)(x1-x0); 
	int d=y1-k*x1;

	if( k < 1.0 && k > -1.0 ){
		for(int x=max(xstart,0);x<min(xend,WIDTH);x++){
			int y = k*x + d;
			putpixel(x,y);
		}
	}else{
		for(int y=max(ystart,0);y<min(yend,HEIGHT);y++){
			int x = (y-d)/k;
			putpixel(x,y);
		}
	}
}
#define A 1 
#define B 2

//#define Z 1000.0
#define Z 0

void draw_triangle(int *a,int *b,int *c,vect_t pos,rgb color){
	int x0,y0,x1,y1,x2,y2;
	vect_t x={a[0],a[1],a[2]+Z};
	vect_t y={b[0],b[1],b[2]+Z};
	vect_t z={c[0],c[1],c[2]+Z};

	x=v_add(x,pos);
	y=v_add(y,pos);
	z=v_add(z,pos);

	int q,w,e;
	q=project_coord(x,&x0,&y0);
	w=project_coord(y,&x1,&y1);
	e=project_coord(z,&x2,&y2);

//	if( !(q && w && e)) return;

	draw_line_color(x0,y0,x1,y1,color);
	draw_line_color(x1,y1,x2,y2,color);
	draw_line_color(x2,y2,x0,y0,color);
}

void draw_triangle2(vect_t a,vect_t b,vect_t c,vect_t pos,rgb color){
	int x0,y0,x1,y1,x2,y2;
	
	a=v_add(a,pos);
	b=v_add(b,pos);
	c=v_add(c,pos);

	project_coord(a,&x0,&y0);
	project_coord(b,&x1,&y1);
	project_coord(c,&x2,&y2);

	draw_line_color(x0,y0,x1,y1,color);
	draw_line_color(x1,y1,x2,y2,color);
	draw_line_color(x2,y2,x0,y0,color);
}


void draw_triangle_gradient_new(point p[],vect_t zv[],double zs[],rgb c[]){
	double minx=DBL_MAX,miny=DBL_MAX,maxx=-DBL_MAX,maxy=-DBL_MAX;	
	for(int i=0;i<3;i++){
		if( p[i].x < minx) minx=p[i].x;
		if( p[i].y < miny) miny=p[i].y;
		if( p[i].x > maxx) maxx=p[i].x;
		if( p[i].y > maxy) maxy=p[i].y;
	}
	vect_t t[3];
//	printf("%f %f %f %f\n",minx,miny,maxx,maxy);
	for(int i=0;i<3;i++){
		t[i].x=p[i].x;
		t[i].y=p[i].y;
		t[i].z=0;
	}

#if BARY_INLINE
	// barycentric part one! 
	vect_t u=v_sub(t[1],t[0]);
	vect_t v=v_sub(t[2],t[0]);
	double d=u.x*v.y-v.x*u.y,a,bb,cc;
	a=(u.y-v.y);
	bb=(v.x-u.x);
	cc=(u.x*v.y-v.x*u.y);
#endif 
	for(int y=miny;y<maxy;y++){
		for(int x=minx;x<maxx;x++){
#if BARY_INLINE
			vect_t pt=v_sub((vect_t){(double)x,(double)y,0},t[0]);
			vect_t b;
			b.x=(pt.x*a+pt.y*bb+cc)/d;
			b.y=(pt.x*v.y-pt.y*v.x)/d;
			b.z=(pt.y*u.x-pt.x*u.y)/d;
#else
			vect_t b=barycentric(t,(vect_t){(double)x,(double)y,0.0});
#endif

// Fun effect if you put this at 0 
#if 1 
			if(b.x < 0 || b.y < 0 || b.z < 0) continue;
			if(b.x > 1.0 || b.y > 1.0 || b.z > 1.0 ) continue;
#else 
		putpixel_c(x,y,c[0]);
		continue;
#endif 
			double z=b.x*zs[0] + b.y*zs[1] + b.z*zs[2];
			rgb cout;
			cout.r=b.x*c[0].r + b.y*c[1].r + b.z*c[2].r;
			cout.g=b.x*c[0].g + b.y*c[1].g + b.z*c[2].g;
			cout.b=b.x*c[0].b + b.y*c[1].b + b.z*c[2].b;
			if(z_buffer[y*WIDTH+x] > z ){ 
				putpixel_c(x,y,cout);
				z_buffer[y*WIDTH+x]=z;
			}
//			putpixel_c(x,y,c[0]);
		}
	}
}

void draw_triangle_gradient(point p[],vect_t zv[],double zs[],rgb c[]){
	int min_i=0,max_i=0,mid_i=-1;
//	for(int i=0;i<3;i++) print_point(p[i]);
	for(int i=1;i<3;i++){
		if( p[i].x < p[min_i].x ) min_i=i;
		if( p[i].x > p[max_i].x ) max_i=i;
	}
/*	printf("min: ");
	print_point(p[min_i]);
	printf("max: ");
	print_point(p[max_i]);
	*/

	for(int i=0;i<3;i++) if( i != min_i && i != max_i ) mid_i = i;

	int dx0=p[min_i].x-p[max_i].x;
	int dy0=p[min_i].y-p[max_i].y;
	double k0=dy0/(float)dx0;
	int d0=p[min_i].y-k0*p[min_i].x;
	
	int dx1=p[min_i].x-p[mid_i].x;
	int dy1=p[min_i].y-p[mid_i].y;
	double k1=dy1/(float)dx1;
	int d1=p[min_i].y-k1*p[min_i].x;

	int dx2=p[mid_i].x-p[max_i].x;
	int dy2=p[mid_i].y-p[max_i].y;
	double k2=dy2/(float)dx2;
	int d2=p[mid_i].y-k2*p[mid_i].x;

	for(int x=p[min_i].x ;x<p[mid_i].x;x++){
		int y0 = k0*x + d0;
		int y1 = k1*x + d1;
		for(int y=min(y0,y1);y<=max(y0,y1);y++){
			point pc={x,y};
//			color cc=_tri_calc_color(p,pc,c);
			vect_t b=barycentric(zv,(vect_t){x,y,0});
			double z=b.x*zs[0] + b.y*zs[1] + b.z*zs[2];
			if( x < 0 || x > WIDTH) continue;
			if( y < 0 || y > HEIGHT) continue;
//			printf("%f\n",z);
			if(z_buffer[y*WIDTH+x] > z ){ 
				putpixel_c(x,y,c[0]);
				z_buffer[y*WIDTH+x]=z;
			}
		}
	}

	for(int x=p[mid_i].x ;x<p[max_i].x;x++){
		int y0 = k0*x + d0;
		int y2 = k2*x + d2;
		for(int y=min(y0,y2);y<=max(y0,y2);y++){
			point pc={x,y};
//			color cc=_tri_calc_color(p,pc,c);
//			putpixel_c(x,y,c[0]);
			vect_t b=barycentric(zv,(vect_t){x,y,0});
//j			double z=zv[0].z/b.x + zv[1].z/b.y + zv[2].z/b.z;
			double z = b.x*zs[0] + b.y*zs[1] + b.z*zs[2]; 
			if( x < 0 || x > WIDTH) continue;
			if( y < 0 || y > HEIGHT) continue;
			if(z_buffer[y*WIDTH+x] > z){ 
				putpixel_c(x,y,c[0]);
				z_buffer[y*WIDTH+x]=z;
			}
		}
	}
}

int outside_of_screen(point p[]){
	int c=0;
	for(int i=0;i<3;i++){
		c+= p[i].x < 0 || p[i].x > WIDTH;
		c+= p[i].y < 0 || p[i].y > HEIGHT;
	}
	return c==3;
}

int clip(int a){
	if(a>255) return 255;
	if( a < 0 ) return 0;
}
/* Third version of draw triangle uses a z_buffer
	And we're using flat shading 
	clear_z_buffer needs to be run in the main loop before every frame. */
void draw_triangle3(int *a,int *b,int *c,vect_t pos,vect_t rot,rgb color){
	int x0,y0,x1,y1,x2,y2;
	// this is is the format that the triangles should be passed to us already 
	vect_t x={a[0],a[1],a[2]+Z};
	vect_t y={b[0],b[1],b[2]+Z};
	vect_t z={c[0],c[1],c[2]+Z};

// Lighting	
	vect_t n=v_norm(v_cross(x,y));
//	vect_t l=v_sub(cam.pos,x);
	vect_t l=cam.pos;
	double lum=1.0;

//	if( lum < 0 ) return 0; 
	rgb colors[3];



	point p[3];

	x=v_rot(x,rot);
	y=v_rot(y,rot);
	z=v_rot(z,rot);
// should we do the translation here as well does this make sense?  
	x=v_add(x,pos);
	y=v_add(y,pos);
	z=v_add(z,pos);

	double lums[3];
	lums[0]=v_dot(l,x)/(v_len(l)*v_len(x));
	lums[1]=v_dot(l,y)/(v_len(l)*v_len(y));
	lums[2]=v_dot(l,z)/(v_len(l)*v_len(z));

#if 1
	for(int i=0;i<3;i++){
	#if 0 
		lum=lums[i];
		if( lum < 0 ) lum=0;
	#else 
		lum=1.0;
	#endif 

		colors[i].r=clip(color.r*lum);
		colors[i].g=clip(color.g*lum);
		colors[i].b=clip(color.b*lum);
	}
#else 
	for(int i=0;i<3;i++){
		colors[i].r=color.r;
		colors[i].g=color.g;
		colors[i].b=color.b;
	}
#endif 


	int q,w,e;
	q=project_coord(x,&p[0].x,&p[0].y);
	w=project_coord(y,&p[1].x,&p[1].y);
	e=project_coord(z,&p[2].x,&p[2].y);

	if(outside_of_screen(p)){
		return;
	}

	rgb ca[3];
	ca[0]=ca[1]=ca[2]=color;	

	vect_t v3[3];
	v3[0]=x; v3[1]=y; v3[2]=z;

	double distances[3];
	
	// introduce max distance of 5000 
	distances[0]=clipfloat(v_len(v_sub(x,cam.pos)),0,MAX_DIST)/MAX_DIST;	
	distances[1]=clipfloat(v_len(v_sub(y,cam.pos)),0,MAX_DIST)/MAX_DIST;	
	distances[2]=clipfloat(v_len(v_sub(z,cam.pos)),0,MAX_DIST)/MAX_DIST;	

/*	
	distances[0]=v_len(v_sub(z,cam.pos));	
	distances[1]=v_len(v_sub(y,cam.pos));	
	distances[2]=v_len(v_sub(x,cam.pos));	
		*/


	draw_triangle_gradient_new(p,v3,distances,colors);	
#if 0
	draw_line_color(p[0].x,p[0].y,p[1].x,p[1].y,black);
	draw_line_color(p[1].x,p[1].y,p[2].x,p[2].y,black);
	draw_line_color(p[1].x,p[1].y,p[0].x,p[0].y,black);
#endif 
}
