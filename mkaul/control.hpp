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
		//		コントロール(抽象クラス)
		//---------------------------------------------------------------------
		class Control : public Window {
		public:
			// ステータス
			enum class Status : uint32_t {
				Null = 0u,
				Disabled = 1u << 0
			};

		protected:
			// 色
			ColorF*				p_col_bg;
			ColorF*				p_col_control;
			// ステータス
			Status					status;
			TRACKMOUSEEVENT			tme;
			HWND					hwnd_parent;
			graphics::Graphics*		p_graphics;

			static LRESULT CALLBACK	wndproc_static(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam);
			virtual LRESULT			wndproc(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam) = 0;

		public:
			// コントロールID
			int						id;
			// ラウンドエッジの半径
			float					round_radius;
			// どの角を丸くするかのフラグ
			enum class RoundEdgeFlag : uint32_t {
				None = 0u,
				LeftTop = 1u << 0,
				LeftBottom = 1u << 1,
				RightTop = 1u << 2,
				RightBottom = 1u << 3,
				All = 0b1111
			}						round_edge_flag;

			// コンストラクタ
			Control() :
				hwnd_parent(NULL),
				id(0),
				status(Status::Null),
				p_col_bg(nullptr),
				p_col_control(nullptr),
				round_radius(0.f),
				round_edge_flag(RoundEdgeFlag::None),
				tme({ 0 }),
				p_graphics(nullptr)
			{}

			// デストラクタ
			~Control()
			{}

			// コントロールを作成
			virtual HWND create(
				HINSTANCE				hinst,
				HWND					hwnd_parent,
				int						id_,
				LPCTSTR					class_name,
				LONG					window_style,
				LONG					class_style,
				const WindowRectangle&	rect,
				const ColorF*			p_col_bg_,
				const ColorF*			p_col_control_,
				RoundEdgeFlag			round_edge_flag_ = RoundEdgeFlag::None,
				float					round_radius_ = 0.f,
				HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW)
			);
			// ステータスを設定
			void set_status(Status status_);

			// ステータスを追加
			void add_status(Status status_);

			// ラウンドエッジを描画
			void draw_round_edge();
		};
	}
}