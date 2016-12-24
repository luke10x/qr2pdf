#include <cairo.h>
#include <cairo-pdf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "svg.h"
#include "filename.h"


#define A4_HEIGHT_MM 297
#define A4_WIDTH_MM  210
#define A4_HEIGHT_PT 842
#define A4_WIDTH_PT  595

cairo_surface_t *create_surface_a4_pdf(char *filename)
{
    return cairo_pdf_surface_create(filename, A4_WIDTH_PT, A4_HEIGHT_PT);
}

int draw_qr(cairo_t * cr, char *text, float x, float y, int size)
{
    double MM = A4_HEIGHT_PT / (float) A4_HEIGHT_MM;

    cairo_surface_t *surf = create_qr_surface(text, size);
    cairo_set_source_surface(cr, surf, x * MM, y * MM);
    cairo_set_source_surface(cr, surf, x * MM, y * MM);
    cairo_paint(cr);
    cairo_surface_destroy(surf);
}

int main(int argc, char *argv[])
{
    char *path = create_output_path("output");
    printf("%s\n", path);

    cairo_surface_t *surface = create_surface_a4_pdf(path);
    cairo_t *cr = cairo_create(surface);

    draw_qr(cr, "first", 0, 10, 70);
    draw_qr(cr, "hello", 70, 10, 70);
    draw_qr(cr, "another message", 140, 10, 70);
    cairo_show_page(cr);

    draw_qr(cr, " message", 140, 90, 70);

    cairo_surface_finish(surface);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
}
