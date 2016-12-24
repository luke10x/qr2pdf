#ifndef doci_h__

#define doci_h__

#include <cairo.h>

#define A4_HEIGHT_MM 297
#define A4_WIDTH_MM  210
#define A4_HEIGHT_PT 842
#define A4_WIDTH_PT  595

typedef struct {
   cairo_surface_t *surface;
   cairo_t *cr;
   int width; 
   int height; 
   int margin; 
   int gap; 
   int size; 
} q2p_doc_t;

q2p_doc_t *q2p_doc_create();

int *q2p_doc_write(q2p_doc_t *self, char *text);

int *q2p_doc_close(q2p_doc_t *self);

#endif
