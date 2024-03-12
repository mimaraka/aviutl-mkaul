#include "button_label.hpp"



namespace mkaul {
	namespace ui {
		// ボタンを作成(ラベル)
		HWND LabelButton::create(
			HINSTANCE hinst,
			HWND hwnd_parent,
			int id,
			const std::string& label,
			const graphics::Font font,
			const ColorF* p_color_bg,
			const ColorF* p_color_control,
			const ColorF* p_color_label,
			const std::string& tooltip_label,
			const WindowRectangle& rect,
			const WindowRectangle& padding,
			flag::RoundEdge round_edge,
			float round_radius,
			float hover_highlight
		) noexcept {
			label_ = label;
			font_ = font;
			p_color_label_ = const_cast<ColorF*>(p_color_label);

			return Button::create(
				hinst,
				hwnd_parent,
				id,
				p_color_bg,
				p_color_control,
				tooltip_label,
				rect,
				padding,
				round_edge,
				round_radius,
				hover_highlight
			);
		}


		void LabelButton::set_label(const std::string& label) noexcept {
			label_ = label;
			redraw();
		}


		LRESULT LabelButton::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);

			switch (message) {
			case WM_PAINT:
			{
				ColorF color = *p_color_control_;

				if (clicked_)
					color.change_brightness(-hover_highlight_);
				else if (hovered_)
					color.change_brightness(hover_highlight_);

				p_graphics_->begin_draw();
				p_graphics_->fill_background(color);
				p_graphics_->draw_text(
					label_,
					Rectangle<float>{
					0.f, 0.f,
						(float)rect_wnd.right,
						(float)rect_wnd.bottom
				},
					font_,
					graphics::AnchorPosition{},
					true,
					* p_color_label_
				);
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
	}
}