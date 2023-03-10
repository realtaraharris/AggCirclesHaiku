#ifndef UTIL_H
#define UTIL_H

#include <Bitmap.h>
#include <Path.h>
#include <Roster.h>

#include "platform/agg_platform_support.h"

color_space pix_format_to_color_space(agg::pix_format_e format);
void attach_buffer_to_BBitmap(agg::rendering_buffer& buffer, BBitmap* bitmap, bool flipY);

#endif // UTIL_H
