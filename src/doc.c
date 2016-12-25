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

    self->surface =
        cairo_pdf_surface_create("output.pdf", A4_WIDTH_PT, A4_HEIGHT_PT);
    self->cr = cairo_create(self->surface);

    self->margin = 6;
    self->gap = 2;
    self->size = 18;
    self->lastx = -1;
    self->lasty = -1;

    return self;
}

int *q2p_doc_write(q2p_doc_t * self, char *text)
{
    int x, y;

    if (self->lastx == -1 && self->lasty == -1) {
      new_page:
        x = self->margin;
        y = self->margin;
    } else {
        x = self->lastx + self->size;
        x = x + self->gap;

        y = self->lasty;

        if (x + self->size + self->margin > A4_WIDTH_MM) {
            x = self->margin;
            y = self->lasty + self->gap + self->size;
        }

        if (y + self->size + self->margin > A4_HEIGHT_MM) {
            x = 0;
            y = 0;
            cairo_show_page(self->cr);
            goto new_page;
        }
    }

    self->lastx = x;
    self->lasty = y;

    draw_qr(self->cr, text, x, y, self->size);
}

int *q2p_doc_close(q2p_doc_t * self)
{
    cairo_t *cr = self->cr;
    cairo_surface_t *surface = self->surface;

    cairo_surface_finish(surface);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    free(self);
}
