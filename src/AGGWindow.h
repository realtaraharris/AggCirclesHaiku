#ifndef AGGWINDOW_H
#define AGGWINDOW_H

#include "AGGView.h"
#include <Window.h>

class AGGWindow : public BWindow {
public:
    AGGWindow(void);
    virtual bool QuitRequested();
    bool Init(BRect frame, agg::PlatformSupport* agg, agg::pix_format_e format, bool flipY, uint32 flags);
    AGGView* View() const;
private:
    AGGView* fView;
};

#endif // AGGWINDOW_H
