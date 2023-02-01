#ifndef PLATFORMSPECIFIC_H
#define PLATFORMSPECIFIC_H

#include "AGGApplication.h"
#include <Application.h>

namespace agg {
    class PlatformSpecific {
    public:
        PlatformSpecific(agg::platform_support* agg, agg::pix_format_e format, bool flip_y);
        ~PlatformSpecific();
        bool Init(int width, int height, unsigned flags);
        int Run();
        void SetTitle(const char* title);
        void StartTimer();
        double ElapsedTime() const;
        void ForceRedraw();
        void UpdateWindow();

        agg::platform_support* fAGG;
        AGGApplication* fApp;
        agg::pix_format_e fFormat;
        bool fFlipY;
        bigtime_t fTimerStart;
        BBitmap* fImages[agg::platform_support::max_images];

        char fAppPath[B_PATH_NAME_LENGTH];
        char fFilePath[B_PATH_NAME_LENGTH];
    };
}

#endif // PLATFORMSPECIFIC_H
