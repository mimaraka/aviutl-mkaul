#pragma once

#include "button.hpp"



namespace mkaul {
	namespace ui {
		// ボタンを作成
		HWND Button::create(
			HINSTANCE hinst,
			HWND hwnd_parent,
			int id,
			const ColorF* p_color_bg,
			const ColorF* p_color_control,
			const std::string& tooltip_label,
			const WindowRectangle& rect,
			const WindowRectangle& padding,
			flag::RoundEdge round_edge,
			float round_radius,
			float hover_highlight
		) noexcept {
			tooltip_label_ = tooltip_label;
			hover_highlight_ = hover_highlight;

			return Control::create(
				hinst,
				hwnd_parent,
				id,
				"MKAUL_BUTTON",
				NULL,
				NULL,
				p_color_bg,
				p_color_control,
				rect,
				padding,
				round_edge,
				round_radius,
				::LoadCursorA(NULL, IDC_HAND)
			);
		}


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


		// ボタンを作成(アイコン)
		HWND IconButton::create(
			HINSTANCE hinst,
			HWND hwnd_parent,
			int id,
			graphics::Bitmap* icon,
			const ColorF* p_color_bg,
			const ColorF* p_color_control,
			const std::string& tooltip_label,
			const WindowRectangle& rect,
			const WindowRectangle& padding,
			flag::RoundEdge round_edge,
			float round_radius,
			float hover_highlight
		) noexcept {
			icon_ = icon;
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


		// ウィンドウプロシージャ
		LRESULT Button::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);

			switch (message) {
			case WM_CREATE:
				p_graphics_->init(hwnd);

				if (!tooltip_label_.empty()) {
					hwnd_tooltip_ = ::CreateWindowEx(
						0, TOOLTIPS_CLASS,
						NULL, TTS_ALWAYSTIP,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						hwnd, NULL,
						(HINSTANCE)::GetWindowLongA(hwnd, GWL_HINSTANCE),
						NULL
					);

					tool_info_.cbSize = sizeof(TOOLINFO);
					tool_info_.uFlags = TTF_SUBCLASS;
					tool_info_.hwnd = hwnd;
					tool_info_.uId = id_;
					tool_info_.lpszText = (LPTSTR)tooltip_label_.c_str();
					::SendMessage(hwnd_tooltip_, TTM_ADDTOOL, 0, (LPARAM)&tool_info_);
				}

				tme_.cbSize = sizeof(tme_);
				tme_.dwFlags = TME_LEAVE;
				tme_.hwndTrack = hwnd;
				break;

			case WM_DESTROY:
			case WM_CLOSE:
				p_graphics_->release();
				break;

			case WM_SIZE:
				p_graphics_->resize();

				if (!tooltip_label_.empty()) {
					tool_info_.rect = rect_wnd;
					::SendMessage(hwnd_tooltip_, TTM_NEWTOOLRECT, 0, (LPARAM)&tool_info_);
				}
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
				draw_round_edge();
				p_graphics_->end_draw();

				break;
			}

			// マウスが動いたとき
			case WM_MOUSEMOVE:
				if ((bool)!(status_ & flag::Status::Disabled)) {
					hovered_ = true;
					redraw();
					::TrackMouseEvent(&tme_);
				}
				break;

			// 左クリックがされたとき
			case WM_LBUTTONDOWN:
				if ((bool)!(status_ & flag::Status::Disabled)) {
					clicked_ = true;
					redraw();
					::TrackMouseEvent(&tme_);
				}
				break;

			// 左クリックが終わったとき
			case WM_LBUTTONUP:
				if ((bool)!(status_ & flag::Status::Disabled) and clicked_) {
					clicked_ = false;
					::SendMessage(hwnd_parent_, WM_COMMAND, id_, 0);
					redraw();
				}
				break;

			// マウスがウィンドウから離れたとき
			case WM_MOUSELEAVE:
				clicked_ = false;
				hovered_ = false;
				redraw();
				break;

			default:
				return ::DefWindowProc(hwnd, message, wparam, lparam);
			}

			return 0;
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
					*p_color_label_
				);
				draw_round_edge();
				p_graphics_->end_draw();

				return 0;
			}
			}

			return Button::wndproc(hwnd, message, wparam, lparam);
		}


		LRESULT IconButton::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
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
				p_graphics_->draw_bitmap(
					icon_,
					Point{rect_wnd.right * 0.5f, rect_wnd.bottom * 0.5f}
				);
				draw_round_edge();
				p_graphics_->end_draw();

				return 0;
			}

			case WM_CLOSE:
				icon_->release();
				break;
			}

			return Button::wndproc(hwnd, message, wparam, lparam);
		}
	}
}