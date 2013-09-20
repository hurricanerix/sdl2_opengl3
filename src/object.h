#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef struct Vertex {
  float x,y,z;             /* the usual 3-space position of a vertex */
} Vertex;

typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face;

//extern GLuint vao[3];
//extern GLuint vao[3];
//extern int triangle_count;
//extern GLuint faces[];

void setupBuffers(char *file_name); // rename to create_object
void render_object();
void destroy_object();

#endif//__OBJECT_H__
