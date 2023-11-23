//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "button.hpp"



namespace mkaul {
	namespace window {
		// ボタンを作成
		HWND Button::create(
			HINSTANCE hinst,
			HWND hwnd_parent_,
			int id_,
			const ColorF* p_col_bg_,
			const ColorF* p_col_control_,
			const WindowRectangle& rect,
			const std::string& tooltip_label,
			RoundEdgeFlag round_edge_flag_,
			float round_radius_,
			float hover_highlight
		)
		{
			tooltip_label_ = tooltip_label;
			hover_highlight_ = hover_highlight;

			return Control::create(
				hinst,
				hwnd_parent_,
				id_,
				TEXT("MKAUL_BUTTON"),
				NULL,
				NULL,
				rect,
				p_col_bg_,
				p_col_control_,
				round_edge_flag_,
				round_radius_,
				::LoadCursor(NULL, IDC_HAND)
			);
		}


		// ボタンを作成(ラベル)
		HWND LabelButton::create(
			HINSTANCE hinst,
			HWND hwnd_parent_,
			int id_,
			const std::string& label,
			const graphics::Font font,
			const ColorF* p_col_bg_,
			const ColorF* p_col_control_,
			const ColorF* p_col_label,
			const WindowRectangle& rect,
			const std::string& tooltip_label,
			RoundEdgeFlag round_edge_flag_,
			float round_radius_,
			float hover_highlight
		)
		{
			label_ = label;
			font_ = font;
			p_col_label_ = const_cast<ColorF*>(p_col_label);

			return Button::create(
				hinst,
				hwnd_parent_,
				id_,
				p_col_bg_,
				p_col_control_,
				rect,
				tooltip_label,
				round_edge_flag_,
				round_radius_,
				hover_highlight
			);
		}


		// ボタンを作成(アイコン)
		HWND IconButton::create(
			HINSTANCE hinst,
			HWND hwnd_parent_,
			int id_,
			const char* icon_src_,
			SourceType src_type_,
			const ColorF* p_col_bg_,
			const ColorF* p_col_control_,
			const WindowRectangle& rect,
			const std::string& tooltip_label,
			RoundEdgeFlag round_edge_flag_,
			float round_radius_,
			float hover_highlight
		)
		{
			src_type = src_type_;

			// ソースの種類で場合分け
			switch (src_type_) {
			// ファイル
			case SourceType::File:
				icon_src = icon_src_;
				break;

			// リソース
			case SourceType::Resource:
				// リソースが数値の場合
				if (!HIWORD(icon_src_))
					icon_resource_num = LOWORD(icon_src_);
				// リソースが文字列の場合
				else
					icon_src = icon_src_;
				break;

			default:
				return NULL;
			}

			return Button::create(
				hinst,
				hwnd_parent_,
				id_,
				p_col_bg_,
				p_col_control_,
				rect,
				tooltip_label,
				round_edge_flag_,
				round_radius_,
				hover_highlight
			);
		}


		// ウィンドウプロシージャ
		LRESULT Button::wndproc(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd_, &rect_wnd);

			switch (message) {
			case WM_CREATE:
				p_graphics->init(hwnd_);

				if (!tooltip_label_.empty()) {
					hwnd_tooltip_ = ::CreateWindowEx(
						0, TOOLTIPS_CLASS,
						NULL, TTS_ALWAYSTIP,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						hwnd_, NULL,
						(HINSTANCE)::GetWindowLongA(hwnd, GWL_HINSTANCE),
						NULL
					);

					tool_info_.cbSize = sizeof(TOOLINFO);
					tool_info_.uFlags = TTF_SUBCLASS;
					tool_info_.hwnd = hwnd_;
					tool_info_.uId = id;
					tool_info_.lpszText = (LPTSTR)tooltip_label_.c_str();
					::SendMessage(hwnd_tooltip_, TTM_ADDTOOL, 0, (LPARAM)&tool_info_);
				}

				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd_;
				break;

			case WM_CLOSE:
				p_graphics->exit();
				break;

			case WM_SIZE:
				p_graphics->resize();

				if (!tooltip_label_.empty()) {
					tool_info_.rect = rect_wnd;
					::SendMessage(hwnd_tooltip_, TTM_NEWTOOLRECT, 0, (LPARAM)&tool_info_);
				}
				break;

			case WM_PAINT:
			{
				ColorF col_f = *p_col_control;

				if (clicked_)
					col_f.change_brightness(-hover_highlight_);
				else if (hovered_)
					col_f.change_brightness(hover_highlight_);

				p_graphics->begin_draw();
				p_graphics->fill_background(col_f);
				draw_round_edge();
				p_graphics->end_draw();

				break;
			}

			// マウスが動いたとき
			case WM_MOUSEMOVE:
				if (!((uint32_t)status & (uint32_t)Status::Disabled)) {
					hovered_ = true;
					redraw();
					::TrackMouseEvent(&tme);
				}
				break;

			// 左クリックがされたとき
			case WM_LBUTTONDOWN:
				if (!((uint32_t)status & (uint32_t)Status::Disabled)) {
					clicked_ = true;
					redraw();
					::TrackMouseEvent(&tme);
				}
				break;

			// 左クリックが終わったとき
			case WM_LBUTTONUP:
				if (!((uint32_t)status & (uint32_t)Status::Disabled) && clicked_) {
					clicked_ = false;
					::SendMessage(hwnd_parent, WM_COMMAND, id, 0);
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
				return ::DefWindowProc(hwnd_, message, wparam, lparam);
			}

			return 0;
		}


		LRESULT LabelButton::wndproc(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd_, &rect_wnd);

			switch (message) {
			case WM_PAINT:
			{
				ColorF col_f = *p_col_control;

				if (clicked_)
					col_f.change_brightness(-hover_highlight_);
				else if (hovered_)
					col_f.change_brightness(hover_highlight_);

				p_graphics->begin_draw();
				p_graphics->fill_background(col_f);
				p_graphics->draw_text(
					label_,
					Rectangle<float>{
						0.f, 0.f,
						(float)rect_wnd.right,
						(float)rect_wnd.bottom
					},
					font_,
					graphics::AnchorPosition::Center,
					true,
					*p_col_label_
				);
				draw_round_edge();
				p_graphics->end_draw();

				break;
			}
			
			default:
				return Button::wndproc(hwnd_, message, wparam, lparam);
			}

			return 0;
		}
	}
}