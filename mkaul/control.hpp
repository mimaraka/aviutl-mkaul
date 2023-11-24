#pragma once

#include "window.hpp"
#include "graphics.hpp"
#include "flag.hpp"



namespace mkaul {
	namespace flag {
		// �X�e�[�^�X
		enum class Status : uint32_t {
			Null = 0u,
			Disabled = 1u << 0
		};

		// �ǂ̊p���ۂ����邩�̃t���O
		enum class RoundEdge : uint32_t {
			None = 0u,
			LeftTop = 1u << 0,
			LeftBottom = 1u << 1,
			RightTop = 1u << 2,
			RightBottom = 1u << 3,
			All = 0b1111
		};
	}

	namespace window {
		//---------------------------------------------------------------------
		//		�R���g���[��(���ۃN���X)
		//---------------------------------------------------------------------
		class Control : public Window {
		protected:
			// �F
			ColorF*					p_color_bg_;
			ColorF*					p_color_control_;
			// �X�e�[�^�X
			flag::Status			status_;
			TRACKMOUSEEVENT			tme_;
			HWND					hwnd_parent_;
			graphics::Graphics*		p_graphics_;
			flag::RoundEdge			round_edge_;
			// �R���g���[��ID
			int						id_;
			float					round_radius_;

			static LRESULT CALLBACK	wndproc_static(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam);
			virtual LRESULT			wndproc(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam) = 0;

		public:
			// �R���X�g���N�^
			Control() :
				hwnd_parent_(NULL),
				id_(0),
				status_(flag::Status::Null),
				p_color_bg_(nullptr),
				p_color_control_(nullptr),
				round_radius_(0.f),
				round_edge_(flag::RoundEdge::None),
				tme_({ 0 }),
				p_graphics_(nullptr)
			{}

			// �f�X�g���N�^
			~Control()
			{}

			// �R���g���[�����쐬
			virtual HWND create(
				HINSTANCE				hinst,
				HWND					hwnd_parent,
				int						id,
				LPCTSTR					class_name,
				LONG					window_style,
				LONG					class_style,
				const WindowRectangle&	rect,
				const ColorF*			p_color_bg,
				const ColorF*			p_color_control,
				flag::RoundEdge			round_edge = flag::RoundEdge::None,
				float					round_radius = 0.f,
				HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW)
			);
			// �X�e�[�^�X��ݒ�
			void set_status(flag::Status status);
			// �X�e�[�^�X��ǉ�
			void add_status(flag::Status status);
			// ���E���h�G�b�W��`��
			void draw_round_edge();
		};
	}
}