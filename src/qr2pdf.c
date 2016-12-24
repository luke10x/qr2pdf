#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "filename.h"
#include "doc.h"

int main(int argc, char *argv[])
{
    char *path = create_output_path("output");
    printf("%s\n", path);

    q2p_doc_t *doc = q2p_doc_create();
    q2p_doc_write(doc, "kolkas nieko");
    q2p_doc_close(doc);

    return 0;
}
