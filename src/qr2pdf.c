#include <cairo.h>
#include <cairo-pdf.h>
#include <librsvg/rsvg.h>

#define A4_HEIGHT_MM 297
#define A4_WIDTH_MM  210
#define A4_HEIGHT_PT 842
#define A4_WIDTH_PT  595

int
main (int argc, char *argv[])
{
    cairo_surface_t *surface =
        cairo_pdf_surface_create ("output.pdf", A4_WIDTH_PT, A4_HEIGHT_PT);

    cairo_t *cr =
        cairo_create (surface);
    cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 32.0);
    cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
    cairo_move_to (cr, 10.0, 50.0);
    cairo_show_text (cr, "Hello, world");
    cairo_destroy (cr);

    cairo_t *cr1 =
        cairo_create (surface);
    cairo_select_font_face (cr1, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr1, 32.0);
    cairo_set_source_rgb (cr1, 1.0, 0.0, 1.0);
    cairo_move_to (cr1, 220.0, 350.0);
    cairo_show_text (cr1, "Hello, world");
    cairo_destroy (cr1);

    GError* err = NULL;
    RsvgHandle *handle = rsvg_handle_new_from_file ("drawing.svg", &err);
    if (!handle) {
        fprintf(stderr, "Failed to init GConf: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }
    cairo_t *cr2 =
        cairo_create (surface);
    gboolean rc = rsvg_handle_render_cairo (handle, cr2);
    if (!rc) {
        fprintf(stderr, "Render went wrong\n");
    }
    cairo_destroy (cr2);

    cairo_surface_destroy (surface);
    return 0;
}
