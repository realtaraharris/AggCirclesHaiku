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
