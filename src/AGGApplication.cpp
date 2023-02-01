#include "AGGWindow.h"
#include "AGGView.h"
#include "AGGApplication.h"

AGGApplication::AGGApplication() : BApplication("application/x-vnd.AGG-AGG") {
    fWindow = new AGGWindow();
}

void AGGApplication::ReadyToRun() {
    if (fWindow) {
        fWindow->Show();
    }
}

bool AGGApplication::Init(PlatformSupport* agg, int width, int height, pix_format_e format, bool flipY, uint32 flags) {
    BRect r(50.0, 50.0, 50.0 + width - 1.0, 50.0 + height - 1.0);
    uint32 windowFlags = B_ASYNCHRONOUS_CONTROLS;
    if (!(flags & window_resize)) {
        windowFlags |= B_NOT_RESIZABLE;
    }
    return fWindow->Init(r, agg, format, flipY, windowFlags);
}

AGGWindow* AGGApplication::Window() const {
    return fWindow;
}
