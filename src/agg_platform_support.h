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
