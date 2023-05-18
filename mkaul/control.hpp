//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "window.hpp"
#include "graphics.hpp"



namespace mkaul {
	namespace window {
		//---------------------------------------------------------------------
		//		�R���g���[��(���ۃN���X)
		//---------------------------------------------------------------------
		class Control : public Window {
		protected:
			// �F
			Color_F*				p_col_bg;
			Color_F*				p_col_control;
			// �X�e�[�^�X
			int						status;
			TRACKMOUSEEVENT			tme;
			graphics::Graphics*		p_graphics;

			static LRESULT CALLBACK	wndproc_static(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam);
			virtual LRESULT			wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam) = 0;

		public:
			static constexpr int    STATUS_DISABLED = 1 << 0;
			// ���E���h�G�b�W�p�t���O
			static constexpr BYTE   ROUND_EDGE_LT = 1 << 0;
			static constexpr BYTE   ROUND_EDGE_LB = 1 << 1;
			static constexpr BYTE   ROUND_EDGE_RT = 1 << 2;
			static constexpr BYTE   ROUND_EDGE_RB = 1 << 3;
			static constexpr BYTE   ROUND_EDGE_ALL = ROUND_EDGE_LT | ROUND_EDGE_LB | ROUND_EDGE_RT | ROUND_EDGE_RB;

			// �R���g���[��ID
			int                     id;
			// ���E���h�G�b�W�̔��a
			float                   round_radius;
			// �ǂ̊p���ۂ����邩�̃t���O
			BYTE                    round_edge_flag;

			// �R���X�g���N�^
			Control() :
				id(0),
				status(0),
				p_col_bg(0),
				p_col_control(0),
				round_radius(0.f),
				round_edge_flag(NULL),
				tme({ 0 }),
				p_graphics(nullptr)
			{}

			// �f�X�g���N�^
			~Control()
			{}

			// �R���g���[�����쐬
			virtual bool create(
				HINSTANCE				hinst,
				HWND					hwnd_parent,
				int						id_,
				const std::string&		class_name,
				LONG					window_style,
				LONG					class_style,
				const Rectangle<LONG>&	rect,
				const Color_F*			p_col_bg_,
				const Color_F*			p_col_control_,
				BYTE					round_edge_flag_ = ROUND_EDGE_ALL,
				float					round_radius_ = 0.f,
				HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW)
			);
			// �X�e�[�^�X��ݒ�
			void set_status(int status_);
			// ���E���h�G�b�W��`��
			void draw_round_edge();
		};
	}
}