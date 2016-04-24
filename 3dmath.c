#include<stdio.h>
#include<math.h>

#include "3dmath.h"
#include "swan.h"

#define QUICK 0

// This really makes no difference! 
#define BARY_INLINE 0

/* Finally a barycentric that works ! */
/*
http://math.stackexchange.com/questions/51326/determining-if-an-arbitrary-point-lies-inside-a-triangle-defined-by-three-points
*/

vect_t barycentric(vect_t t[],vect_t P){
#if 1
	vect_t u=v_sub(t[1],t[0]);
	vect_t v=v_sub(t[2],t[0]);
	vect_t p=v_sub(P,t[0]);
#else
	vect_t u=(vect_t){t[1].x-t[0].x,t[1].y-t[0].y,t[1].z-t[0].z};
	vect_t v=(vect_t){t[2].x-t[0].x,t[2].y-t[0].y,t[2].z-t[0].z};
	vect_t p=(vect_t){P.x-t[0].x,P.y-t[0].y,P.z-t[0].z};
#endif 

	vect_t w;
// TODO: optimization take calculation of d and move it into triangle function
// all that is only u and v we don't have to calculate again 
	double d=u.x*v.y-v.x*u.y;
	w.x=(p.x*(u.y-v.y)+p.y*(v.x-u.x)+u.x*v.y-v.x*u.y)/d;
	w.y=(p.x*v.y-p.y*v.x)/d;
	w.z=(p.y*u.x-p.x*u.y)/d;

#if 0 
	double d=u.x*v.y-v.x*u.y; // outside
	a=(u.y-v.y);
	b=(v.x-u.x);
	c=(u.x*c.y-v.x*u.y);
	// inside 
	w.x=(p.x*a+p.y*b+c)/d;

	w.y=(p.x*v.y-p.y*v.x)/d;
	w.z=(p.y*u.x-p.x*u.y)/d;
#endif

	return w;
}
/*
 * The camera's position is in origio
 */ 

camera_t cam = {{0,0,-1000},// pos
	{0,0,-10}, // view
	{0,0,0}}; // dir // dir

plane_t plane={{0,0,0},{0,0,-1}};
vect_t tri[3]={{0,1,5},{-1,0,5},{1,0,5}};

vect_t campos={0,0,0};

double cdist=500;
// ordinary translation
void movecamera(vect_t v){
	cam.pos=v_add(cam.pos,v);
// Old code	
	plane.p=v_add(plane.p,v);
	campos=v_add(plane.p, v_scalar(v_norm(plane.n),-cdist));
/*	vect_t right=v_rotate(plane.n,(vect_t){0,M_PI/2,0,0});
//	plane.p=v_add(plane.p,v);

	plane.p=v_add(plane.p, v_scalar(plane.n,v.z));
	plane.p=v_add(plane.p, v_scalar(right,v.x));

	if(plane.p.z > -100 ) plane.p.z=-100;
	campos=v_add(plane.p, v_scalar(plane.n,-cdist));
	*/

}
vect_t v_inv(vect_t v){
	return (vect_t){-v.x,-v.y,-v.z};
}

vect_t v_rot(vect_t v,vect_t r){
	float s,c;
// Around x
	s=sin(r.x);
	c=cos(r.x);
	v.x=v.x;
	v.y=c*v.y - s*v.z;
	v.z=s*v.y + c*v.z;

// around y 
	s=sin(r.y);
	c=cos(r.y);
	v.x=c*v.x+s*v.z;
	v.y=v.y;
	v.z=v.z-s*v.x;

// around z
	s=sin(r.z);
	c=cos(r.y);
	v.x=v.x*c-s*v.y;
	v.y=v.x*s-c*v.y;
	v.z=v.z;

	return v;
}
void rotatecamera(vect_t rot){
/*j	plane.n=v_rotate(plane.n,rot);
	campos=v_add(plane.p, v_scalar(plane.n,-cdist));
	*/
	cam.dir=v_add(cam.dir,rot);
}
double v_dot2d(vect_t a,vect_t b){
	return a.x*b.x + a.y*b.y;
}
double v_dot(vect_t a,vect_t b){
	return a.x*b.x+
		a.y*b.y+
		a.z*b.z;
}

vect_t v_scalar(vect_t v,double s){
	return (vect_t){v.x*s,v.y*s,v.z*s};
}

vect_t v_sub(vect_t a,vect_t b){
	return (vect_t){a.x-b.x,a.y-b.y,a.z-b.z};
}
vect_t v_add(vect_t a,vect_t b){
	return (vect_t){a.x+b.x,a.y+b.y,a.z+b.z};
}

void print_vector(char *s,vect_t v){
	if(s) printf("%s:\t",s);
	printf("(%f,%f,%f)\n",v.x,v.y,v.z);
}

void print_camera(){
	printf(
	"(%f,%f,%f) (%f,%f,%f) (%f,%f,%f)\n",
	cam.pos.x,
	cam.pos.y,
	cam.pos.z,
	cam.view.x,
	cam.view.y,
	cam.view.z,
	cam.dir.x,
	cam.dir.y,
	cam.dir.z);
}
/*0 
 * Rewrite using https://en.wikipedia.org/wiki/3D_projection
 */
void project_coord(vect_t v,int *x,int *y){
	// subtract camera
	vect_t t=v_sub(v,cam.pos);	
	// rotate according to camera
	// really we should inv
	vect_t d=v_rot(t,v_inv(cam.dir));

	double xx=d.x*cam.view.z/d.z - cam.view.x;
	double yy=d.y*cam.view.z/d.z - cam.view.y;


	*x=xx;
	*y=yy;
#if 1 
	*x=(xx)*HW+HW;
	*y=(yy)*HH+HH;
#endif  

#if 0
	printf("*****\n");
	print_vector("v",v);
	print_vector("t",t);
	print_vector("d",d);
	printf("(%f,%f)\n",xx,yy);
	printf("(%i,%i)\n",*x,*y);
#endif 
}


/*int project_coord_old(vect_t v,int *sx,int *sy){
	if( v_dot(v,plane.n) > 0) return 0;

	vect_t l=v_sub(v,campos); 
	vect_t a=v_sub(plane.n,campos);
	
	double x=v_dot(a,plane.n);
	double y=v_dot(l,plane.n);
		
	double z=x/y;
#ifndef QUICK
	printf("y=%f\n",y);
#endif
	if( z < 0 ) return 0;
	if( y < 0 && x < 0) return 0;
//	vect_t pos=v_add(campos,v_scalar(l,z));
	vect_t pos=v_scalar(l,z);

	vect_t t1=v_sub(v,campos);
	vect_t t2=v_sub(pos,campos);
	vect_t t3=v_sub(plane.p,campos);
	double d1=v_len(t1);
	double d2=v_len(t2);
	double d3=v_len(t3);

//	printf("%f %f %s\n",d1,d2,d2<d1?" OK ":" !OK ");

	double a1=v_dot(t1,t3);
	double a2=v_dot(t1,t2);
	double a3=v_dot(t2,t3);
//	printf("%f %f %f\n",a1,a2,a3);
//	printf("%f %f\n",x,y);
//	if( x > 0 || y > 0) return 0;
//	printf("%f %f\n",x,y);
//	printf("%f %f\n",d1,d3);
//	printf("%s\n",v_dot(t1,t2)?"positive":"negative");
	vect_t i=v_sub(v,plane.p);
	vect_t j=v_sub(campos,plane.p);
//	printf("%f %f\n",d1,d2);
//	if( z < 0 ) return 0;

#ifndef QUICK
	printf("*** project_coord:\n");
	print_vector("v",v);
	print_vector("a",a);
	print_vector("p",plane.p);
	print_vector("n",plane.n);
	printf("%f = %f / %f\n",z,x,y);
	print_vector("pos",pos);
#endif

	*sx=(int) pos.x+HW;
	*sy=(int) pos.y+HH;
	return 1;
}*/
double v_len(vect_t v){
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

vect_t v_norm(vect_t v){
	double l=v_len(v);
	vect_t r;
	r.x=v.x/l;
	r.y=v.y/l;
	r.z=v.z/l;
	return r;
}
vect_t v_cross(vect_t a,vect_t b){
	return (vect_t){
		a.y*b.z-a.z*b.y,
		a.z*b.x-a.x*b.z,
		a.x*a.y-a.y*b.x};
}
double v_len_squared(vect_t v){
	double l=v.x*v.x+v.y*v.y+v.z*v.z;
	return l>0.0? l : -l;
}
