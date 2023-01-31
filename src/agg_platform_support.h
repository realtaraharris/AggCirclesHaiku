// class platform_support
//
// It's not a part of the AGG library, it's just a helper class to create 
// interactive demo examples. Since the examples should not be too complex
// this class is provided to support some very basic interactive graphical
// funtionality, such as putting the rendered image to the window, simple 
// mouse input, window resizing, setting the window title, and catching
// the "idle" events.
// 
// The idea is to have a single header file that does not depend on any 
// platform (I hate these endless #ifdef/#elif/#elif.../#endif) and a number
// of different implementations depending on the concrete platform. 
// The most popular platforms are:
//
// Windows-32 API
// X-Window API
// SDL library (see http://www.libsdl.org/)
// MacOS C/C++ API
// 
// This file does not include any system dependent .h files such as
// windows.h or X11.h, so, your demo applications do not depend on the
// platform. The only file that can #include system dependend headers
// is the implementation file agg_platform_support.cpp. Different
// implementations are placed in different directories, such as
// ~/agg/src/platform/win32
// ~/agg/src/platform/sdl
// ~/agg/src/platform/X11
// and so on.
//
// All the system dependent stuff sits in the platform_specific 
// class which is forward-declared here but not defined. 
// The platform_support class has just a pointer to it and it's 
// the responsibility of the implementation to create/delete it.
// This class being defined in the implementation file can have 
// any platform dependent stuff such as HWND, X11 Window and so on.

#ifndef AGG_PLATFORM_SUPPORT_INCLUDED
#define AGG_PLATFORM_SUPPORT_INCLUDED

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_trans_viewport.h"
#include "ctrl/agg_ctrl.h"

#include "CtrlContainer.h"

namespace agg {
    // These are flags used in method init(). Not all of them are
    // applicable on different platforms, for example the win32_api
    // cannot use a hardware buffer (window_hw_buffer).
    // The implementation should simply ignore unsupported flags.
    enum window_flag_e {
        window_resize            = 1,
        window_hw_buffer         = 2,
        window_keep_aspect_ratio = 4,
    };

    // Possible formats of the rendering buffer. Initially I thought that it's
    // reasonable to create the buffer and the rendering functions in 
    // accordance with the native pixel format of the system because it 
    // would have no overhead for pixel format conersion. 
    // But eventually I came to a conclusion that having a possibility to 
    // convert pixel formats on demand is a good idea. First, it was X11 where 
    // there lots of different formats and visuals and it would be great to 
    // render everything in, say, RGB-24 and display it automatically without
    // any additional efforts. The second reason is to have a possibility to 
    // debug renderers for different pixel formats and colorspaces having only 
    // one computer and one system.
    //
    // This stuff is not included into the basic AGG functionality because the 
    // number of supported pixel formats (and/or colorspaces) can be great and 
    // if one needs to add new format it would be good only to add new 
    // rendering files without having to modify any existing ones (a general 
    // principle of incapsulation and isolation).
    //
    // Using a particular pixel format doesn't obligatory mean the necessity
    // of software conversion. For example, win32 API can natively display 
    // gray8, 15-bit RGB, 24-bit BGR, and 32-bit BGRA formats. 
    // This list can be (and will be!) extended in future.
    enum pix_format_e {
        pix_format_undefined = 0,  // By default. No conversions are applied 
        pix_format_bw,             // 1 bit per color B/W
        pix_format_gray8,          // Simple 256 level grayscale
        pix_format_gray16,         // Simple 65535 level grayscale
        pix_format_rgb555,         // 15 bit rgb. Depends on the byte ordering!
        pix_format_rgb565,         // 16 bit rgb. Depends on the byte ordering!
        pix_format_rgbAAA,         // 30 bit rgb. Depends on the byte ordering!
        pix_format_rgbBBA,         // 32 bit rgb. Depends on the byte ordering!
        pix_format_bgrAAA,         // 30 bit bgr. Depends on the byte ordering!
        pix_format_bgrABB,         // 32 bit bgr. Depends on the byte ordering!
        pix_format_rgb24,          // R-G-B, one byte per color component
        pix_format_bgr24,          // B-G-R, native win32 BMP format.
        pix_format_rgba32,         // R-G-B-A, one byte per color component
        pix_format_argb32,         // A-R-G-B, native MAC format
        pix_format_abgr32,         // A-B-G-R, one byte per color component
        pix_format_bgra32,         // B-G-R-A, native win32 BMP format
        pix_format_rgb48,          // R-G-B, 16 bits per color component
        pix_format_bgr48,          // B-G-R, native win32 BMP format.
        pix_format_rgba64,         // R-G-B-A, 16 bits byte per color component
        pix_format_argb64,         // A-R-G-B, native MAC format
        pix_format_abgr64,         // A-B-G-R, one byte per color component
        pix_format_bgra64,         // B-G-R-A, native win32 BMP format

        end_of_pix_formats
    };

	// Mouse flags. They can be different on different platforms and the ways they are
	// obtained are also different. But in any case the system dependent flags should
	// be mapped into these ones. The meaning of that is as follows. They are also used
	// in the overridden methods such as on_mouse_move(), on_mouse_button_down(),
	// on_mouse_button_dbl_click(), on_mouse_button_up(). In the method
	// on_mouse_button_up() the mouse flags have different meaning. They mean that the
	// respective button is being released, but the meaning of the flags remains the
	// same. There's absolut minimal set of flags is used because they'll be most
	// probably supported on different platforms. Even the mouse_right flag is
	// restricted because Mac mice have only one button, but AFAIK it can be simulated
	// with holding a special key on the keydoard.
    enum input_flag_e {
        mouse_left  = 1,
        mouse_right = 2,
    };

    // A predeclaration of the platform dependent class. Since we do not
    // know anything here the only we can have is just a pointer to this
    // class as a data member. It should be created and destroyed explicitly
    // in the constructor/destructor of the platform_support class. 
    // Although the pointer to platform_specific is public the application 
    // cannot have access to its members or methods since it does not know
    // anything about them and it's a perfect incapsulation :-)
    class platform_specific;
}

#endif
