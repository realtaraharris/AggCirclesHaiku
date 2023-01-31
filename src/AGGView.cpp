#include <Bitmap.h>
#include <Window.h>
#include "util/agg_color_conv_rgb8.h"

#include "AGGView.h"
#include "Util.h"

enum input_flag_e {
    mouse_left  = 1,
    mouse_right = 2,
};

AGGView::AGGView(BRect frame, agg::platform_support* agg, agg::pix_format_e format, bool flipY) : BView(frame, "AGG View", B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW),
    fFormat(format),
    fFlipY(flipY),

    fAGG(agg),

    fMouseButtons(0),
    fMouseX(-1),
    fMouseY(-1),

    fRedraw(true),

    fPulse(NULL),
    fLastPulse(0),
    fEnableTicks(true) {
    SetViewColor(B_TRANSPARENT_32_BIT);

    frame.OffsetTo(0.0, 0.0);
    fBitmap = new BBitmap(frame, 0, pix_format_to_color_space(fFormat));
    if (fBitmap->IsValid()) {
        attach_buffer_to_BBitmap(fAGG->rbuf_window(), fBitmap, fFlipY);
    } else {
        delete fBitmap;
        fBitmap = NULL;
    }
}

AGGView::~AGGView() {
    delete fBitmap;
    delete fPulse;
}

void AGGView::AttachedToWindow() {
    BMessage message('tick');
    BMessenger target(this, Looper());
    delete fPulse;
    // BScreen screen;
    // TODO: calc screen retrace
    fPulse = new BMessageRunner(target, &message, 40000);

    // make sure we call this once
    fAGG->on_resize(Bounds().IntegerWidth() + 1,
                    Bounds().IntegerHeight() + 1);
    MakeFocus();
}

void AGGView::DetachedFromWindow() {
    delete fPulse;
    fPulse = NULL;
}

void AGGView::MessageReceived(BMessage* message) {
    bigtime_t now = system_time();
    switch (message->what) {
    case 'tick':
        // drop messages that have piled up
        if (/*now - fLastPulse > 30000*/fEnableTicks) {
            fLastPulse = now;
            fAGG->on_idle();
            Window()->PostMessage('entk', this);
            fEnableTicks = false;
        } else {
            // printf("dropping tick message (%lld)\n", now - fLastPulse);
        }
        break;
    case 'entk':
        fEnableTicks = true;
        if (now - fLastPulse > 30000) {
            fLastPulse = now;
            fAGG->on_idle();
        }
        break;
    default:
        BView::MessageReceived(message);
        break;
    }
}

void AGGView::Draw(BRect updateRect) {
    if (fBitmap) {
        if (fRedraw) {
            fAGG->on_draw();
            fRedraw = false;
        }
        if (fFormat == agg::pix_format_bgra32) {
            DrawBitmap(fBitmap, updateRect, updateRect);
        } else {
            BBitmap* bitmap = new BBitmap(fBitmap->Bounds(), 0, B_RGBA32);

            agg::rendering_buffer rbufSrc;
            attach_buffer_to_BBitmap(rbufSrc, fBitmap, false);

            agg::rendering_buffer rbufDst;
            attach_buffer_to_BBitmap(rbufDst, bitmap, false);

            switch(fFormat) {
            case agg::pix_format_rgb555:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_rgb555_to_bgra32());
                break;
            case agg::pix_format_rgb565:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_rgb565_to_bgra32());
                break;
            case agg::pix_format_rgb24:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_rgb24_to_bgra32());
                break;
            case agg::pix_format_bgr24:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_bgr24_to_bgra32());
                break;
            case agg::pix_format_rgba32:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_rgba32_to_bgra32());
                break;
            case agg::pix_format_argb32:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_argb32_to_bgra32());
                break;
            case agg::pix_format_abgr32:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_abgr32_to_bgra32());
                break;
            case agg::pix_format_bgra32:
                agg::color_conv(&rbufDst, &rbufSrc, agg::color_conv_bgra32_to_bgra32());
                break;
            }
            DrawBitmap(bitmap, updateRect, updateRect);
            delete bitmap;
        }
    } else {
        FillRect(updateRect);
    }
}

void AGGView::FrameResized(float width, float height) {
    BRect r(0.0, 0.0, width, height);
    BBitmap* bitmap = new BBitmap(r, 0, pix_format_to_color_space(fFormat));
    if (bitmap->IsValid()) {
        delete fBitmap;
        fBitmap = bitmap;
        attach_buffer_to_BBitmap(fAGG->rbuf_window(), fBitmap, fFlipY);

        fAGG->trans_affine_resizing((int)width + 1,
                                    (int)height + 1);

        // pass the event on to AGG
        fAGG->on_resize((int)width + 1, (int)height + 1);

        fRedraw = true;
        Invalidate();
    } else {
        delete bitmap;
    }
}

void AGGView::MouseDown(BPoint where) {
    BMessage* currentMessage = Window()->CurrentMessage();
    if (currentMessage) {
        if (currentMessage->FindInt32("buttons", (int32*)&fMouseButtons) < B_OK) {
            fMouseButtons = B_PRIMARY_MOUSE_BUTTON;
        }
    } else {
        fMouseButtons = B_PRIMARY_MOUSE_BUTTON;
    }

    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fMouseButtons == B_PRIMARY_MOUSE_BUTTON) {
        // left mouse button -> see if to handle in controls
        fAGG->m_ctrls.set_cur(fMouseX, fMouseY);
        if (fAGG->m_ctrls.on_mouse_button_down(fMouseX, fMouseY)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        } else {
            if (fAGG->m_ctrls.in_rect(fMouseX, fMouseY)) {
                if (fAGG->m_ctrls.set_cur(fMouseX, fMouseY)) {
                    fAGG->on_ctrl_change();
                    fAGG->force_redraw();
                }
            } else {
                fAGG->on_mouse_button_down(fMouseX, fMouseY, GetMouseFlags());
            }
        }
    } else if (fMouseButtons & B_SECONDARY_MOUSE_BUTTON) {
        // right mouse button -> simple
        fAGG->on_mouse_button_down(fMouseX, fMouseY, GetMouseFlags());
    }
    SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}

void AGGView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMesage) {
    // workarround missed mouse up events
    // (if we react too slowly, app_server might have dropped events)
    BMessage* currentMessage = Window()->CurrentMessage();
    int32 buttons = 0;
    if (currentMessage->FindInt32("buttons", &buttons) < B_OK) {
        buttons = 0;
    }
    if (!buttons) {
        MouseUp(where);
    }

    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fAGG->m_ctrls.on_mouse_move(fMouseX, fMouseY,
                                    (GetMouseFlags() & mouse_left) != 0)) {
        fAGG->on_ctrl_change();
        fAGG->force_redraw();
    } else {
        if (!fAGG->m_ctrls.in_rect(fMouseX, fMouseY)) {
            fAGG->on_mouse_move(fMouseX, fMouseY, GetMouseFlags());
        }
    }
}

void AGGView::MouseUp(BPoint where) {
    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fMouseButtons == B_PRIMARY_MOUSE_BUTTON) {
        fMouseButtons = 0;

        if (fAGG->m_ctrls.on_mouse_button_up(fMouseX, fMouseY)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        }
        fAGG->on_mouse_button_up(fMouseX, fMouseY, GetMouseFlags());
    } else if (fMouseButtons == B_SECONDARY_MOUSE_BUTTON) {
        fMouseButtons = 0;

        fAGG->on_mouse_button_up(fMouseX, fMouseY, GetMouseFlags());
    }
}

BBitmap* AGGView::Bitmap() const {
    return fBitmap;
}

uint32 AGGView::MouseButtons() {
    uint32 buttons = 0;
    if (LockLooper()) {
        buttons = fMouseButtons;
        UnlockLooper();
    }
    return buttons;
}

void AGGView::Update() {
    // trigger display update
    if (LockLooper()) {
        Invalidate();
        UnlockLooper();
    }
}

void AGGView::ForceRedraw() {
    // force a redraw (fRedraw = true;)
    // and trigger display update
    if (LockLooper()) {
        fRedraw = true;
        Invalidate();
        UnlockLooper();
    }
}

unsigned AGGView::GetMouseFlags() {
    uint32 buttons = fMouseButtons;
    uint32 mods = modifiers();
    unsigned flags = 0;
    if (buttons & B_PRIMARY_MOUSE_BUTTON) {
        flags |= mouse_left;
    }
    if (buttons & B_SECONDARY_MOUSE_BUTTON) {
        flags |= mouse_right;
    }

    return flags;
}
