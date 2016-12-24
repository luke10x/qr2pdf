#include <cairo.h>
#include <cairo-pdf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "svg.h"
#include "doc.h"

int draw_qr(cairo_t * cr, char *text, float x, float y, int size)
{
    double MM = A4_HEIGHT_PT / (float) A4_HEIGHT_MM;

    cairo_surface_t *surf = create_qr_surface(text, size);
    cairo_set_source_surface(cr, surf, x * MM, y * MM);
    cairo_set_source_surface(cr, surf, x * MM, y * MM);
    cairo_paint(cr);
    cairo_surface_destroy(surf);
}

q2p_doc_t *q2p_doc_create()
{
    q2p_doc_t *self;
    self = malloc(sizeof(q2p_doc_t));

    self->surface = cairo_pdf_surface_create("output.pdf", A4_WIDTH_PT, A4_HEIGHT_PT);
    self->cr = cairo_create(self->surface);
}

int *q2p_doc_write(q2p_doc_t *self, char *text)
{
    cairo_t *cr = self->cr;
    draw_qr(cr, "first", 0, 10, 70);
    /*draw_qr(cr, "hello", 70, 10, 70);*/
    draw_qr(cr, "another message", 140, 10, 70);
    cairo_show_page(cr);

    draw_qr(cr, " message", 140, 90, 70);
}

int *q2p_doc_close(q2p_doc_t *self)
{
    cairo_t *cr = self->cr;
    cairo_surface_t *surface = self->surface;

    cairo_surface_finish(surface);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    free(self);
}
