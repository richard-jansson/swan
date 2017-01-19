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
