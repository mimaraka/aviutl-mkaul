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
		//		コントロール
		//---------------------------------------------------------------------
		class Control : public Window {
		protected:
			// 色
			Color_F					color_bg,
									color_control;
			// ステータス
			int						status;
			TRACKMOUSEEVENT			tme;
			graphics::Graphics*		p_graphics;

			static LRESULT CALLBACK wndproc_static(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam);
			virtual LRESULT		    wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam);

		public:
			static constexpr int    STATUS_DISABLED = 1 << 0;
			// ラウンドエッジ用フラグ
			static constexpr BYTE   ROUND_EDGE_LT = 1 << 0;
			static constexpr BYTE   ROUND_EDGE_LB = 1 << 1;
			static constexpr BYTE   ROUND_EDGE_RT = 1 << 2;
			static constexpr BYTE   ROUND_EDGE_RB = 1 << 3;
			static constexpr BYTE   ROUND_EDGE_ALL = ROUND_EDGE_LT | ROUND_EDGE_LB | ROUND_EDGE_RT | ROUND_EDGE_RB;

			// コントロールID
			int                     id;
			// ラウンドエッジの半径
			float                   round_radius;
			// どの角を丸くするかのフラグ
			BYTE                    round_edge_flag;

			// コンストラクタ
			Control() :
				id(0),
				status(0),
				color_bg(0),
				color_control(0),
				round_radius(0.f),
				round_edge_flag(NULL),
				tme({ NULL }),
				p_graphics(nullptr)
			{}

			// デストラクタ
			~Control()
			{}

			// コントロールを作成
			virtual BOOL create(
				HWND		        hwnd_parent,
				int                 id_,
				LPCTSTR				class_name,
				LONG				window_style,
				LONG				class_style,
				HCURSOR             cursor,
				const RECT& rect,
				const Color_F&		color_bg_,
				const Color_F&		color_control_,
				BYTE                round_edge_flag_,
				float               round_radius_,
				int                 status_
			);
			// ステータスを設定
			void set_status(int status_);
			// フォントを設定
			virtual void set_font(LPCSTR font_name);
			// コントロールの色を変更
			virtual void change_color(
				const Color_F& color_bg_,
				const Color_F& color_control
			);
			// ラウンドエッジを描画
			void draw_round_edge();
		};
	}
}