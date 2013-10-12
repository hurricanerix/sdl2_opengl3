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


void print_help(char *command);


int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_help(argv[0]);
        exit(1);
    }

    init_logger(stderr);

    Config config;
    init_config(&config);
    load_config(&config, argv[1]);
    if (config.status.is_error) {
        fprintf(stderr, "Error: %s\n", config.status.error_msg);
        return 1;
    }

    load_app(&config);
    if (config.status.is_error) {
        fprintf(stderr, "Error: %s\n", config.status.error_msg);
        return 1;
    }

    Status app_status;
    run_app(&app_status);
    if (app_status.is_error) {
        fprintf(stderr, "Error: %s\n", app_status.error_msg);
        return 1;
    }

    destroy_app();

    return 0;
}

void print_help(char *command)
{
    assert(command != NULL);

    fprintf(stderr, "%s help:\n", command);
    fprintf(stderr, "%s <config file>\n\n", command);
}
