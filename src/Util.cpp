#include "Util.h"

color_space pix_format_to_color_space(agg::pix_format_e format) {
    color_space bitmapFormat = B_NO_COLOR_SPACE;
    switch (format) {
    case agg::pix_format_rgb555:
        bitmapFormat = B_RGB15;
        break;

    case agg::pix_format_rgb565:
        bitmapFormat = B_RGB16;
        break;

    case agg::pix_format_rgb24:
    case agg::pix_format_bgr24:
        bitmapFormat = B_RGB24;
        break;

    case agg::pix_format_rgba32:
    case agg::pix_format_argb32:
    case agg::pix_format_abgr32:
    case agg::pix_format_bgra32:
        bitmapFormat = B_RGBA32;
        break;
    }
    return bitmapFormat;
}

void attach_buffer_to_BBitmap(agg::rendering_buffer& buffer, BBitmap* bitmap, bool flipY) {
    uint8* bits = (uint8*)bitmap->Bits();
    uint32 width = bitmap->Bounds().IntegerWidth() + 1;
    uint32 height = bitmap->Bounds().IntegerHeight() + 1;
    int32 bpr = bitmap->BytesPerRow();
    if (flipY) {
        // XXX: why don't I have to do this?!?
        // bits += bpr * (height - 1);
        bpr = -bpr;
    }
    buffer.attach(bits, width, height, bpr);
}
