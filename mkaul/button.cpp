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
			HWND hwnd_parent_,
			int id_,
			const Color_F* p_col_bg_,
			const Color_F* p_col_control_,
			const Rectangle<LONG>& rect,
			const std::string& tooltip_label_,
			int round_edge_flag_,
			float round_radius_
		)
		{
			tooltip_label = tooltip_label_;

			return Control::create(
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
		bool Button_Label::create(
			HWND hwnd_parent_,
			int id_,
			const std::string& label_,
			const Color_F* p_col_bg_,
			const Color_F* p_col_control_,
			const Color_F* p_col_label_,
			const Rectangle<LONG>& rect,
			const std::string& tooltip_label_,
			int round_edge_flag_,
			float round_radius_
		)
		{
			label = label_;

			return Button::create(
				hwnd_parent_,
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
			HWND hwnd_parent_,
			int id_,
			const char* icon_source_,
			Source_Type source_type_,
			const Color_F* p_col_bg_,
			const Color_F* p_col_control_,
			const Rectangle<LONG>& rect,
			const std::string& tooltip_label_,
			BYTE round_edge_flag_,
			float round_radius_
		)
		{
			source_type = source_type_;
			switch (source_type_) {
			case Source_Type::Filename:
				icon_source = icon_source_;
				break;

			case Source_Type::Resource:
				if (!HIWORD(icon_source_)) {
					icon_resource_num = reinterpret_cast<uint16_t>(icon_source_);
				}
				else {
					icon_source = icon_source_;
				}
				break;

			default:
				return false;
			}

			return Button::create(
				hwnd_parent_,
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
		LRESULT Button::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return 0;
		}
	}
}