//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "button.hpp"



namespace mkaul {
	namespace window {
		// �{�^�����쐬
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


		// �{�^�����쐬(���x��)
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


		// �{�^�����쐬(�A�C�R��)
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

			// �\�[�X�̎�ނŏꍇ����
			switch (src_type_) {
			// �t�@�C��
			case Source_Type::File:
				icon_src = icon_src_;
				break;

			// ���\�[�X
			case Source_Type::Resource:
				// ���\�[�X�����l�̏ꍇ
				if (!HIWORD(icon_src_))
					icon_resource_num = LOWORD(icon_src_);
				// ���\�[�X��������̏ꍇ
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


		// �E�B���h�E�v���V�[�W��
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