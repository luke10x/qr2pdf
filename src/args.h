#ifndef q2p_args_h__

#define q2p_args_h__

typedef struct {
    int margin;
    int gap;
    int size;
} args_t;

args_t parse_opts(int argc, char *argv[]);

#endif
