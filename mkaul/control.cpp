#include "control.hpp"



namespace mkaul {
	namespace window {
		// �R���g���[�����쐬
		HWND Control::create(
			HINSTANCE				hinst,
			HWND					hwnd_parent,
			int						id,
			LPCTSTR					class_name,
			LONG					window_style,
			LONG					class_style,
			const WindowRectangle&	rect,
			const ColorF*			p_color_bg,
			const ColorF*			p_color_control,
			flag::RoundEdge			round_edge,
			float					round_radius,
			HCURSOR					cursor
		)
		{
			id_ = id;
			hwnd_parent_ = hwnd_parent;
			round_edge_ = round_edge;
			round_radius_ = round_radius;
			p_color_bg_ = const_cast<ColorF*>(p_color_bg);
			p_color_control_ = const_cast<ColorF*>(p_color_control);

			// �`��I�u�W�F�N�g���쐬
			if (!graphics::Manager::create_graphics(&p_graphics_)) return NULL;

			return Window::create(
				hinst,
				hwnd_parent,
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
		LRESULT CALLBACK Control::wndproc_static(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
		{
			// ���g�̃C���X�^���X�̃|�C���^���E�B���h�E�ɐݒ肵�����[�U�[�f�[�^����擾
			// (�ÓI�����o�֐����ł�this�|�C���^����є�ÓI�����o���g�p�ł��Ȃ�����)
			Control* app = (Control*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);

			// �E�B���h�E���܂��쐬����Ă��Ȃ��ꍇ
			if (!app) {
				// �����ŃE�B���h�E���쐬�����ꍇ
				if (message == WM_CREATE) {
					// lParam�Ɋi�[����Ă���LPCREATESTRUCT���烆�[�U�[�f�[�^�ɃA�N�Z�X�ł��邽�߂�������擾
					app = (Control*)((LPCREATESTRUCT)lparam)->lpCreateParams;

					if (app) {
						::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
						return app->wndproc(hwnd, message, wparam, lparam);
					}
				}
				return ::DefWindowProc(hwnd, message, wparam, lparam);
			}
			// �E�B���h�E�����݂���ꍇ
			else
				// �����o�֐��̃E�B���h�E�v���V�[�W�����Q��
				return app->wndproc(hwnd, message, wparam, lparam);
		}


		// �X�e�[�^�X��ݒ�
		void Control::set_status(flag::Status status)
		{
			status_ = status;
			redraw();
		}


		// �X�e�[�^�X��ǉ�
		void Control::add_status(flag::Status status)
		{
			status_ |= status;
			redraw();
		}


		// ���E���h�G�b�W��`��
		void Control::draw_round_edge()
		{
			// �`��I�u�W�F�N�g�����݂��A�`�撆�ł���ꍇ
			if (p_graphics_ && p_graphics_->is_drawing()) {
				RECT rect;

				if (::GetClientRect(hwnd_, &rect)) {
					// �n�_�̔z��
					Point<float> pts[] = {
						Point(0.f, 0.f),
						Point((float)rect.right, 0.f),
						Point((float)rect.right, (float)rect.bottom),
						Point(0.f, (float)rect.bottom)
					};

					auto r = std::min(std::min(rect.right * 0.5f, rect.bottom * 0.5f), round_radius_);

					auto pt = Point(0.f, r);
					float angle = -90.f;

					for (int i = 0; i < 4; i++) {
						// �p���ۂ�����t���O�������Ă���ꍇ
						if ((uint32_t)round_edge_ & 1 << i) {
							graphics::Path* path;
							graphics::Manager::create_path(&path);

							path->begin(pts[i]);
							path->add_line(pts[i] + pt);
							path->add_arc(
								Size(r, r),
								angle, 90.f
							);
							path->end();

							p_graphics_->fill_path(
								path,
								*p_color_bg_
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