typedef struct _vect{
	double x,y,z;
}vect_t;

typedef struct _point {
	int x,y;
}point;

typedef struct _camera{
	vect_t pos,view,dir;
}camera_t;

extern camera_t cam;

typedef struct _plane {
	vect_t p;
	vect_t n;	
} plane_t;

extern plane_t plane;

extern vect_t campos;

double v_dot(vect_t a,vect_t b);
vect_t v_scalar(vect_t v,double s);
vect_t v_add(vect_t a,vect_t b);
vect_t v_sub(vect_t a,vect_t b);
void print_vector(char *s,vect_t v);

void movecamera(vect_t v);

double v_len(vect_t v);
vect_t v_norm(vect_t v);


extern double cdist;
vect_t v_rot(vect_t v,vect_t r);

vect_t v_cross(vect_t a,vect_t b);
double v_len_squared(vect_t v);

void print_camera();
vect_t barycentric(vect_t t[],vect_t P);
