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
			HWND hwnd_parent_,
			int id_,
			const ColorF* p_col_bg_,
			const ColorF* p_col_control_,
			const WindowRectangle& rect,
			const std::string& tooltip_label_,
			RoundEdgeFlag round_edge_flag_,
			float round_radius_,
			float hover_highlight_
		)
		{
			tooltip_label = tooltip_label_;
			hover_highlight = hover_highlight_;

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


		// �{�^�����쐬(���x��)
		bool ButtonLabel::create(
			HINSTANCE hinst,
			HWND hwnd_parent_,
			int id_,
			const std::string& label_,
			const ColorF* p_col_bg_,
			const ColorF* p_col_control_,
			const ColorF* p_col_label_,
			const WindowRectangle& rect,
			const std::string& tooltip_label_,
			RoundEdgeFlag round_edge_flag_,
			float round_radius_,
			float hover_highlight_
		)
		{
			label = label_;
			p_col_label = const_cast<ColorF*>(p_col_label_);

			return Button::create(
				hinst,
				hwnd_parent_,
				id_,
				p_col_bg_,
				p_col_control_,
				rect,
				tooltip_label_,
				round_edge_flag_,
				round_radius_,
				hover_highlight_
			);
		}


		// �{�^�����쐬(�A�C�R��)
		bool ButtonIcon::create(
			HINSTANCE hinst,
			HWND hwnd_parent_,
			int id_,
			const char* icon_src_,
			SourceType src_type_,
			const ColorF* p_col_bg_,
			const ColorF* p_col_control_,
			const WindowRectangle& rect,
			const std::string& tooltip_label_,
			RoundEdgeFlag round_edge_flag_,
			float round_radius_,
			float hover_highlight_
		)
		{
			src_type = src_type_;

			// �\�[�X�̎�ނŏꍇ����
			switch (src_type_) {
			// �t�@�C��
			case SourceType::File:
				icon_src = icon_src_;
				break;

			// ���\�[�X
			case SourceType::Resource:
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
				hwnd_parent_,
				id_,
				p_col_bg_,
				p_col_control_,
				rect,
				tooltip_label_,
				round_edge_flag_,
				round_radius_,
				hover_highlight_
			);
		}


		// �E�B���h�E�v���V�[�W��
		LRESULT Button::wndproc(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd_, &rect_wnd);

			switch (message) {
			case WM_CREATE:
				p_graphics->init(hwnd_);

				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd_;
				break;

			case WM_CLOSE:
				p_graphics->exit();
				break;

			case WM_SIZE:
				p_graphics->resize();
				break;

			case WM_PAINT:
			{
				ColorF col_f = *p_col_control;

				if (clicked)
					col_f.change_brightness(-hover_highlight);
				else if (hovered)
					col_f.change_brightness(hover_highlight);

				p_graphics->begin_draw();
				p_graphics->fill_background(col_f);
				draw_round_edge();
				p_graphics->end_draw();

				break;
			}

			// �}�E�X���������Ƃ�
			case WM_MOUSEMOVE:
				if (!((uint32_t)status & (uint32_t)Status::Disabled)) {
					hovered = true;
					redraw();
					::TrackMouseEvent(&tme);
				}
				break;

			// ���N���b�N�����ꂽ�Ƃ�
			case WM_LBUTTONDOWN:
				if (!((uint32_t)status & (uint32_t)Status::Disabled)) {
					clicked = true;
					redraw();
					::TrackMouseEvent(&tme);
				}
				break;

			// ���N���b�N���I������Ƃ�
			case WM_LBUTTONUP:
				if (!((uint32_t)status & (uint32_t)Status::Disabled) && clicked) {
					clicked = false;
					::SendMessage(hwnd_parent, WM_COMMAND, id, 0);
					redraw();
				}
				break;

			// �}�E�X���E�B���h�E���痣�ꂽ�Ƃ�
			case WM_MOUSELEAVE:
				clicked = false;
				hovered = false;
				redraw();
				break;

			default:
				return ::DefWindowProc(hwnd_, message, wparam, lparam);
			}

			return 0;
		}
	}
}