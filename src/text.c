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
#include "text.h"

#include "logger.h"
#include "main.h"


char *text_file_read(char *fn, int *size)
{
    assert(fn != NULL);
    log_debug("text_file_read {");
    log_debug("  -in- fn - %s", fn);
    log_debug("  -in- size - %x", size);

    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

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

    log_debug("text_file_read }");
    log_debug("  -out- size - %d", *size);
    return content;
}

int get_next_line(char *buffer, int buffer_size)
{
    int result = FALSE;
    log_debug("get_next_line {");
    log_debug("  -in- buffer - %x", buffer);
    log_debug("  -in- buffer_size - %d", buffer_size);

    log_debug("get_next_line }");
    log_debug("  -out- result - %d", result);
    return result;
}
