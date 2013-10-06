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
#include "includes.h"


#define OBJECT_DEFAULT_FILENAME ('\0')
#define OBJECT_DEFAULT_VERTEX_COUNT (0)
#define OBJECT_DEFAULT_TRIANGLE_COUNT (0)
#define OBJECT_DEFAULT_VERTICES (NULL)
#define OBJECT_DEFAULT_NORMALS (NULL)
#define OBJECT_DEFAULT_TANGENTS (NULL)
#define OBJECT_DEFAULT_TEX_COORDS (NULL)
#define OBJECT_DEFAULT_TRIANGLES (NULL)
#define OBJECT_DEFAULT_SHADER (NULL)
#define OBJECT_DEFAULT_COLORMAP (NULL)
#define OBJECT_DEFAULT_NORMALMAP (NULL)

// list of property information for a vertex
PlyProperty vert_props[] = {
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
  {"nx", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nx), 0, 0, 0, 0},
  {"ny", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,ny), 0, 0, 0, 0},
  {"nz", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nz), 0, 0, 0, 0},
  {"s", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,s), 0, 0, 0, 0},
  {"t", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,t), 0, 0, 0, 0},
};

// list of property information for a vertex
PlyProperty face_props[] = {
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};


void _set_object_defaults(Object *o);


Object init_object()
{
    Object o;
    init_status(&o.status);

    _set_object_defaults(&o);

    return o;
}

void load_object(Object *o, char *filename, Shader *shader, Texture *colormap,
        Texture *normalmap)
{
    assert(o != NULL);
    assert(filename != NULL);
    assert(shader != NULL);

    o->status.is_error = FALSE;

    if (strncmp(o->filename, filename, MAX_FILENAME_LEN) == 0) {
        return;
    }

    strncpy(o->filename, (filename), MAX_FILENAME_LEN);

    o->shader = shader;
    o->shader->ref_count++;

    if (colormap != NULL) {
        o->colormap = colormap;
        o->colormap->ref_count++;
    }

    if (normalmap != NULL) {
        o->normalmap = normalmap;
        o->normalmap->ref_count++;
    }

    // open a PLY file for reading
    PlyFile *ply;
    int nelems;
    char **elist;
    int file_type;
    float version;
    ply = ply_open_for_reading(o->filename, &nelems, &elist, &file_type, &version);

    // go through each kind of element that we learned is in the file
    // and read them
    int nprops;
    int num_elems;
    char *elem_name;

    for (int i = 0; i < nelems; i++) {

        // get the description of the first element
        elem_name = elist[i];
        ply_get_element_description (ply, elem_name, &num_elems, &nprops);

        // if we're on vertex elements, read them in
        if (strncmp("vertex", elem_name, sizeof("vertex")) == 0) {

            o->vertex_count = num_elems;
            o->vertices = malloc(sizeof(vec3) * o->vertex_count);
            o->normals = malloc(sizeof(vec3) * o->vertex_count);
            //o->tangents = malloc(sizeof(vec3) * o->vertex_count);
            o->tex_coords = malloc(sizeof(vec2) * o->vertex_count);

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
            for (int j = 0; j < num_elems; j++) {
                Vertex vertex;
                ply_get_element (ply, (void *) &vertex);

                vec3 *v = &(o->vertices[j]);
                v->x = vertex.x;
                v->y = vertex.y;
                v->z = vertex.z;

                vec3 *n = &(o->normals[j]);
                n->x = vertex.nx;
                n->y = vertex.ny;
                n->z = vertex.nz;

                vec2 *t = &(o->tex_coords[j]);
                t->s = vertex.s;
                t->t = vertex.t;
            }
        }

        if (strncmp("face", elem_name, sizeof("face")) == 0) {
            o->triangles = malloc(sizeof(vec3) * o->triangle_count);

            // we're on face elements
            o->triangle_count = num_elems;

            // set up for getting face elements
            ply_get_property (ply, elem_name, &face_props[0]);

            // grab all the face elements

            for (int j = 0; j < num_elems; j++) {
                Face triangle;
                ply_get_element (ply, (void *) &triangle);

                ivec3 *t = &(o->triangles[j]);
                t->x = triangle.verts[0];
                t->y = triangle.verts[1];
                t->z = triangle.verts[2];

                /*
                vec3 *p1, *p2, *p3, *ptmp;
                p1 = &(o->vertices[t->x]);
                p2 = &(o->vertices[t->y]);
                p3 = &(o->vertices[t->z]);

                vec2 *t1, *t2, *t3, *ttmp;
                t1 = &(o->tex_coords[t->x]);
                t2 = &(o->tex_coords[t->y]);
                t3 = &(o->tex_coords[t->z]);

                for (int k = 0; k < 3; k++) {
                    vec3 tangent = get_surface_local_tangent(*p1, *t1, *p2, *t2, *p3, *t3);
                    vec3 *t = &(o->vertices[j]);
                    t->x = tangent.x;
                    t->y = tangent.y;
                    t->z = tangent.z;

                    ptmp = p1; p1 = p2; p2 = p3; p3 = ptmp;
                    ttmp = t1; t1 = t2; t2 = t3; t3 = ttmp;
                }
            */
            }
        }
    }

    ply_close(ply);
}

void bind_object(Object *o)
{
    assert(o != NULL);
    assert(o->shader != NULL);

    if (o->vertex_count == 0) {
        return;
    }

    glEnable(GL_CULL_FACE);

    glGenBuffers(OBJECT_GL_BUFFER_COUNT, o->gl_buffers);
    // bind buffer for vertex and copy data into buffer

    glBindBuffer(GL_ARRAY_BUFFER, o->gl_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->vertex_count * 3, o->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(o->shader->vertex_loc);
    glVertexAttribPointer(o->shader->vertex_loc, 3, GL_FLOAT, 0, 0, 0);

    // bind buffer for normal and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, o->gl_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->vertex_count * 3, o->normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(o->shader->normal_loc);
    glVertexAttribPointer(o->shader->normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // bind buffer for tanget and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, o->gl_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->vertex_count * 3, o->tangents, GL_STATIC_DRAW);
    glEnableVertexAttribArray(o->shader->tangent_loc);
    glVertexAttribPointer(o->shader->tangent_loc, 3, GL_FLOAT, 0, 0, 0);

    // bind buffer for tex coords and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, o->gl_buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->vertex_count * 2, o->tex_coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(o->shader->tex_coords_loc);
    glVertexAttribPointer(o->shader->tex_coords_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->gl_buffers[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * o->triangle_count, o->triangles, GL_STATIC_DRAW);
}

void render_object(Object *o)
{
    assert(o != NULL);

    glUseProgram(o->shader->program_id);

    glDrawElements(GL_TRIANGLES, o->triangle_count * 3, GL_UNSIGNED_INT, o->triangles);
}

void unbind_object(Object *o)
{
    assert(o != NULL);
    assert(o->shader != NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &o->gl_buffers[0]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (int i = 1; i < OBJECT_GL_BUFFER_COUNT; i++) {
        glDeleteBuffers(1, &o->gl_buffers[i]);
    }
}

void destroy_object(Object *o)
{
    if (o == NULL) {
        return;
    }

    if (glIsBuffer(o->gl_buffers[0])) {
        unbind_object(o);
    }

    free(o->vertices);
    free(o->normals);
    free(o->tangents);
    free(o->tex_coords);
    free(o->triangles);
    o->shader->ref_count--;
    if (o->colormap != NULL) {
        o->colormap->ref_count--;
    }
    if (o->normalmap != NULL) {
        o->normalmap->ref_count--;
    }

    _set_object_defaults(o);
}

void _set_object_defaults(Object *o)
{
    assert(o != NULL);

    o->filename[0] = OBJECT_DEFAULT_FILENAME;
    o->vertex_count = OBJECT_DEFAULT_VERTEX_COUNT;
    o->triangle_count = OBJECT_DEFAULT_TRIANGLE_COUNT;
    o->vertices = OBJECT_DEFAULT_VERTICES;
    o->normals = OBJECT_DEFAULT_NORMALS;
    o->tangents = OBJECT_DEFAULT_TANGENTS;
    o->tex_coords = OBJECT_DEFAULT_TEX_COORDS;
    o->triangles = OBJECT_DEFAULT_TRIANGLES;
    o->shader = OBJECT_DEFAULT_SHADER;
    o->colormap = OBJECT_DEFAULT_COLORMAP;
    o->normalmap = OBJECT_DEFAULT_NORMALMAP;
}

void print_object(Object *o)
{
    assert(o != NULL);
    assert(o->shader != NULL);

    fprintf(stderr, "FILENAME: %s\n", o->filename);
    print_shader(o->shader);
    fprintf(stderr, "Vertex Count : %d\n", o->vertex_count);
    for (int i = 0; i < o->vertex_count; i++) {
        fprintf(stderr, "(%f, %f, %f)\n", o->vertices[i].x, o->vertices[i].y, o->vertices[i].z);
    }
    fprintf(stderr, "Normals\n");
    for (int i = 0; i < o->vertex_count; i++) {
        fprintf(stderr, "(%f, %f, %f)\n", o->normals[i].x, o->normals[i].y, o->normals[i].z);
    }
    fprintf(stderr, "Triangle Count : %d\n", o->triangle_count);
    for (int i = 0; i < o->triangle_count; i++) {
        fprintf(stderr, "(%d, %d, %d)\n", o->triangles[i].x, o->triangles[i].y, o->triangles[i].z);
    }
}
