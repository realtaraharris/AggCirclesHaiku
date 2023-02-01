#include <Application.h>
#include "AGGWindow.h"

AGGWindow::AGGWindow() : BWindow(BRect(-50.0, -50.0, -10.0, -10.0), "AGG Circles", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS) {}

bool AGGWindow::QuitRequested() {
    be_app->PostMessage(B_QUIT_REQUESTED);
    return true;
}

bool AGGWindow::Init(BRect frame, PlatformSupport* agg, pix_format_e format, bool flipY, uint32 flags) {
    MoveTo(frame.LeftTop());
    ResizeTo(frame.Width(), frame.Height());

    SetFlags(flags);

    frame.OffsetTo(0.0, 0.0);
    fView = new AGGView(frame, agg, format, flipY);
    AddChild(fView);

    return fView->Bitmap() != NULL;
}

AGGView* AGGWindow::View() const {
    return fView;
}
