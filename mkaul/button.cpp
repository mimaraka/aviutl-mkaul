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
			const WindowRectangle& rect,
			const std::string& tooltip_label,
			flag::RoundEdge round_edge,
			float round_radius,
			float hover_highlight
		)
		{
			tooltip_label_ = tooltip_label;
			hover_highlight_ = hover_highlight;

			return Control::create(
				hinst,
				hwnd_parent,
				id,
				"MKAUL_BUTTON",
				NULL,
				NULL,
				rect,
				p_color_bg,
				p_color_control,
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
			const WindowRectangle& rect,
			const std::string& tooltip_label,
			flag::RoundEdge round_edge,
			float round_radius,
			float hover_highlight
		)
		{
			label_ = label;
			font_ = font;
			p_color_label_ = const_cast<ColorF*>(p_color_label);

			return Button::create(
				hinst,
				hwnd_parent,
				id,
				p_color_bg,
				p_color_control,
				rect,
				tooltip_label,
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
			const char* icon_source,
			SourceType source_type,
			const ColorF* p_color_bg,
			const ColorF* p_color_control,
			const WindowRectangle& rect,
			const std::string& tooltip_label,
			flag::RoundEdge round_edge,
			float round_radius,
			float hover_highlight
		)
		{
			source_type_ = source_type;

			// ソースの種類で場合分け
			switch (source_type) {
			// ファイル
			case SourceType::File:
				icon_source_ = icon_source;
				break;

			// リソース
			case SourceType::Resource:
				// リソースが数値の場合
				if (!HIWORD(icon_source))
					icon_resource_num_ = LOWORD(icon_source);
				// リソースが文字列の場合
				else
					icon_source_ = icon_source;
				break;

			default:
				return NULL;
			}

			return Button::create(
				hinst,
				hwnd_parent,
				id,
				p_color_bg,
				p_color_control,
				rect,
				tooltip_label,
				round_edge,
				round_radius,
				hover_highlight
			);
		}


		void LabelButton::set_label(const std::string& label) noexcept
		{
			label_ = label;
			redraw();
		}


		// ウィンドウプロシージャ
		LRESULT Button::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
		{
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

			case WM_CLOSE:
				p_graphics_->exit();
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


		LRESULT LabelButton::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
		{
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

				break;
			}
			
			default:
				return Button::wndproc(hwnd, message, wparam, lparam);
			}

			return 0;
		}
	}
}