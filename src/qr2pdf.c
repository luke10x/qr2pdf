#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "filename.h"
#include "doc.h"

#define READ_BLOCK_SIZE 2953

int main(int argc, char *argv[])
{
    char *path = create_output_path("output");
    printf("%s\n", path);

    q2p_doc_t *doc = q2p_doc_create();

    char *line = malloc(READ_BLOCK_SIZE);
    FILE *stream = fdopen(0, "r");
    while (line = fgets(line, READ_BLOCK_SIZE, stream)) {
        q2p_doc_write(doc, line);
    }
    fclose(stream);

    q2p_doc_close(doc);

    return 0;
}
