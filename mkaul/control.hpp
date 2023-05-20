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
			Color_F*				p_col_bg;
			Color_F*				p_col_control;
			// ステータス
			Status					status;
			TRACKMOUSEEVENT			tme;
			HWND					hwnd_parent;
			graphics::Graphics*		p_graphics;

			static LRESULT CALLBACK	wndproc_static(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam);
			virtual LRESULT			wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam) = 0;

		public:
			// コントロールID
			int						id;
			// ラウンドエッジの半径
			float					round_radius;
			// どの角を丸くするかのフラグ
			enum class Round_Edge_Flag : uint32_t {
				None = 0u,
				Left_Top = 1u << 0,
				Left_Bottom = 1u << 1,
				Right_Top = 1u << 2,
				Right_Bottom = 1u << 3,
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
				round_edge_flag(Round_Edge_Flag::None),
				tme({ 0 }),
				p_graphics(nullptr)
			{}

			// デストラクタ
			~Control()
			{}

			// コントロールを作成
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
				Round_Edge_Flag			round_edge_flag_ = Round_Edge_Flag::None,
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