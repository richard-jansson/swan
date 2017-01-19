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
#include<stdint.h>

#define IDENT 0x49445032
typedef struct _md2_vertice {
	unsigned char v[3];
	unsigned char lightnormalindex;
}md2_vertex;

typedef struct _md2_tris {
	short vertexindex[3];
	short textureindex[3];
}md2_tris;

typedef struct _md2_header {
	int ident;
	int version;
	int skinwidth;
	int skinheight;
	int framesize;
	int num_skins;
	int num_xyz;
	int num_st;
	int num_tris;
	int num_glcmds;
	int num_frames;
	int ofs_skins;
	int ofs_st;
	int ofs_tris;
	int ofs_frames;
	int ofs_glcmds;
	int ofs_end;
} md2_header;

typedef struct _md2_frame_header {
	float scale[3];
	float translate[3];
	char name[16];
} md2_frame_header;

void md2_openfile(char *path);
