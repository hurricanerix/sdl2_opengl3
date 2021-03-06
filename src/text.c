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


char *text_file_read(char *filename, int *size)
{
    assert(filename != NULL);

    FILE *fp;
    char *content = NULL;
    int count = 0;

    if (filename != NULL) {
        fp = fopen(filename,"rt");

        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            *size = count + 1;
            rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (*size));
                *size = fread(content,sizeof(char), *size, fp);
                content[*size] = '\0';
            }

            fclose(fp);
        }
    }

    return content;
}

int get_next_line(char *data, int data_size, char *buffer, int buffer_size)
{
    static char *prev_data = NULL;
    static int position = 0;

    assert(data != NULL);
    assert(buffer != NULL);


    if (position >= data_size) {
        return FALSE;
    }

    if (prev_data != data) {
        prev_data = data;
        position = 0;
    }

    int start_position = position;
    char current_char;
    int is_eol = FALSE;

    while (!is_eol) {
        assert(position < data_size);
        current_char = data[position];

        if (current_char == '\n' || position >= data_size) {
            is_eol = TRUE;
            break;
        }

        position++;
    }

    int copy_size = position - start_position;
    if (copy_size > buffer_size) {
        copy_size = buffer_size;
    }

    strncpy(buffer, &data[start_position], copy_size);
    buffer[copy_size] = '\0';

    position++;

    return TRUE;
}
