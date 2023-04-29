//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "control.hpp"



namespace mkaul {
	namespace window {
		// �R���g���[�����쐬
		BOOL Control::create(
			HWND		        hwnd_parent,
			int                 id_,
			LPCTSTR             class_name,
			LONG		        window_style,
			LONG		        class_style,
			HCURSOR             cursor,
			const RECT& rect,
			const Color_F& color_bg_,
			const Color_F& color_control_,
			BYTE                round_edge_flag_,
			float               round_radius_,
			int                 status_
		)
		{
			id = id_;
			status = status_;
			round_edge_flag = round_edge_flag_;
			round_radius = round_radius_;
			color_bg = color_bg_;
			color_control = color_control_;

			return Window::create(
				hwnd_parent,
				NULL,
				class_name,
				wndproc_static,
				WS_CHILD | window_style,
				class_style,
				cursor,
				rect,
				this
			);
		}

		// �ÓI�E�B���h�E�v���V�[�W��
		LRESULT CALLBACK Control::wndproc_static(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			// ���g�̃C���X�^���X�̃|�C���^���E�B���h�E�ɐݒ肵�����[�U�[�f�[�^����擾
			// (�ÓI�����o�֐����ł�this�|�C���^����є�ÓI�����o���g�p�ł��Ȃ�����)
			Control* app = (Control*)::GetWindowLongPtr(hwnd_, GWLP_USERDATA);

			// �E�B���h�E���܂��쐬����Ă��Ȃ��ꍇ
			if (!app) {
				// �����ŃE�B���h�E���쐬�����ꍇ
				if (msg == WM_CREATE) {
					// lParam�Ɋi�[����Ă���LPCREATESTRUCT���烆�[�U�[�f�[�^�ɃA�N�Z�X�ł��邽�߂�������擾
					app = (Control*)((LPCREATESTRUCT)lparam)->lpCreateParams;

					if (app) {
						::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)app);
						return app->wndproc(hwnd_, msg, wparam, lparam);
					}
				}
				return ::DefWindowProc(hwnd_, msg, wparam, lparam);
			}
			// �E�B���h�E�����݂���ꍇ
			else
				// �����o�֐��̃E�B���h�E�v���V�[�W�����Q��
				return app->wndproc(hwnd_, msg, wparam, lparam);
		}

		// �E�B���h�E�v���V�[�W��
		LRESULT Control::wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return ::DefWindowProc(hwnd_, msg, wparam, lparam);
		}

		// �X�e�[�^�X��ݒ�
		inline void Control::set_status(int status_)
		{
			status = status_;
			redraw();
		}

		// ���E���h�G�b�W��`��
		inline void Control::draw_round_edge()
		{

		}

		// �R���g���[���̐F��ύX
		inline void Control::change_color(
			const Color_F& color_bg_,
			const Color_F& color_control_
		)
		{
			color_bg = color_bg_;
			color_control = color_control_;
			redraw();
		}
	}
}