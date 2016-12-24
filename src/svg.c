#include <cairo.h>
#include <cairo-pdf.h>
#include <librsvg/rsvg.h>
#include <qrencode.h>
#include <stdlib.h>
#include <string.h>

QRcode *char2qrcode(char *text)
{
    return QRcode_encodeString(text, 0, QR_ECLEVEL_L, QR_MODE_8, 0);
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
    snprintf(buf, 9000, tpl, size, size, margin, margin, symwidth, symwidth,
             margin, margin, dots);

    return buf;
}

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

cairo_surface_t *create_qr_surface(char *text, int size)
{
    QRcode *qrcode = char2qrcode(text);
    char *svg = qrcode2svg(qrcode, size);

    /* For debugging */
    /*printf("%s", svg); */

    return cairo_image_surface_create_from_svg(svg);
}
