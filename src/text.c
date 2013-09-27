#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "text.h"

char *textFileRead(char *fn)
{
    assert(fn != NULL);
    log_debug("textFileRead {");
    log_debug("  -in- fn - %s", fn);

    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }

    log_debug("textFileRead }");
    return content;
}
