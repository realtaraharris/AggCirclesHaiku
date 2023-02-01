#include <stdio.h>

#include "PlatformSpecific.h"
#include "Util.h"

PlatformSpecific::PlatformSpecific(PlatformSupport* agg, pix_format_e format, bool flip_y) : fAGG(agg), fApp(NULL), fFormat(format), fFlipY(flip_y), fTimerStart(system_time()) {
    memset(fImages, 0, sizeof(fImages));
    fApp = new AGGApplication();
    fAppPath[0] = 0;
    // figure out where we're running from
    app_info info;
    status_t ret = fApp->GetAppInfo(&info);
    if (ret >= B_OK) {
        BPath path(&info.ref);
        ret = path.InitCheck();
        if (ret >= B_OK) {
            ret = path.GetParent(&path);
            if (ret >= B_OK) {
                sprintf(fAppPath, "%s", path.Path());
            } else {
                fprintf(stderr, "getting app parent folder failed: %s\n", strerror(ret));
            }
        } else {
            fprintf(stderr, "making app path failed: %s\n", strerror(ret));
        }
    } else {
        fprintf(stderr, "GetAppInfo() failed: %s\n", strerror(ret));
    }
}

PlatformSpecific::~PlatformSpecific() {
    for (int32 i = 0; i < PlatformSupport::max_images; i++) {
        delete fImages[i];
    }
    delete fApp;
}

bool PlatformSpecific::Init(int width, int height, unsigned flags) {
    return fApp->Init(fAGG, width, height, fFormat, fFlipY, flags);
}

int PlatformSpecific::Run() {
    status_t ret = B_NO_INIT;
    if (fApp) {
        fApp->Run();
        ret = B_OK;
    }
    return ret;
}

void PlatformSpecific::SetTitle(const char* title) {
    if (fApp && fApp->Window() && fApp->Window()->Lock()) {
        fApp->Window()->SetTitle(title);
        fApp->Window()->Unlock();
    }
}

void PlatformSpecific::StartTimer() {
    fTimerStart = system_time();
}

double PlatformSpecific::ElapsedTime() const {
    return (system_time() - fTimerStart) / 1000.0;
}

void PlatformSpecific::ForceRedraw() {
    fApp->Window()->View()->ForceRedraw();
}

void PlatformSpecific::UpdateWindow() {
    fApp->Window()->View()->Update();
}
