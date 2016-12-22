#include <cairo.h>
#include <cairo-pdf.h>
#include <librsvg/rsvg.h>
#include <qrencode.h>
#include <stdlib.h>
#include <string.h>

#define A4_HEIGHT_MM 297
#define A4_WIDTH_MM  210
#define A4_HEIGHT_PT 842
#define A4_WIDTH_PT  595

static cairo_surface_t *cairo_image_surface_create_from_svg(const gchar * svg)
{
    cairo_surface_t *surface;
    cairo_t *cr;
    RsvgHandle *handle;
    RsvgDimensionData dimensions;

    GError *err = NULL;
    handle = rsvg_handle_new_from_data(svg, strlen(svg), &err);
    if (!handle) {
        fprintf(stderr, "Failed to init GConf: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }
    rsvg_handle_get_dimensions(handle, &dimensions);
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                         dimensions.width, dimensions.height);
    cr = cairo_create(surface);
    gboolean rc = rsvg_handle_render_cairo(handle, cr);
    if (!rc) {
        fprintf(stderr, "Render SVG went wrong\n");
    }
    cairo_destroy(cr);
    return surface;
}

char *qrcode2svg(const QRcode * qrcode, int size)
{
    unsigned char *row, *p;
    int x, y;

    char dots[9000] = "";
    char *buf = malloc(9000);

    p = qrcode->data;
    for (y = 0; y < qrcode->width; y++) {
        row = (p + (y * qrcode->width));
        for (x = 0; x < qrcode->width; x++) {
            char dot[10];
            if (*(row + x) & 0x1) {
                snprintf(dot, sizeof dot, "M%d,%dh1", x, y);
                strncat(dots, dot, sizeof dots);
            }
        }
    }

    int margin = 3;
    int symwidth = qrcode->width - 1 + margin * 2;
    /* *INDENT-OFF* */
	const char *tpl =
        "<?xml version=\"1.0\""\
        " encoding=\"UTF-8\""\
        " standalone=\"yes\"?>\n"\
        "<svg width=\"%dmm\""\
        " height=\"%dmm\""\
        " viewBox=\"%d %d %d %d\""\
        " preserveAspectRatio=\"none\""\
        " version=\"1.1\""\
        " xmlns=\"http://www.w3.org/2000/svg\">\n"
        "\t<g id=\"QRcode\">\n"\
        "\t\t<path style=\"stroke:#000000\""\
        " transform=\"translate(%d,%d.5)\""\
        " d=\"%s\"/>\n"\
        "\t</g>\n"\
        "</svg>\n";
    /* *INDENT-ON* */
    snprintf(buf, 9000, tpl, margin, margin, symwidth, symwidth, margin, margin,
             dots);

    return buf;
}

cairo_surface_t *create_surface_a4_pdf(char *filename)
{
    cairo_pdf_surface_create(filename, A4_WIDTH_PT, A4_HEIGHT_PT);
}

QRcode *char2qrcode(char *text)
{
    return QRcode_encodeString(text, 0, QR_ECLEVEL_L, QR_MODE_8, 0);
}

cairo_surface_t *create_qr_surface(char *text, int size)
{
    QRcode *qrcode = char2qrcode(text);
    char *svg = qrcode2svg(qrcode, size);

    /* For debugging */
    printf("%s", svg);

    return cairo_image_surface_create_from_svg(svg);
}

int draw_qr(cairo_t * cr, char *text, float x, float y, int size)
{
    double MM = A4_HEIGHT_PT / (float) A4_HEIGHT_MM;

    cairo_surface_t *surf = create_qr_surface(text, size);
    cairo_set_source_surface(cr, surf, x * MM, y * MM);
    cairo_paint(cr);
    cairo_surface_destroy(surf);
}

int main(int argc, char *argv[])
{
    cairo_surface_t *surface = create_surface_a4_pdf("output.pdf");
    cairo_t *cr = cairo_create(surface);

    draw_qr(cr, "first", 0, 10, 70);
    draw_qr(cr, "hello", 70, 10, 70);
    draw_qr(cr, "another message", 140, 10, 70);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
}
