//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "control.hpp"



namespace mkaul {
	namespace window {
		// コントロールを作成
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

		// 静的ウィンドウプロシージャ
		LRESULT CALLBACK Control::wndproc_static(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			// 自身のインスタンスのポインタをウィンドウに設定したユーザーデータから取得
			// (静的メンバ関数内ではthisポインタおよび非静的メンバが使用できないため)
			Control* app = (Control*)::GetWindowLongPtr(hwnd_, GWLP_USERDATA);

			// ウィンドウがまだ作成されていない場合
			if (!app) {
				// ここでウィンドウが作成される場合
				if (msg == WM_CREATE) {
					// lParamに格納されているLPCREATESTRUCTからユーザーデータにアクセスできるためそこから取得
					app = (Control*)((LPCREATESTRUCT)lparam)->lpCreateParams;

					if (app) {
						::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)app);
						return app->wndproc(hwnd_, msg, wparam, lparam);
					}
				}
				return ::DefWindowProc(hwnd_, msg, wparam, lparam);
			}
			// ウィンドウが存在する場合
			else
				// メンバ関数のウィンドウプロシージャを参照
				return app->wndproc(hwnd_, msg, wparam, lparam);
		}

		// ウィンドウプロシージャ
		LRESULT Control::wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return ::DefWindowProc(hwnd_, msg, wparam, lparam);
		}

		// ステータスを設定
		inline void Control::set_status(int status_)
		{
			status = status_;
			redraw();
		}

		// ラウンドエッジを描画
		inline void Control::draw_round_edge()
		{

		}

		// コントロールの色を変更
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