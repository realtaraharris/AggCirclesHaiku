#ifndef PLATFORMSPECIFIC_H
#define PLATFORMSPECIFIC_H

#include "AGGApplication.h"
#include <Application.h>

class PlatformSpecific {
public:
    PlatformSpecific(PlatformSupport* agg, pix_format_e format, bool flip_y);
    ~PlatformSpecific();
    bool Init(int width, int height, unsigned flags);
    int Run();
    void SetTitle(const char* title);
    void StartTimer();
    double ElapsedTime() const;
    void ForceRedraw();
    void UpdateWindow();

    PlatformSupport* fAGG;
    AGGApplication* fApp;
    pix_format_e fFormat;
    bool fFlipY;
    bigtime_t fTimerStart;
    BBitmap* fImages[PlatformSupport::max_images];

    char fAppPath[B_PATH_NAME_LENGTH];
    char fFilePath[B_PATH_NAME_LENGTH];
};

#endif // PLATFORMSPECIFIC_H
