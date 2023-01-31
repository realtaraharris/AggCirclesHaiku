#ifndef AGG_H
#define AGG_H

#include "AGGApplication.h"
#include <Application.h>
#include "platform/agg_platform_support.h"

namespace agg {
    class platform_specific {
    public:
        platform_specific(agg::platform_support* agg, agg::pix_format_e format, bool flip_y);
        ~platform_specific();
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

#endif // AGG_H
