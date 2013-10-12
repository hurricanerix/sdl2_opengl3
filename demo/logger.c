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


FILE *log_fp;

char LOG_STR_DEBUG[] = "DEBUG";
char LOG_STR_INFO[] = "INFO";
char LOG_STR_WARNING[] = "WARNING";
char LOG_STR_ERROR[] = "ERROR";

void init_logger(FILE *fp)
{
    log_fp = fp;
}

void _log_message(char *type, char *file, int line, char *msg, ...)
{
    assert(type != NULL);
    assert(file != NULL);
    assert(msg != NULL);

    va_list arg;
    //int done;

    va_start (arg, msg);
    fprintf(log_fp, "%s %d %s %d: ", type, (int)time(NULL), file, line);

    //done = vfprintf (log_fp, msg, arg);
    vfprintf (log_fp, msg, arg);
    fprintf(log_fp, "\n");
    va_end (arg);
}
