//----------------------------------------------------------------------------------
//		MKAul (Control)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "control.hpp"



namespace mkaul {
	namespace window {
		// コントロールを作成
		bool Control::create(
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
		)
		{
			id = id_;
			round_edge_flag = round_edge_flag_;
			round_radius = round_radius_;
			p_col_bg = const_cast<Color_F*>(p_col_bg_);
			p_col_control = const_cast<Color_F*>(p_col_control_);

			return Window::create(
				hwnd_parent,
				NULL,
				class_name,
				wndproc_static,
				WS_CHILD | window_style,
				class_style,
				rect,
				cursor,
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
		LRESULT CALLBACK Control::wndproc(HWND hwnd_, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch (msg) {
			case WM_CREATE:

			default:
				return ::DefWindowProc(hwnd_, msg, wparam, lparam);
			}
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
	}
}