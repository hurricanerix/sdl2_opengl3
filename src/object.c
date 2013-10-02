/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Richard Hawkins
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif

#include "logger.h"
#include "3dmath.h"
#include "plyfile.h"
#include "shader.h"
#include "object.h"
#include "bmp.h"

unsigned char *bmp_data;
int bmp_width;
int bmp_height;


PlyProperty vert_props[] = { /* list of property information for a vertex */
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
  {"nx", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nx), 0, 0, 0, 0},
  {"ny", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,ny), 0, 0, 0, 0},
  {"nz", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nz), 0, 0, 0, 0},
  {"s", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,u), 0, 0, 0, 0},
  {"t", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,v), 0, 0, 0, 0},
};
PlyProperty face_props[] = { /* list of property information for a vertex */
  {"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};


GLuint vao[3];

float *vertices;
float *normals;
float *tangents;
float *tex_coords;
GLuint *faces;

GLuint vertex_count = -1;
GLuint tex_count = -1;
GLuint face_count = -1;

GLuint buffers[5];

void read_object(char *file_name);

void setup_texture(TextureConfig *config)
{
    assert(config != NULL);

    unsigned char *bmp_data;
    int bmp_width;
    int bmp_height;
    GLuint textureID;

    load_bmp(config->bmp_file, &bmp_width, &bmp_height, &bmp_data);

    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp_width, bmp_height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, bmp_data);

    free(bmp_data);
}


void setupBuffers(char *file_name)
{
    assert(file_name != NULL);

    read_object(file_name);
    assert(vertex_count != -1);
    assert(face_count != -1);

    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    // bind buffer for vertex and copy data into buffer
    glGenBuffers(1, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_count, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);

    // bind buffer for normal and copy data into buffer
    glGenBuffers(1, &buffers[1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals) * vertex_count, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalLoc);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);

    // bind buffer for tanget and copy data into buffer
    glGenBuffers(1, &buffers[2]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tangents) * vertex_count, tangents, GL_STATIC_DRAW);
    glEnableVertexAttribArray(tangentLoc);
    glVertexAttribPointer(tangentLoc, 3, GL_FLOAT, 0, 0, 0);

    // bind buffer for tex coords and copy data into buffer
    glGenBuffers(1, &buffers[3]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords) * tex_count, tex_coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texLoc);
    glVertexAttribPointer(texLoc, 3, GL_FLOAT, 0, 0, 0);

    glGenBuffers(1, &buffers[4]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * face_count, faces, GL_STATIC_DRAW);
}

void render_object()
{
    glDrawElements(GL_TRIANGLES, face_count, GL_UNSIGNED_INT, NULL);
}

void destroy_object()
{
    glDisableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &buffers[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &buffers[1]);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, vao);

    free(vertices);
    free(normals);
    free(tangents);
    free(tex_coords);
    free(faces);
    face_count = -1;
    vertex_count = -1;
    tex_count = -1;
}

void read_object(char *file_name)
{
    assert(file_name != NULL);

    int i,j,k;
    PlyFile *ply;
    int nelems;
    char **elist;
    int file_type;
    float version;
    int nprops;
    int num_elems;
    PlyProperty **plist;
    Vertex **vlist;
    Face **flist;
    char *elem_name;
    int num_comments;
    char **comments;
    int num_obj_info;
    char **obj_info;

    // open a PLY file for reading 
    ply = ply_open_for_reading(file_name, &nelems, &elist, &file_type, &version);

    // print what we found out about the file 

    // go through each kind of element that we learned is in the file 
    // and read them 

    for (i = 0; i < nelems; i++) {
        // get the description of the first element 
        elem_name = elist[i];
        plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

        // print the name of the element, for debugging 

        // if we're on vertex elements, read them in 
        if (strncmp("vertex", elem_name, sizeof("vertex")) == 0) {
            vertex_count = num_elems * 3;
            tex_count = num_elems * 2;
            vertices = malloc(sizeof(float) * vertex_count);
            normals = malloc(sizeof(float) * vertex_count);
            tangents = malloc(sizeof(float) * vertex_count);
            tex_coords = malloc(sizeof(float) * tex_count);

            // create a vertex list to hold all the vertices 
            vlist = (Vertex **) malloc (sizeof (Vertex *) * num_elems);

            // set up for getting vertex elements 
            ply_get_property (ply, elem_name, &vert_props[0]);
            ply_get_property (ply, elem_name, &vert_props[1]);
            ply_get_property (ply, elem_name, &vert_props[2]);
            ply_get_property (ply, elem_name, &vert_props[3]);
            ply_get_property (ply, elem_name, &vert_props[4]);
            ply_get_property (ply, elem_name, &vert_props[5]);
            ply_get_property (ply, elem_name, &vert_props[6]);
            ply_get_property (ply, elem_name, &vert_props[7]);

            // grab all the vertex elements 
            for (j = 0; j < num_elems; j++) {
                // grab and element from the file 
                vlist[j] = (Vertex *) malloc (sizeof (Vertex));
                assert(vlist[j] != NULL);
                ply_get_element (ply, (void *) vlist[j]);

                // print out vertex x,y,z for debugging 
                int x = (j * 3);
                int y = (j * 3) + 1;
                int z = (j * 3) + 2; 
                vertices[x] = vlist[j]->x;
                vertices[y] = vlist[j]->y;
                vertices[z] = vlist[j]->z;

                normals[x] = vlist[j]->nx;
                normals[y] = vlist[j]->ny;
                normals[z] = vlist[j]->nz;

                tex_coords[x] = vlist[j]->u;
                tex_coords[y] = vlist[j]->v;
            }
        }

        // if we're on face elements, read them in 
        if (strncmp("face", elem_name, sizeof("face")) == 0) {
            // create a list to hold all the face elements 
            face_count = num_elems * 3;
            faces = malloc(sizeof(GLuint) * face_count);
            flist = (Face **) malloc (sizeof (Face *) * num_elems);

            // set up for getting face elements 
            ply_get_property (ply, elem_name, &face_props[0]);
            ply_get_property (ply, elem_name, &face_props[1]);

            // grab all the face elements 
            int current_tan = 0;
            for (j = 0; j < num_elems; j++) {
                // grab and element from the file 
                flist[j] = (Face *) malloc (sizeof (Face));
                ply_get_element (ply, (void *) flist[j]);

                // print out face info, for debugging 

                int x = (j * 3);
                int y = (j * 3) + 1;
                int z = (j * 3) + 2;
                faces[x] = flist[j]->verts[0];
                faces[y] = flist[j]->verts[1];
                faces[z] = flist[j]->verts[2];

                float pAx = vertices[faces[x]];
                float pAy = vertices[faces[x] + 1];
                float pAz = vertices[faces[x] + 2];
                float tAx = tex_coords[faces[x]];
                float tAy = tex_coords[faces[x] + 1];

                float pBx = vertices[faces[y]];
                float pBy = vertices[faces[y] + 1];
                float pBz = vertices[faces[y] + 2];
                float tBx = tex_coords[faces[y]];
                float tBy = tex_coords[faces[y] + 1];

                float pCx = vertices[faces[z]];
                float pCy = vertices[faces[z] + 1];
                float pCz = vertices[faces[z] + 2];
                float tCx = tex_coords[faces[z]];
                float tCy = tex_coords[faces[z] + 1];
                printf("FACE\n");

                for (k = 0; k < 3; k++) {
                    printf("VERT %d %d %d\n", k*3, k*3+1, k*3+2);
                    get_sl_tangent(
                        pAx, pAy, pAz, tAx, tAy,
                        pBx, pBy, pBz, tBx, tBy,
                        pCx, pCy, pCz, tCx, tCy,
                        &(tangents[current_tan]),
                        &(tangents[current_tan + 1]),
                        &(tangents[current_tan + 2]));
                    current_tan += 2;

                // swap around
                float tmpx, tmpy, tmpz;
                tmpx = pAx;
                tmpy = pAy;
                tmpz = pAz;
                pAx = pCx;
                pAy = pCy;
                pAz = pCz;
                pCx = pBx;
                pCy = pBy;
                pCz = pBz;
                pBx = tmpx;
                pBy = tmpy;
                pBz = tmpz;

                tAx = tCx;
                tAy = tCy;
                tCx = tBx;
                tCy = tBy;
                tBx = tmpx;
                tBy = tmpy;
                }
            }
        }

        // print out the properties we got, for debugging 
        for (j = 0; j < nprops; j++) {
        }

        }

        // grab and print out the comments in the file 
        comments = ply_get_comments (ply, &num_comments);
        for (i = 0; i < num_comments; i++) {
        }

        // grab and print out the object information 
        obj_info = ply_get_obj_info (ply, &num_obj_info);
        for (i = 0; i < num_obj_info; i++) {
        }

        ply_close(ply);
}
