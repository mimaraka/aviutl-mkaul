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
		bool Button::create(
			HINSTANCE hinst,
			HWND hwnd_parent,
			int id_,
			const Color_F* p_col_bg_,
			const Color_F* p_col_control_,
			const Rectangle<LONG>& rect,
			const std::string& tooltip_label_,
			Round_Edge_Flag round_edge_flag_,
			float round_radius_
		)
		{
			tooltip_label = tooltip_label_;

			return Control::create(
				hinst,
				hwnd_parent,
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
		bool Button_Label::create(
			HINSTANCE hinst,
			HWND hwnd_parent,
			int id_,
			const std::string& label_,
			const Color_F* p_col_bg_,
			const Color_F* p_col_control_,
			const Color_F* p_col_label_,
			const Rectangle<LONG>& rect,
			const std::string& tooltip_label_,
			Round_Edge_Flag round_edge_flag_,
			float round_radius_
		)
		{
			label = label_;
			p_col_label = const_cast<Color_F*>(p_col_label_);

			return Button::create(
				hinst,
				hwnd_parent,
				id_,
				p_col_bg_,
				p_col_control_,
				rect,
				tooltip_label_,
				round_edge_flag_,
				round_radius_
			);
		}


		// ボタンを作成(アイコン)
		bool Button_Icon::create(
			HINSTANCE hinst,
			HWND hwnd_parent,
			int id_,
			const char* icon_src_,
			Source_Type src_type_,
			const Color_F* p_col_bg_,
			const Color_F* p_col_control_,
			const Rectangle<LONG>& rect,
			const std::string& tooltip_label_,
			Round_Edge_Flag round_edge_flag_,
			float round_radius_
		)
		{
			src_type = src_type_;

			// ソースの種類で場合分け
			switch (src_type_) {
			// ファイル
			case Source_Type::File:
				icon_src = icon_src_;
				break;

			// リソース
			case Source_Type::Resource:
				// リソースが数値の場合
				if (!HIWORD(icon_src_))
					icon_resource_num = LOWORD(icon_src_);
				// リソースが文字列の場合
				else
					icon_src = icon_src_;
				break;

			default:
				return false;
			}

			return Button::create(
				hinst,
				hwnd_parent,
				id_,
				p_col_bg_,
				p_col_control_,
				rect,
				tooltip_label_,
				round_edge_flag_,
				round_radius_
			);
		}


		// ウィンドウプロシージャ
		LRESULT Button::wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd_, &rect_wnd);

			switch (msg) {
			case WM_CREATE:
				p_graphics->init(hwnd_);
				break;

			case WM_CLOSE:
				p_graphics->exit();
				break;

			case WM_SIZE:
				p_graphics->resize();
				break;

			case WM_PAINT:
				p_graphics->begin_draw();

				p_graphics->fill_background(*p_col_control);

				draw_round_edge();

				p_graphics->end_draw();
				break;

			default:
				return ::DefWindowProc(hwnd_, msg, wparam, lparam);
			}

			return 0;
		}
	}
}