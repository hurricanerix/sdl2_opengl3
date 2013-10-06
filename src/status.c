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


void init_status(Status *s)
{
    assert(s != NULL);

    s->is_error = FALSE;
    s->error_msg[0] = '\0';
}

void set_error_msg(Status *s, char *msg, ...)
{
    assert(s != NULL);
    assert(msg != NULL);

    s->is_error = TRUE;

    va_list arg;
    va_start (arg, msg);
    vsnprintf (s->error_msg, MAX_ERROR_MSG_LEN, msg, arg);
    va_end (arg);
}

void copy_status(Status *dst, Status *src)
{
    assert(dst != NULL);
    assert(src != NULL);

    dst->is_error = src->is_error;
    strncpy(dst->error_msg, src->error_msg, MAX_ERROR_MSG_LEN);
}

void _print_status(FILE *fp, Status *s)
{
    assert(s != NULL);
    assert(fp != NULL);

    fprintf(fp, "Status {\n");
    fprintf(fp, "  is_error == %d\n", s->is_error);
    fprintf(fp, "  error_msg == '%s'\n", s->error_msg);
    fprintf(fp, "}");
}
