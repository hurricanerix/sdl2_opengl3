/*
 * the mit license (mit)
 *
 * copyright (c) 2013 richard hawkins
 *
 * permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "software"), to deal
 * in the software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the software is
 * furnished to do so, subject to the following conditions:
 *
 * the above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the software.
 *
 * the software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. in no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in
 * the software.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"


// Data read from the header of the BMP file
unsigned char header[54]; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;   // = width*height*3
// Actual RGB data


void load_bmp(char *imagepath, int *width, int *height, unsigned char **data)
{
    assert(imagepath != NULL);

    // Open the file
    FILE * file = fopen(imagepath,"rb");

    if (!file) {
        log_error("Image could not be opened");
        exit(1);
    }

    if (fread(header, 1, 54, file)!=54){ // If not 54 bytes read : problem
        log_error("Image could not be opened");
        exit(1);
    }

    if ( header[0]!='B' || header[1]!='M' ){
        log_error("Not a correct BMP file");
        exit(1);
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    (*width)      = *(int*)&(header[0x12]);
    (*height)    = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0) {
        imageSize=(*width) * (*height) *3; // 3 : one byte for each Red, Green and Blue component
    }

    if (dataPos==0) {
        dataPos=54; // The BMP header is done that way
    }

    // Create a buffer
    *data = malloc(sizeof(unsigned char) * imageSize);
    assert(data != NULL);

    // Read the actual data from the file into the buffer
    if (fread(*data, 1, imageSize, file) == 0) {
        fprintf(stderr, "ERROR BMP\n");
    }

    //Everything is in memory now, the file can be closed
    fclose(file);

}
