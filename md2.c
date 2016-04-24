#include<stdio.h>
#include<string.h>
#include<assert.h>

#include"3dmath.h"

#include<stdlib.h>

// these three are needed for fstat
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include "primitives.h"
#include "md2.h"

md2_header *header;	
md2_tris *tris;
char *buffer;

void print_frame_head(md2_frame_header *h){
	printf("frame %s scale=%f translate=%f\n",h->name,h->scale,h->translate);
}

void print_vertex(md2_vertex v){
	printf("(%i,%i,%i) i=%i\n",v.v[0],v.v[1],v.v[2],v.lightnormalindex);
}


// If we scale the vectors in before hand we will save quite a lot of processing time
void scale_vector(int *ret,unsigned char *v,float *scale,float *trans){
	for(int i=0;i<3;i++){
//j		ret[i]=(float)v[i]*scale[i]+trans[i];
		ret[i]=(float)v[i]*scale[i]+trans[i];
//		ret[i]*=10.0;
//			ret[i]=v[i]*scale[0]*50;
	}
}

//void renderframe(md2_frame_header *head,md2_vertex *vertices,md2_tris *tris,int num,int frame_num){
void renderframe(int frame_num,vect_t pos,vect_t rot,rgb color){
	int num=header->num_tris;
	md2_frame_header *framehead;
	char name[256]; *name='\0';
	char *p=buffer+header->ofs_frames +frame_num*header->framesize;
	char *f=p+sizeof(md2_frame_header);
	md2_vertex *vertices=(md2_vertex *)f;
	framehead=(md2_frame_header*)p;

//	print_frame_head(framehead);
	
	int a[3],b[3],c[3];
	vect_t t[3];
	srandom(0);
	for(int i=0;i<num;i++){
			scale_vector(a,vertices[tris[i].vertexindex[0]].v,framehead->scale,framehead->translate);
			scale_vector(b,vertices[tris[i].vertexindex[1]].v,framehead->scale,framehead->translate);
			scale_vector(c,vertices[tris[i].vertexindex[2]].v,framehead->scale,framehead->translate);

			int r=random()%3;
			switch(r){
				case 0:
					color=red;
					break;
				case 1:
					color=green;
					break;
				case 2:
					color=blue;
					break;
			}
			draw_triangle3(a,b,c,pos,rot,color);
			/*
			t[0].x=a[0]; t[0].y=a[1]; t[0].z=a[2];
			t[1].x=b[0]; t[1].y=b[1]; t[1].z=b[2];
			t[2].x=c[0]; t[2].y=c[1]; t[2].z=c[2];

			g[0].x=a[0]; g[0].y=a[1]; g[0].z=0;
			g[1].x=b[0]; g[1].y=b[1]; g[1].z=0;
			g[2].x=c[0]; g[2].y=c[1]; g[2].z=0;
			*/
	}
}

void md2_openfile(char *path){
// Read the header 
	FILE *fd=fopen(path,"r");
	struct stat st;

	stat(path,&st);
	buffer=malloc(st.st_size);

	assert(fd);
	
	fread(buffer,st.st_size,1,fd);

	header=(md2_header*)buffer;
	char *f=buffer+header->ofs_tris;
	tris=(md2_tris*)f;
	
	printf("num frames = %i\n",header->num_frames);
	printf("num tris = %i\n",header->num_tris);
	printf("num vertices = %i\n",header->num_xyz);

/*	for(int i=0;i<header->num_frames;i++){
		renderframe((md2_frame_header*)p,
			(md2_vertex*)(p+sizeof(md2_frame_header)),
			tris,header->num_tris,i+1);	
		p+=header->framesize;
	}*/

//	free(buffer);
//	fclose(fd);	
}

