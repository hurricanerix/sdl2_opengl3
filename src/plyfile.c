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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "logger.h"
#include "plyfile.h"


char *type_names[] = {
"invalid",
"char", "short", "int",
"uchar", "ushort", "uint",
"float", "double",
};

int ply_type_size[] = {
  0, 1, 2, 4, 1, 2, 4, 4, 8
};

#define NO_OTHER_PROPS  -1

#define DONT_STORE_PROP  0
#define STORE_PROP       1

#define OTHER_PROP       0
#define NAMED_PROP       1


/* returns 1 if strings are equal, 0 if not */
int equal_strings(char *, char *);

/* find an element in a plyfile's list */
PlyElement *find_element(PlyFile *, char *);

/* find a property in an element's list */
PlyProperty *find_property(PlyElement *, char *, int *);

/* read a line from a file and break it up into separate words */
char **get_words(FILE *, int *, char **);

/* add information to a PLY file descriptor */
void add_element(PlyFile *, char **, int);
void add_property(PlyFile *, char **, int);
void add_comment(PlyFile *, char *);
void add_obj_info(PlyFile *, char *);

/* copy a property */
void copy_property(PlyProperty *, PlyProperty *);

/* store a value into where a pointer and a type specify */
void store_item(char *, int, int, unsigned int, double);

/* return the value of a stored item */
void get_stored_item( void *, int, int *, unsigned int *, double *);

/* return the value stored in an item, given ptr to it and its type */
double get_item_value(char *, int);

/* get binary or ascii item and store it according to ptr and type */
void get_ascii_item(char *, int, int *, unsigned int *, double *);
//void get_binary_item(FILE *, int, int *, unsigned int *, double *);

/* get a bunch of elements from a file */
void ascii_get_element(PlyFile *, char *);
//void binary_get_element(PlyFile *, char *);

/* memory allocation */
char *my_alloc(int, int, char *);


/******************************************************************************
Specify a comment that will be written in the header.

Entry:
  plyfile - file identifier
  comment - the comment to be written
******************************************************************************/
void ply_put_comment(PlyFile *plyfile, char *comment)
{
  /* (re)allocate space for new comment */
  if (plyfile->num_comments == 0)
    plyfile->comments = (char **) myalloc (sizeof (char *));
  else
    plyfile->comments = (char **) realloc (plyfile->comments,
                         sizeof (char *) * (plyfile->num_comments + 1));

  /* add comment to list */
  plyfile->comments[plyfile->num_comments] = strdup (comment);
  plyfile->num_comments++;
}


/******************************************************************************
Specify a piece of object information (arbitrary text) that will be written
in the header.

Entry:
  plyfile  - file identifier
  obj_info - the text information to be written
******************************************************************************/
void ply_put_obj_info(PlyFile *plyfile, char *obj_info)
{
  /* (re)allocate space for new info */
  if (plyfile->num_obj_info == 0)
    plyfile->obj_info = (char **) myalloc (sizeof (char *));
  else
    plyfile->obj_info = (char **) realloc (plyfile->obj_info,
                         sizeof (char *) * (plyfile->num_obj_info + 1));

  /* add info to list */
  plyfile->obj_info[plyfile->num_obj_info] = strdup (obj_info);
  plyfile->num_obj_info++;
}


/******************************************************************************
Given a file pointer, get ready to read PLY data from the file.

Entry:
  fp - the given file pointer

Exit:
  nelems     - number of elements in object
  elem_names - list of element names
  returns a pointer to a PlyFile, used to refer to this file, or NULL if error
******************************************************************************/
PlyFile *ply_read(FILE *fp, int *nelems, char ***elem_names)
{
    int i,j;
    PlyFile *plyfile;
    int nwords;
    char **words;
    int found_format = 0;
    char **elist;
    PlyElement *elem;
    char *orig_line;

    // check for NULL file pointer
    assert(fp != NULL);

    // create record for this object

    plyfile = (PlyFile *) myalloc (sizeof (PlyFile));
    plyfile->nelems = 0;
    plyfile->comments = NULL;
    plyfile->num_comments = 0;
    plyfile->obj_info = NULL;
    plyfile->num_obj_info = 0;
    plyfile->fp = fp;
    //plyfile->other_elems = NULL;

    // read and parse the file's header
    words = get_words (plyfile->fp, &nwords, &orig_line);
    assert(words && equal_strings(words[0], "ply"));

    while (words) {
        // parse words

        if (equal_strings (words[0], "format")) {
            if (nwords != 3) {
                return (NULL);
            }
            if (equal_strings (words[1], "ascii")) {
                plyfile->file_type = PLY_ASCII;
            } else if (equal_strings (words[1], "binary_big_endian")) {
                plyfile->file_type = PLY_BINARY_BE;
            } else if (equal_strings (words[1], "binary_little_endian")) {
                plyfile->file_type = PLY_BINARY_LE;
            } else {
                return (NULL);
            }
            plyfile->version = atof (words[2]);
            found_format = 1;
        }
        else if (equal_strings (words[0], "element"))
            add_element (plyfile, words, nwords);
        else if (equal_strings (words[0], "property"))
            add_property (plyfile, words, nwords);
        else if (equal_strings (words[0], "comment"))
            add_comment (plyfile, orig_line);
        else if (equal_strings (words[0], "obj_info"))
            add_obj_info (plyfile, orig_line);
        else if (equal_strings (words[0], "end_header"))
            break;

        // free up words space
        free (words);

        words = get_words (plyfile->fp, &nwords, &orig_line);
    }

    // create tags for each property of each element, to be used
    // later to say whether or not to store each property for the user

    for (i = 0; i < plyfile->nelems; i++) {
        elem = plyfile->elems[i];
        elem->store_prop = (char *) myalloc (sizeof (char) * elem->nprops);
        for (j = 0; j < elem->nprops; j++) {
            elem->store_prop[j] = DONT_STORE_PROP;
        }
        elem->other_offset = NO_OTHER_PROPS; // no "other" props by default
    }

    // set return values about the elements

    elist = (char **) myalloc (sizeof (char *) * plyfile->nelems);
    for (i = 0; i < plyfile->nelems; i++) {
        elist[i] = strdup (plyfile->elems[i]->name);
    }

    *elem_names = elist;
    *nelems = plyfile->nelems;

    // return a pointer to the file's information 

    return (plyfile);
}


/******************************************************************************
Open a polygon file for reading.

Entry:
  filename - name of file to read from

Exit:
  nelems     - number of elements in object
  elem_names - list of element names
  file_type  - file type, either ascii or binary
  version    - version number of PLY file
  returns a file identifier, used to refer to this file, or NULL if error
******************************************************************************/
PlyFile *ply_open_for_reading(char *filename, int *nelems, char ***elem_names,
        int *file_type, float *version)
{
    assert(filename != NULL);

    FILE *fp;
    PlyFile *plyfile;

    // open the file for reading
    fp = fopen (filename, "r");
    if (fp == NULL) {
        assert(fp != NULL);
    }

    // create the PlyFile data structure
    plyfile = ply_read(fp, nelems, elem_names);
    assert(plyfile != NULL);

    // determine the file type and version
    *file_type = plyfile->file_type;

    *version = plyfile->version;

    // return a pointer to the file's information */
    return (plyfile);
}


/******************************************************************************
Get information about a particular element.

Entry:
  plyfile   - file identifier
  elem_name - name of element to get information about

Exit:
  nelems   - number of elements of this type in the file
  nprops   - number of properties
  returns a list of properties, or NULL if the file doesn't contain that elem
******************************************************************************/
PlyProperty **ply_get_element_description( PlyFile *plyfile, char *elem_name,
    int *nelems, int *nprops)
{
    int i;
    PlyElement *elem;
    PlyProperty *prop;
    PlyProperty **prop_list;

    // find information about the element
    elem = find_element (plyfile, elem_name);
    //if (elem == NULL) {
    //    return (NULL);
    //}
    assert(elem != NULL);

    *nelems = elem->num;
    *nprops = elem->nprops;

    // make a copy of the element's property list
    prop_list = (PlyProperty **) myalloc (sizeof (PlyProperty *) * elem->nprops);
    for (i = 0; i < elem->nprops; i++) {
        prop = (PlyProperty *) myalloc (sizeof (PlyProperty));
        copy_property (prop, elem->props[i]);
        prop_list[i] = prop;
    }

    // return this duplicate property list
    return (prop_list);
}


/******************************************************************************
Specify which properties of an element are to be returned.  This should be
called before a call to the routine ply_get_element().

Entry:
  plyfile   - file identifier
  elem_name - which element we're talking about
  nprops    - number of properties
  prop_list - list of properties
******************************************************************************/

//void ply_get_element_setup(
//  PlyFile *plyfile,
//  char *elem_name,
//  int nprops,
//  PlyProperty *prop_list
//)
//{
//  int i;
//  PlyElement *elem;
//  PlyProperty *prop;
//  int index;
//
//  /* find information about the element */
//  elem = find_element (plyfile, elem_name);
//  plyfile->which_elem = elem;
//
//  /* deposit the property information into the element's description */
//  for (i = 0; i < nprops; i++) {
//
//    /* look for actual property */
//    prop = find_property (elem, prop_list[i].name, &index);
//    if (prop == NULL) {
//      fprintf (stderr, "Warning:  Can't find property '%s' in element '%s'\n",
//               prop_list[i].name, elem_name);
//      continue;
//    }
//
//    /* store its description */
//    prop->internal_type = prop_list[i].internal_type;
//    prop->offset = prop_list[i].offset;
//    prop->count_internal = prop_list[i].count_internal;
//    prop->count_offset = prop_list[i].count_offset;
//
//    /* specify that the user wants this property */
//    elem->store_prop[index] = STORE_PROP;
//  }
//}


/******************************************************************************
Specify a property of an element that is to be returned.  This should be
called (usually multiple times) before a call to the routine ply_get_element().
This routine should be used in preference to the less flexible old routine
called ply_get_element_setup().

Entry:
  plyfile   - file identifier
  elem_name - which element we're talking about
  prop      - property to add to those that will be returned
******************************************************************************/
void ply_get_property(PlyFile *plyfile, char *elem_name, PlyProperty *prop)
{
    PlyElement *elem;
    PlyProperty *prop_ptr;
    int index;

    // find information about the element
    elem = find_element (plyfile, elem_name);

    plyfile->which_elem = elem;

    // deposit the property information into the element's description */

    prop_ptr = find_property (elem, prop->name, &index);
    if (prop_ptr == NULL) {
        log_error("Can't find property '%s' in element '%s'\n",
             prop->name, elem_name);
        return;
    }
    prop_ptr->internal_type  = prop->internal_type;
    prop_ptr->offset         = prop->offset;
    prop_ptr->count_internal = prop->count_internal;
    prop_ptr->count_offset   = prop->count_offset;

    // specify that the user wants this property */
    elem->store_prop[index] = STORE_PROP;
}


/******************************************************************************
Read one element from the file.  This routine assumes that we're reading
the type of element specified in the last call to the routine
ply_get_element_setup().

Entry:
  plyfile  - file identifier
  elem_ptr - pointer to location where the element information should be put
******************************************************************************/
void ply_get_element(PlyFile *plyfile, void *elem_ptr)
{
    if (plyfile->file_type == PLY_ASCII) {
        ascii_get_element (plyfile, (char *) elem_ptr);
    } else {
        //binary_get_element (plyfile, (char *) elem_ptr);
    }
}


/******************************************************************************
Extract the comments from the header information of a PLY file.

Entry:
  plyfile - file identifier

Exit:
  num_comments - number of comments returned
  returns a pointer to a list of comments
******************************************************************************/
char **ply_get_comments(PlyFile *plyfile, int *num_comments)
{
  *num_comments = plyfile->num_comments;
  return (plyfile->comments);
}


/******************************************************************************
Extract the object information (arbitrary text) from the header information
of a PLY file.

Entry:
  plyfile - file identifier

Exit:
  num_obj_info - number of lines of text information returned
  returns a pointer to a list of object info lines
******************************************************************************/
char **ply_get_obj_info(PlyFile *plyfile, int *num_obj_info)
{
  *num_obj_info = plyfile->num_obj_info;
  return (plyfile->obj_info);
}


/******************************************************************************
Close a PLY file.

Entry:
  plyfile - identifier of file to close
******************************************************************************/
void ply_close(PlyFile *plyfile)
{
  fclose (plyfile->fp);

  /* free up memory associated with the PLY file */
  free (plyfile);
}


/******************************************************************************
Compare two strings.  Returns 1 if they are the same, 0 if not.
******************************************************************************/
int equal_strings(char *s1, char *s2)
{
    int result;
    result  = strncmp(s1, s2, 32);
    for (int i = 0; i < 4; i++) {
        //log_debug("s1 - %x", s1[i]);
        //log_debug("s2 - %x", s2[i]);
    }
    if (result == 0) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}


/******************************************************************************
Find an element from the element list of a given PLY object.

Entry:
  plyfile - file id for PLY file
  element - name of element we're looking for

Exit:
  returns the element, or NULL if not found
******************************************************************************/
PlyElement *find_element(PlyFile *plyfile, char *element)
{
    int i;

    for (i = 0; i < plyfile->nelems; i++)
        if (equal_strings (element, plyfile->elems[i]->name)) {
            return (plyfile->elems[i]);
        }

    return (NULL);
}


/******************************************************************************
Find a property in the list of properties of a given element.

Entry:
  elem      - pointer to element in which we want to find the property
  prop_name - name of property to find

Exit:
  index - index to position in list
  returns a pointer to the property, or NULL if not found
******************************************************************************/
PlyProperty *find_property(PlyElement *elem, char *prop_name, int *index)
{
  int i;

  for (i = 0; i < elem->nprops; i++)
    if (equal_strings (prop_name, elem->props[i]->name)) {
      *index = i;
      return (elem->props[i]);
    }

  *index = -1;
  return (NULL);
}


/******************************************************************************
Read an element from an ascii file.

Entry:
  plyfile  - file identifier
  elem_ptr - pointer to element
******************************************************************************/
void ascii_get_element(PlyFile *plyfile, char *elem_ptr)
{
    //int i,j,k;
    int j,k;
    PlyElement *elem;
    PlyProperty *prop;
    char **words;
    int nwords;
    int which_word;
    //FILE *fp = plyfile->fp;
    char *elem_data,*item;
    char *item_ptr;
    int item_size;
    int int_val;
    unsigned int uint_val;
    double double_val;
    int list_count;
    int store_it;
    char **store_array;
    char *orig_line;
    char *other_data;
    int other_flag;

    // the kind of element we're reading currently
    elem = plyfile->which_elem;

    // do we need to setup for other_props?

    if (elem->other_offset != NO_OTHER_PROPS) {
        char **ptr;
        other_flag = 1;
        // make room for other_props
        other_data = (char *) myalloc (elem->other_size);
        // store pointer in user's structure to the other_props
        ptr = (char **) (elem_ptr + elem->other_offset);
        *ptr = other_data;
    } else {
        other_flag = 0;
    }

    // read in the element

    words = get_words(plyfile->fp, &nwords, &orig_line);
    if (words == NULL) {
        fprintf (stderr, "ply_get_element: unexpected end of file\n");
        exit (-1);
    }

    which_word = 0;

    for (j = 0; j < elem->nprops; j++) {
        prop = elem->props[j];
        store_it = (elem->store_prop[j] | other_flag);

        // store either in the user's structure or in other_props
        if (elem->store_prop[j]) {
            elem_data = elem_ptr;
        } else {
            elem_data = other_data;
        }

        if (prop->is_list) {       // a list
            // get and store the number of items in the list
            get_ascii_item (words[which_word++], prop->count_external,
                &int_val, &uint_val, &double_val);
            if (store_it) {
                item = elem_data + prop->count_offset;
                store_item(item, prop->count_internal, int_val, uint_val,
                    double_val);
            }

            // allocate space for an array of items and store a ptr to the array
            list_count = int_val;
            item_size = ply_type_size[prop->internal_type];
            store_array = (char **) (elem_data + prop->offset);

            if (list_count == 0) {
                if (store_it) {
                    *store_array = NULL;
                }
            } else {
                if (store_it) {
                    item_ptr = (char *) myalloc (sizeof (char) * item_size * list_count);
                    item = item_ptr;
                    *store_array = item_ptr;
                }

                // read items and store them into the array
                for (k = 0; k < list_count; k++) {
                    get_ascii_item (words[which_word++], prop->external_type,
                          &int_val, &uint_val, &double_val);
                    if (store_it) {
                        store_item (item, prop->internal_type,
                            int_val, uint_val, double_val);
                        item += item_size;
                    }
                }
            }
        } else {                     // not a list 
            get_ascii_item (words[which_word++], prop->external_type,
                &int_val, &uint_val, &double_val);
            if (store_it) {
                item = elem_data + prop->offset;
                store_item (item, prop->internal_type, int_val, uint_val, double_val);
            }
        }
    }

    free (words);
}


/******************************************************************************
Get a text line from a file and break it up into words.

IMPORTANT: The calling routine call "free" on the returned pointer once
finished with it.

Entry:
  fp - file to read from

Exit:
  nwords    - number of words returned
  orig_line - the original line of characters
  returns a list of words from the line, or NULL if end-of-file
******************************************************************************/
char **get_words(FILE *fp, int *nwords, char **orig_line)
{
    #define BIG_STRING 4096
    //int i,j;
    static char str[BIG_STRING];
    static char str_copy[BIG_STRING];
    char **words;
    int max_words = 10;
    int num_words = 0;
    char *ptr,*ptr2;
    char *result;

    words = (char **) myalloc (sizeof (char *) * max_words);
    assert(words != NULL);

    // read in a line
    result = fgets (str, BIG_STRING, fp);
    if (result == NULL) {
        *nwords = 0;
        *orig_line = NULL;
        return (NULL);
    }

    // convert line-feed and tabs into spaces
    // (this guarentees that there will be a space before the
    //  null character at the end of the string)

    str[BIG_STRING-2] = ' ';
    str[BIG_STRING-1] = '\0';

    for (ptr = str, ptr2 = str_copy; *ptr != '\0'; ptr++, ptr2++) {
        *ptr2 = *ptr;
        if (*ptr == '\t') {
            *ptr = ' ';
            *ptr2 = ' ';
        } else if (*ptr == '\n') {
            *ptr = ' ';
            *ptr2 = '\0';
            break;
        }
    }

    // find the words in the line

    ptr = str;
    while (*ptr != '\0') {
        // jump over leading spaces
        while (*ptr == ' ') {
            ptr++;
        }

        // break if we reach the end
        if (*ptr == '\0') {
            break;
        }

        // save pointer to beginning of word
        if (num_words >= max_words) {
            max_words += 10;
            words = (char **) realloc (words, sizeof (char *) * max_words);
        }
        words[num_words++] = ptr;

        // jump over non-spaces
        while (*ptr != ' ') {
            ptr++;
        }

        // place a null character here to mark the end of the word
        *ptr++ = '\0';
    }

    // return the list of words
    *nwords = num_words;
    *orig_line = str_copy;
    return (words);
}


/******************************************************************************
Extract the value of an item from an ascii word, and place the result
into an integer, an unsigned integer and a double.

Entry:
  word - word to extract value from
  type - data type supposedly in the word

Exit:
  int_val    - integer value
  uint_val   - unsigned integer value
  double_val - double-precision floating point value
******************************************************************************/
void get_ascii_item(char *word, int type, int *int_val, unsigned int *uint_val,
    double *double_val)
{
    switch (type) {
    case PLY_CHAR:
    case PLY_UCHAR:
    case PLY_SHORT:
    case PLY_USHORT:
    case PLY_INT:
        *int_val = atoi (word);
        *uint_val = *int_val;
        *double_val = *int_val;
        break;

    case PLY_UINT:
        *uint_val = strtoul (word, (char **) NULL, 10);
        *int_val = *uint_val;
        *double_val = *uint_val;
        break;

    case PLY_FLOAT:
    case PLY_DOUBLE:
        *double_val = atof (word);
        *int_val = (int) *double_val;
        *uint_val = (unsigned int) *double_val;
        break;

    default:
        fprintf (stderr, "get_ascii_item: bad type = %d\n", type);
        exit (-1);
  }
}


/******************************************************************************
Store a value into a place being pointed to, guided by a data type.

Entry:
  item       - place to store value
  type       - data type
  int_val    - integer version of value
  uint_val   - unsigned integer version of value
  double_val - double version of value

Exit:
  item - pointer to stored value
******************************************************************************/
void store_item (
  char *item,
  int type,
  int int_val,
  unsigned int uint_val,
  double double_val
)
{
  unsigned char *puchar;
  short int *pshort;
  unsigned short int *pushort;
  int *pint;
  unsigned int *puint;
  float *pfloat;
  double *pdouble;

  switch (type) {
    case PLY_CHAR:
      *item = int_val;
      break;
    case PLY_UCHAR:
      puchar = (unsigned char *) item;
      *puchar = uint_val;
      break;
    case PLY_SHORT:
      pshort = (short *) item;
      *pshort = int_val;
      break;
    case PLY_USHORT:
      pushort = (unsigned short *) item;
      *pushort = uint_val;
      break;
    case PLY_INT:
      pint = (int *) item;
      *pint = int_val;
      break;
    case PLY_UINT:
      puint = (unsigned int *) item;
      *puint = uint_val;
      break;
    case PLY_FLOAT:
      pfloat = (float *) item;
      *pfloat = double_val;
      break;
    case PLY_DOUBLE:
      pdouble = (double *) item;
      *pdouble = double_val;
      break;
    default:
      fprintf (stderr, "store_item: bad type = %d\n", type);
      exit (-1);
  }
}


/******************************************************************************
Add an element to a PLY file descriptor.

Entry:
  plyfile - PLY file descriptor
  words   - list of words describing the element
  nwords  - number of words in the list
******************************************************************************/
void add_element (PlyFile *plyfile, char **words, int nwords)
{
  PlyElement *elem;

  /* create the new element */
  elem = (PlyElement *) myalloc (sizeof (PlyElement));
  elem->name = strdup (words[1]);
  elem->num = atoi (words[2]);
  elem->nprops = 0;

  /* make room for new element in the object's list of elements */
  if (plyfile->nelems == 0)
    plyfile->elems = (PlyElement **) myalloc (sizeof (PlyElement *));
  else
    plyfile->elems = (PlyElement **) realloc (plyfile->elems,
                     sizeof (PlyElement *) * (plyfile->nelems + 1));

  /* add the new element to the object's list */
  plyfile->elems[plyfile->nelems] = elem;
  plyfile->nelems++;
}


/******************************************************************************
Return the type of a property, given the name of the property.

Entry:
  name - name of property type

Exit:
  returns integer code for property, or 0 if not found
******************************************************************************/
int get_prop_type(char *type_name)
{
  int i;

  for (i = PLY_START_TYPE + 1; i < PLY_END_TYPE; i++)
    if (equal_strings (type_name, type_names[i]))
      return (i);

  /* if we get here, we didn't find the type */
  return (0);
}


/******************************************************************************
Add a property to a PLY file descriptor.

Entry:
  plyfile - PLY file descriptor
  words   - list of words describing the property
  nwords  - number of words in the list
******************************************************************************/
void add_property (PlyFile *plyfile, char **words, int nwords)
{
  //int prop_type;
  //int count_type;
  PlyProperty *prop;
  PlyElement *elem;

  /* create the new property */

  prop = (PlyProperty *) myalloc (sizeof (PlyProperty));

  if (equal_strings (words[1], "list")) {       /* is a list */
    prop->count_external = get_prop_type (words[2]);
    prop->external_type = get_prop_type (words[3]);
    prop->name = strdup (words[4]);
    prop->is_list = 1;
  }
  else {                                        /* not a list */
    prop->external_type = get_prop_type (words[1]);
    prop->name = strdup (words[2]);
    prop->is_list = 0;
  }

  /* add this property to the list of properties of the current element */

  elem = plyfile->elems[plyfile->nelems - 1];

  if (elem->nprops == 0)
    elem->props = (PlyProperty **) myalloc (sizeof (PlyProperty *));
  else
    elem->props = (PlyProperty **) realloc (elem->props,
                  sizeof (PlyProperty *) * (elem->nprops + 1));

  elem->props[elem->nprops] = prop;
  elem->nprops++;
}


/******************************************************************************
Add a comment to a PLY file descriptor.

Entry:
  plyfile - PLY file descriptor
  line    - line containing comment
******************************************************************************/
void add_comment (PlyFile *plyfile, char *line)
{
  int i;

  /* skip over "comment" and leading spaces and tabs */
  i = 7;
  while (line[i] == ' ' || line[i] == '\t')
    i++;

  ply_put_comment (plyfile, &line[i]);
}


/******************************************************************************
Add a some object information to a PLY file descriptor.

Entry:
  plyfile - PLY file descriptor
  line    - line containing text info
******************************************************************************/
void add_obj_info (PlyFile *plyfile, char *line)
{
  int i;

  /* skip over "obj_info" and leading spaces and tabs */
  i = 8;
  while (line[i] == ' ' || line[i] == '\t')
    i++;

  ply_put_obj_info (plyfile, &line[i]);
}


/******************************************************************************
Copy a property.
******************************************************************************/
void copy_property(PlyProperty *dest, PlyProperty *src)
{
  dest->name = strdup (src->name);
  dest->external_type = src->external_type;
  dest->internal_type = src->internal_type;
  dest->offset = src->offset;

  dest->is_list = src->is_list;
  dest->count_external = src->count_external;
  dest->count_internal = src->count_internal;
  dest->count_offset = src->count_offset;
}


/******************************************************************************
Allocate some memory.

Entry:
  size  - amount of memory requested (in bytes)
  lnum  - line number from which memory was requested
  fname - file name from which memory was requested
******************************************************************************/

//static char *my_alloc(int size, int lnum, char *fname)
char *my_alloc(int size, int lnum, char *fname)
{
    assert(fname != NULL);

    char *ptr;

    ptr = (char *) malloc (size);

    if (ptr == 0) {
        printf("Memory allocation bombed on line %d in %s\n", lnum, fname);
    }

  return (ptr);
}

