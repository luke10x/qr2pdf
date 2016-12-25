#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "args.h"

args_t parse_opts(int argc, char *argv[])
{
    args_t o;

    o.gap = 2;
    o.margin = 6;
    o.size = 18;

    int opterr = 0;
    int c;

    while ((c = getopt(argc, argv, "g:m:s:")) != -1) {
        switch (c) {
        case 'm':
            o.margin = atoi(optarg);
            break;
        case 'g':
            o.gap = atoi(optarg);
            break;
        case 's':
            o.size = atoi(optarg);
            break;
        case '?':
            if (optopt == 's' || optopt == 'm' || optopt == 'g') {
                fprintf(stderr, "Option -%c requires an arg.\n", optopt);
            } else if (isprint(optopt)) {
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            } else {
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            }
            exit(1);
        default:
            abort();
        }
    }

    return o;
}
