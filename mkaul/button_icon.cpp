#include "button_icon.hpp"



namespace mkaul {
	namespace ui {
		LRESULT IconButton::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);

			switch (message) {
			case WM_DESTROY:
			case WM_CLOSE:
				icon_->release();
				break;

			case WM_PAINT:
			{
				ColorF color = *p_color_control_;

				if (clicked_)
					color.change_brightness(-hover_highlight_);
				else if (hovered_)
					color.change_brightness(hover_highlight_);

				p_graphics_->begin_draw();
				p_graphics_->fill_background(color);
				if (icon_.get()) {
					p_graphics_->draw_bitmap(
						icon_.get(),
						Point{ rect_wnd.right * 0.5f, rect_wnd.bottom * 0.5f }
					);
				}
				if ((uint32_t)(status_ & flag::Status::Disabled)) {
					ColorF tmp = color;
					tmp.set_a(0.5f);
					p_graphics_->fill_background(tmp);
				}
				draw_round_edge();
				p_graphics_->end_draw();

				return 0;
			}
			}

			return Button::wndproc(hwnd, message, wparam, lparam);
		}

		void IconButton::set_icon_from_filename(const std::filesystem::path& path) {
			if (icon_.get()) {
				icon_.release();
			}
			icon_ = p_graphics_->load_bitmap_from_filename(path);
		}

		void IconButton::set_icon_from_resource(HINSTANCE hinst, const char* res_name, const char* res_type) {
			if (icon_.get()) {
				icon_.release();
			}
			icon_ = p_graphics_->load_bitmap_from_resource(hinst, res_name, res_type);
		}
	}
}