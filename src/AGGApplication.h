#ifndef AGGAPPLICATION_H
#define AGGAPPLICATION_H

#include "AGGWindow.h"
#include "AGGView.h"
#include <Application.h>

class AGGApplication : public BApplication {
public:
    AGGApplication();
    virtual void ReadyToRun();
    virtual bool Init(agg::platform_support* agg, int width, int height, agg::pix_format_e format, bool flipY, uint32 flags);
    AGGWindow* Window() const;

private:
    AGGWindow* fWindow;
};

#endif // AGGAPPLICATION_H
