//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "control.hpp"



namespace mkaul {
	namespace window {
		// �R���g���[�����쐬
		bool Control::create(
			HINSTANCE				hinst,
			HWND					hwnd_parent_,
			int						id_,
			LPCTSTR					class_name,
			LONG					window_style,
			LONG					class_style,
			const WindowRectangle&	rect,
			const ColorF*			p_col_bg_,
			const ColorF*			p_col_control_,
			RoundEdgeFlag			round_edge_flag_,
			float					round_radius_,
			HCURSOR					cursor
		)
		{
			id = id_;
			hwnd_parent = hwnd_parent_;
			round_edge_flag = round_edge_flag_;
			round_radius = round_radius_;
			p_col_bg = const_cast<ColorF*>(p_col_bg_);
			p_col_control = const_cast<ColorF*>(p_col_control_);

			// �`��I�u�W�F�N�g���쐬
			graphics::Manager::create_graphics(&p_graphics);

			return Window::create(
				hinst,
				hwnd_parent_,
				"",
				class_name,
				wndproc_static,
				WS_CHILD | window_style,
				class_style,
				rect,
				cursor,
				this
			);
		}


		// �ÓI�E�B���h�E�v���V�[�W��
		LRESULT CALLBACK Control::wndproc_static(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam)
		{
			// ���g�̃C���X�^���X�̃|�C���^���E�B���h�E�ɐݒ肵�����[�U�[�f�[�^����擾
			// (�ÓI�����o�֐����ł�this�|�C���^����є�ÓI�����o���g�p�ł��Ȃ�����)
			Control* app = (Control*)::GetWindowLongPtr(hwnd_, GWLP_USERDATA);

			// �E�B���h�E���܂��쐬����Ă��Ȃ��ꍇ
			if (!app) {
				// �����ŃE�B���h�E���쐬�����ꍇ
				if (message == WM_CREATE) {
					// lParam�Ɋi�[����Ă���LPCREATESTRUCT���烆�[�U�[�f�[�^�ɃA�N�Z�X�ł��邽�߂�������擾
					app = (Control*)((LPCREATESTRUCT)lparam)->lpCreateParams;

					if (app) {
						::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)app);
						return app->wndproc(hwnd_, message, wparam, lparam);
					}
				}
				return ::DefWindowProc(hwnd_, message, wparam, lparam);
			}
			// �E�B���h�E�����݂���ꍇ
			else
				// �����o�֐��̃E�B���h�E�v���V�[�W�����Q��
				return app->wndproc(hwnd_, message, wparam, lparam);
		}


		// �X�e�[�^�X��ݒ�
		void Control::set_status(Status status_)
		{
			status = status_;
			redraw();
		}


		// �X�e�[�^�X��ǉ�
		void Control::add_status(Status status_)
		{
			status = (Status)((uint32_t)status | (uint32_t)status_);
			redraw();
		}


		// ���E���h�G�b�W��`��
		void Control::draw_round_edge()
		{
			// �`��I�u�W�F�N�g�����݂��A�`�撆�ł���ꍇ
			if (p_graphics && p_graphics->is_drawing()) {
				RECT rect;

				if (::GetClientRect(hwnd, &rect)) {
					// �n�_�̔z��
					Point<float> pts[] = {
						Point(0.f, 0.f),
						Point((float)rect.right, 0.f),
						Point((float)rect.right, (float)rect.bottom),
						Point(0.f, (float)rect.bottom)
					};

					auto r = std::min(std::min(rect.right * 0.5f, rect.bottom * 0.5f), round_radius);

					auto pt = Point(0.f, r);
					float angle = -90.f;

					for (int i = 0; i < 4; i++) {
						// �p���ۂ�����t���O�������Ă���ꍇ
						if ((uint32_t)round_edge_flag & 1 << i) {
							graphics::Path* path;
							graphics::Manager::create_path(&path);

							path->begin(pts[i]);
							path->add_line(pts[i] + pt);
							path->add_arc(
								Size(r, r),
								angle, 90.f
							);
							path->end();

							p_graphics->fill_path(
								path,
								*p_col_bg
							);
							path->release();
						}

						// 90d��]
						angle += 90.f;
						pt.rotate(90.);
					}
				}
			}
		}
	}
}