#include <stdio.h>

#include <Application.h>
#include <Alert.h>
#include <TranslationUtils.h>
#include <Bitmap.h>

#include "agg_platform_support.h"
#include "AGG.h"
#include "Util.h"
#include "util/agg_color_conv_rgb8.h"

namespace agg {
    platform_specific::platform_specific(agg::platform_support* agg, agg::pix_format_e format, bool flip_y) : fAGG(agg), fApp(NULL), fFormat(format), fFlipY(flip_y), fTimerStart(system_time()) {
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

    platform_specific::~platform_specific() {
        for (int32 i = 0; i < agg::platform_support::max_images; i++) {
            delete fImages[i];
        }
        delete fApp;
    }

    bool platform_specific::Init(int width, int height, unsigned flags) {
        return fApp->Init(fAGG, width, height, fFormat, fFlipY, flags);
    }

    int platform_specific::Run() {
        status_t ret = B_NO_INIT;
        if (fApp) {
            fApp->Run();
            ret = B_OK;
        }
        return ret;
    }

    void platform_specific::SetTitle(const char* title) {
        if (fApp && fApp->Window() && fApp->Window()->Lock()) {
            fApp->Window()->SetTitle(title);
            fApp->Window()->Unlock();
        }
    }

    void platform_specific::StartTimer() {
        fTimerStart = system_time();
    }

    double platform_specific::ElapsedTime() const {
        return (system_time() - fTimerStart) / 1000.0;
    }

    void platform_specific::ForceRedraw() {
        fApp->Window()->View()->ForceRedraw();
    }

    void platform_specific::UpdateWindow() {
        fApp->Window()->View()->Update();
    }

    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(this, format, flip_y)),
        m_format(format),
        m_bpp(32/*m_specific->m_bpp*/),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10) {
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }
    
    platform_support::~platform_support() {
        delete m_specific;
    }

    void platform_support::caption(const char* cap) {
        strcpy(m_caption, cap);
        m_specific->SetTitle(cap);
    }
    
    void platform_support::start_timer() {
        m_specific->StartTimer();
    }
    
    double platform_support::elapsed_time() const {
        return m_specific->ElapsedTime();
    }
    
    void* platform_support::raw_display_handler() {
        // TODO: if we ever support BDirectWindow here, that would
        // be the frame buffer pointer with offset to the window top left
        return NULL;
    }
    
    void platform_support::message(const char* msg) {
        BAlert* alert = new BAlert("AGG Message", msg, "Ok");
        alert->Go(/*NULL*/);
    }
    
    bool platform_support::init(unsigned width, unsigned height, unsigned flags) {
        m_initial_width = width;
        m_initial_height = height;
        m_window_flags = flags;
    
        if (m_specific->Init(width, height, flags)) {
            on_init();
            return true;
        }
    
        return false;
    }
    
    int platform_support::run() {
        return m_specific->Run();
    }
    
    const char* platform_support::img_ext() const {
        return ".ppm";
    }
    
    const char* platform_support::full_file_name(const char* file_name) {
        sprintf(m_specific->fFilePath, "%s/%s", m_specific->fAppPath, file_name);
        return m_specific->fFilePath;
    }
    
    bool platform_support::load_img(unsigned idx, const char* file) {
        if (idx < max_images) {
            char path[B_PATH_NAME_LENGTH];
            sprintf(path, "%s/%s%s", m_specific->fAppPath, file, img_ext());
            BBitmap* transBitmap = BTranslationUtils::GetBitmap(path);
            if (transBitmap && transBitmap->IsValid()) {
                if(transBitmap->ColorSpace() != B_RGB32 && transBitmap->ColorSpace() != B_RGBA32) {
                    // oops we've got a smart-ass Translator making our life harder
                    delete transBitmap;
                    return false;
                }
    
                color_space format = B_RGB24;
    
                switch (m_format) {
                case pix_format_gray8:
                    format = B_GRAY8;
                    break;
                case pix_format_rgb555:
                    format = B_RGB15;
                    break;
                case pix_format_rgb565:
                    format = B_RGB16;
                    break;
                case pix_format_rgb24:
                    format = B_RGB24_BIG;
                    break;
                case pix_format_bgr24:
                    format = B_RGB24;
                    break;
                case pix_format_abgr32:
                case pix_format_argb32:
                case pix_format_bgra32:
                    format = B_RGB32;
                    break;
                case pix_format_rgba32:
                    format = B_RGB32_BIG;
                    break;
                }
                BBitmap* bitmap = new (std::nothrow) BBitmap(transBitmap->Bounds(), 0, format);
                if (!bitmap || !bitmap->IsValid()) {
                    fprintf(stderr, "failed to allocate temporary bitmap!\n");
                    delete transBitmap;
                    delete bitmap;
                    return false;
                }
    
                delete m_specific->fImages[idx];
    
                rendering_buffer rbuf_tmp;
                attach_buffer_to_BBitmap(rbuf_tmp, transBitmap, m_flip_y);
    
                m_specific->fImages[idx] = bitmap;
    
                attach_buffer_to_BBitmap(m_rbuf_img[idx], bitmap, m_flip_y);
    
                rendering_buffer* dst = &m_rbuf_img[idx];
    
                switch(m_format) {
                case pix_format_gray8:
                    return false;
                    // color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray8()); break;
                    break;
    
                case pix_format_rgb555:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb555());
                    break;
                    break;
    
                case pix_format_rgb565:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb565());
                    break;
                    break;
    
                case pix_format_rgb24:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb24());
                    break;
                    break;
    
                case pix_format_bgr24:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr24());
                    break;
                    break;
    
                case pix_format_abgr32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr32());
                    break;
                    break;
    
                case pix_format_argb32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb32());
                    break;
                    break;
    
                case pix_format_bgra32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra32());
                    break;
                    break;
    
                case pix_format_rgba32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba32());
                    break;
                    break;
                }
                delete transBitmap;
    
                return true;
    
            } else {
                fprintf(stderr, "failed to load bitmap: '%s'\n", full_file_name(file));
            }
        }
        return false;
    }
    
    bool platform_support::save_img(unsigned idx, const char* file) {
        // TODO: implement using BTranslatorRoster and friends
        return false;
    }
    
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height) {
        if(idx < max_images) {
            if(width  == 0) {
                width  = m_specific->fApp->Window()->View()->Bitmap()->Bounds().IntegerWidth() + 1;
            }
            if(height == 0) {
                height = m_specific->fApp->Window()->View()->Bitmap()->Bounds().IntegerHeight() + 1;
            }
            BBitmap* bitmap = new BBitmap(BRect(0.0, 0.0, width - 1, height - 1), 0, B_RGBA32);;
            if (bitmap && bitmap->IsValid()) {
                delete m_specific->fImages[idx];
                m_specific->fImages[idx] = bitmap;
                attach_buffer_to_BBitmap(m_rbuf_img[idx], bitmap, m_flip_y);
                return true;
            } else {
                delete bitmap;
            }
        }
        return false;
    }
    
    void platform_support::force_redraw() {
        m_specific->ForceRedraw();
    }
    
    void platform_support::update_window() {
        m_specific->UpdateWindow();
    }
    
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}
}
