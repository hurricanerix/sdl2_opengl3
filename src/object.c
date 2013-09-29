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
#include "object.h"

#include "logger.h"
#include "plyfile.h"
#include "shader.h"


PlyProperty vert_props[] = { /* list of property information for a vertex */
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
  {"nx", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nx), 0, 0, 0, 0},
  {"ny", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,ny), 0, 0, 0, 0},
  {"nz", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nz), 0, 0, 0, 0},
};
PlyProperty face_props[] = { /* list of property information for a vertex */
  {"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};


GLuint vao[3];

float *vertices;
float *normals;
GLuint *faces;
GLuint face_count = -1;
GLuint vertex_count = -1;
GLuint buffers[3];

void read_object(char *file_name);

void setupBuffers(char *file_name)
{
    assert(file_name != NULL);
    log_debug("setupBuffers {");
    log_debug("  -in- file_name - %s {", file_name);

    read_object(file_name);
    assert(vertex_count != -1);
    assert(face_count != -1);

    //glEnable(GL_CULL_FACE);
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

    glGenBuffers(1, &buffers[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * face_count, faces, GL_STATIC_DRAW);

    log_debug("setupBuffers }");
}

void render_object()
{
    log_debug("render_object {");

    glDrawElements(GL_TRIANGLES, face_count, GL_UNSIGNED_INT, NULL);

    log_debug("render_object }");
}

void destroy_object()
{
    log_debug("destroy_object {");

    glDisableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &buffers[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &buffers[1]);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, vao);

    free(vertices);
    free(faces);
    face_count = -1;
    vertex_count = -1;

    log_debug("destroy_object }");
}

void read_object(char *file_name)
{
    assert(file_name != NULL);
    log_debug("read_object {");
    log_debug("  -in- file_name - %s {", file_name);

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
        //log_info("Element %s %d", elem_name, num_elems);

        // if we're on vertex elements, read them in 
        if (equal_strings ("vertex", elem_name)) {
            vertex_count = num_elems * 3;
            vertices = malloc(sizeof(float) * vertex_count);
            normals = malloc(sizeof(float) * vertex_count);

            // create a vertex list to hold all the vertices 
            vlist = (Vertex **) malloc (sizeof (Vertex *) * num_elems);

            // set up for getting vertex elements 
            ply_get_property (ply, elem_name, &vert_props[0]);
            ply_get_property (ply, elem_name, &vert_props[1]);
            ply_get_property (ply, elem_name, &vert_props[2]);
            ply_get_property (ply, elem_name, &vert_props[3]);
            ply_get_property (ply, elem_name, &vert_props[4]);
            ply_get_property (ply, elem_name, &vert_props[5]);

            // grab all the vertex elements 
            for (j = 0; j < num_elems; j++) {
                // grab and element from the file 
                vlist[j] = (Vertex *) malloc (sizeof (Vertex));
                assert(vlist[j] != NULL);
                ply_get_element (ply, (void *) vlist[j]);

                // print out vertex x,y,z for debugging 
                //log_info("Vertex: %g %g %g", vlist[j]->x, vlist[j]->y, vlist[j]->z);
                //log_info("Normals: %g %g %g", vlist[j]->nx, vlist[j]->ny, vlist[j]->nz);
                int base = j * 3;
                vertices[base] = vlist[j]->x;
                vertices[base + 1] = vlist[j]->y;
                vertices[base + 2] = vlist[j]->z;

                normals[base] = vlist[j]->nx;
                normals[base + 1] = vlist[j]->ny;
                normals[base + 2] = vlist[j]->nz;
            }
        }

        // if we're on face elements, read them in 
        if (equal_strings ("face", elem_name)) {
            // create a list to hold all the face elements 
            face_count = num_elems * 3;
            faces = malloc(sizeof(GLuint) * face_count);
            flist = (Face **) malloc (sizeof (Face *) * num_elems);

            // set up for getting face elements 
            ply_get_property (ply, elem_name, &face_props[0]);
            ply_get_property (ply, elem_name, &face_props[1]);

            // grab all the face elements 
            for (j = 0; j < num_elems; j++) {
                // grab and element from the file 
                flist[j] = (Face *) malloc (sizeof (Face));
                ply_get_element (ply, (void *) flist[j]);

                // print out face info, for debugging 
                //log_info("Face: %d, list = ", flist[j]->intensity);

                int base = j * 3;
                faces[base] = flist[j]->verts[0];
                faces[base + 1] = flist[j]->verts[1];
                faces[base + 2] = flist[j]->verts[2];

                for (k = 0; k < flist[j]->nverts; k++) {
                    //log_info("-- %d ", flist[j]->verts[k]);
                }
            }
        }

        // print out the properties we got, for debugging 
        for (j = 0; j < nprops; j++) {
            //log_info("property %s", plist[j]->name);
        }

        }

        // grab and print out the comments in the file 
        comments = ply_get_comments (ply, &num_comments);
        for (i = 0; i < num_comments; i++) {
            //log_info("comment = '%s'", comments[i]);
        }

        // grab and print out the object information 
        obj_info = ply_get_obj_info (ply, &num_obj_info);
        for (i = 0; i < num_obj_info; i++) {
            //log_info("obj_info = '%s'", obj_info[i]);
        }

        ply_close(ply);

    log_debug("read_object }");
}
