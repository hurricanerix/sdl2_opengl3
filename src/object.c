#include <assert.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <OpenGL/gl3.h>

#include "logger.h"
#include "shader.h"
#include "object.h"
#include "plyfile.h"


PlyProperty vert_props[] = { /* list of property information for a vertex */
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
};
PlyProperty face_props[] = { /* list of property information for a vertex */
  {"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};


GLuint vao[3];

float *vertices;
GLuint *faces;
GLuint face_count = -1;
GLuint vertex_count = -1;
GLuint buffers[2];

void read_object(char *file_name);

void setupBuffers(char *file_name) {
    log_info("Setup Buffers...");
    assert(file_name != NULL);

    read_object(file_name);
    assert(vertex_count != -1);
    assert(face_count != -1);

    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_count, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &buffers[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * face_count, faces, GL_STATIC_DRAW);

    // bind buffer for colors and copy data into buffer
    /*
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
    */
    log_info("Setup Buffers Complete");
}

void render_object() {
    glDrawElements(GL_TRIANGLES, face_count, GL_UNSIGNED_INT, NULL);
}

void destroy_object() {
    log_info("Destroy Object...");
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
    log_info("Destroy Object Complete");
}

void read_object(char *file_name) {
    assert(file_name != NULL);
    log_info("Read Object '%s'...", file_name);

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
    log_info("Open PLY For Reading");
    ply = ply_open_for_reading(file_name, &nelems, &elist, &file_type, &version);

    // print what we found out about the file 
    log_info("Version: %f", version);
    log_info("Type: %d", file_type);

    // go through each kind of element that we learned is in the file 
    // and read them 

    for (i = 0; i < nelems; i++) {
        // get the description of the first element 
        elem_name = elist[i];
        plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

        // print the name of the element, for debugging 
        log_info("Element %s %d", elem_name, num_elems);

        // if we're on vertex elements, read them in 
        if (equal_strings ("vertex", elem_name)) {
            vertex_count = num_elems * 3;
            vertices = malloc(sizeof(float) * vertex_count);

            // create a vertex list to hold all the vertices 
            vlist = (Vertex **) malloc (sizeof (Vertex *) * num_elems);

            // set up for getting vertex elements 
            ply_get_property (ply, elem_name, &vert_props[0]);
            ply_get_property (ply, elem_name, &vert_props[1]);
            ply_get_property (ply, elem_name, &vert_props[2]);

            // grab all the vertex elements 
            for (j = 0; j < num_elems; j++) {
                // grab and element from the file 
                vlist[j] = (Vertex *) malloc (sizeof (Vertex));
                printf("foo\n");
                ply_get_element (ply, (void *) vlist[j]);
                printf("bar\n");

                // print out vertex x,y,z for debugging 
                log_info("Vertex: %g %g %g", vlist[j]->x, vlist[j]->y, vlist[j]->z);
                int base = j * 3;
                vertices[base] = vlist[j]->x;
                vertices[base + 1] = vlist[j]->y;
                vertices[base + 2] = vlist[j]->z;
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
                log_info("Face: %d, list = ", flist[j]->intensity);

                int base = j * 3;
                faces[base] = flist[j]->verts[0];
                faces[base + 1] = flist[j]->verts[1];
                faces[base + 2] = flist[j]->verts[2];

                for (k = 0; k < flist[j]->nverts; k++)
                    log_info("-- %d ", flist[j]->verts[k]);
            }
        }

        // print out the properties we got, for debugging 
        for (j = 0; j < nprops; j++)
            log_info("property %s", plist[j]->name);
        }

        // grab and print out the comments in the file 
        comments = ply_get_comments (ply, &num_comments);
        for (i = 0; i < num_comments; i++) {
            log_info("comment = '%s'", comments[i]);
        }

        // grab and print out the object information 
        obj_info = ply_get_obj_info (ply, &num_obj_info);
        for (i = 0; i < num_obj_info; i++) {
            log_info("obj_info = '%s'", obj_info[i]);
        }

        log_info("Closing PLY");
        ply_close(ply);

    log_info("Read Object Complete");
}
