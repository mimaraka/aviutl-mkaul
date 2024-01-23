#include "graphics_base.hpp"



namespace mkaul {
    namespace graphics {
        void Graphics::get_rect(mkaul::WindowRectangle* p_rect) const noexcept {
            RECT tmp;
            ::GetClientRect(hwnd_, &tmp);
            *p_rect = tmp;
        }
    }
}