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
#ifndef __PLY_H__
#define __PLY_H__

#define PLY_ASCII      1        /* ascii PLY file */
#define PLY_BINARY_BE  2        /* binary PLY file, big endian */
#define PLY_BINARY_LE  3        /* binary PLY file, little endian */

#define PLY_OKAY    0           /* ply routine worked okay */
#define PLY_ERROR  -1           /* error in ply routine */

// scalar data types supported by PLY format

#define PLY_START_TYPE 0
#define PLY_CHAR       1
#define PLY_SHORT      2
#define PLY_INT        3
#define PLY_UCHAR      4
#define PLY_USHORT     5
#define PLY_UINT       6
#define PLY_FLOAT      7
#define PLY_DOUBLE     8
#define PLY_END_TYPE   9

#define  PLY_SCALAR  0
#define  PLY_LIST    1


typedef struct PlyProperty {    /* description of a property */
  char *name;                           /* property name */
  int external_type;                    /* file's data type */
  int internal_type;                    /* program's data type */
  int offset;                           /* offset bytes of prop in a struct */

  int is_list;                          /* 1 = list, 0 = scalar */
  int count_external;                   /* file's count type */
  int count_internal;                   /* program's count type */
  int count_offset;                     /* offset byte for list count */

} PlyProperty;

typedef struct PlyElement {     /* description of an element */
  char *name;                   /* element name */
  int num;                      /* number of elements in this object */
  int size;                     /* size of element (bytes) or -1 if variable */
  int nprops;                   /* number of properties for this element */
  PlyProperty **props;          /* list of properties in the file */
  char *store_prop;             /* flags: property wanted by user? */
  int other_offset;             /* offset to un-asked-for props, or -1 if none*/
  int other_size;               /* size of other_props structure */
} PlyElement;

typedef struct PlyFile {        /* description of PLY file */
  FILE *fp;                     /* file pointer */
  int file_type;                /* ascii or binary */
  float version;                /* version number of file */
  int nelems;                   /* number of elements of object */
  PlyElement **elems;           /* list of elements */
  int num_comments;             /* number of comments */
  char **comments;              /* list of comments */
  int num_obj_info;             /* number of items of object information */
  char **obj_info;              /* list of object info items */
  PlyElement *which_elem;       /* which element we're currently writing */
} PlyFile;

// memory allocation
extern char *my_alloc();
#define myalloc(mem_size) my_alloc((mem_size), __LINE__, __FILE__)


// delcaration of routines
extern void ply_put_comment(PlyFile *, char *);
extern void ply_put_obj_info(PlyFile *, char *);
extern PlyFile *ply_read(FILE *, int *, char ***);
extern PlyFile *ply_open_for_reading( char *, int *, char ***, int *, float *);
extern PlyProperty **ply_get_element_description(PlyFile *, char *, int*, int*);
//extern void ply_get_element_setup( PlyFile *, char *, int, PlyProperty *);
extern void ply_get_property(PlyFile *, char *, PlyProperty *);
extern void ply_get_element(PlyFile *, void *);
extern char **ply_get_comments(PlyFile *, int *);
extern char **ply_get_obj_info(PlyFile *, int *);
extern void ply_close(PlyFile *);

extern int equal_strings(char *, char *);

#endif /* !__PLY_H__ */

