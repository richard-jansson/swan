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
