typedef struct _rgb {
	unsigned char r,g,b;
}rgb;

void draw_line(int x0,int y0,int x1,int y1);
void draw_triangle2(vect_t a,vect_t b,vect_t c,vect_t pos,rgb color);

extern rgb red,green,blue,black,white;

extern double z_buffer[]; 

void render_zbuffer();
void draw_triangle_gradient_test(point p[],rgb c[]);
vect_t barycentric_debug(vect_t tri[3],vect_t P);
void draw_line_color(int x0,int y0,int x1,int y1,rgb c);
