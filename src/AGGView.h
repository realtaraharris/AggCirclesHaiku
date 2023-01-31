#ifndef AGGVIEW_H
#define AGGVIEW_H

#include <MessageRunner.h>
#include <View.h>
#include "agg_platform_support.h"

class AGGView : public BView {
public:
    AGGView(BRect frame, agg::platform_support* agg, agg::pix_format_e format, bool flipY);
    virtual                 ~AGGView();

    virtual void            AttachedToWindow();
    virtual void            DetachedFromWindow();

    virtual void            MessageReceived(BMessage* message);
    virtual void            Draw(BRect updateRect);
    virtual void            FrameResized(float width, float height);

    virtual void            KeyDown(const char* bytes, int32 numBytes);

    virtual void            MouseDown(BPoint where);
    virtual void            MouseMoved(BPoint where, uint32 transit, const BMessage* dragMesage);
    virtual void            MouseUp(BPoint where);

    BBitmap*        Bitmap() const;

    uint8           LastKeyDown() const;
    uint32          MouseButtons();

    void            Update();
    void            ForceRedraw();

    unsigned        GetKeyFlags();

private:
    BBitmap*                fBitmap;
    agg::pix_format_e       fFormat;
    bool                    fFlipY;

    agg::platform_support*  fAGG;

    uint32                  fMouseButtons;
    int32                   fMouseX;
    int32                   fMouseY;

    uint8                   fLastKeyDown;

    bool                    fRedraw;

    BMessageRunner*         fPulse;
    bigtime_t               fLastPulse;
    bool                    fEnableTicks;
};

#endif // AGGVIEW_H
