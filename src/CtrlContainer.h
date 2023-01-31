#ifndef _CTRLCONTAINER_H_
#define _CTRLCONTAINER_H_

#include "ctrl/agg_ctrl.h"

namespace agg {
    // ctrl_container
    // A helper class that contains pointers to a number of controls.
    // This class is used to ease the event handling with controls.
    // The implementation should simply call the appropriate methods
    // of this class when appropriate events occur.
    class ctrl_container {
        enum max_ctrl_e { max_ctrl = 64 };

    public:
        ctrl_container() : m_num_ctrl(0), m_cur_ctrl(-1) {}

        void add(ctrl& c) {
            if(m_num_ctrl < max_ctrl) {
                m_ctrl[m_num_ctrl++] = &c;
            }
        }

        bool in_rect(double x, double y) {
            unsigned i;
            for(i = 0; i < m_num_ctrl; i++) {
                if(m_ctrl[i]->in_rect(x, y)) return true;
            }
            return false;
        }

        bool on_mouse_button_down(double x, double y) {
            unsigned i;
            for(i = 0; i < m_num_ctrl; i++) {
                if(m_ctrl[i]->on_mouse_button_down(x, y)) return true;
            }
            return false;
        }

        bool on_mouse_button_up(double x, double y) {
            unsigned i;
            bool flag = false;
            for(i = 0; i < m_num_ctrl; i++) {
                if(m_ctrl[i]->on_mouse_button_up(x, y)) flag = true;
            }
            return flag;
        }

        bool on_mouse_move(double x, double y, bool button_flag) {
            unsigned i;
            for(i = 0; i < m_num_ctrl; i++)
            {
                if(m_ctrl[i]->on_mouse_move(x, y, button_flag)) return true;
            }
            return false;
        }

        bool set_cur(double x, double y) {
            unsigned i;
            for(i = 0; i < m_num_ctrl; i++) {
                if(m_ctrl[i]->in_rect(x, y)) {
                    if(m_cur_ctrl != int(i)) {
                        m_cur_ctrl = i;
                        return true;
                    }
                    return false;
                }
            }
            if(m_cur_ctrl != -1) {
                m_cur_ctrl = -1;
                return true;
            }
            return false;
        }

    private:
        ctrl*         m_ctrl[max_ctrl];
        unsigned      m_num_ctrl;
        int           m_cur_ctrl;
    };
}

#endif // _CTRLCONTAINER_H_
