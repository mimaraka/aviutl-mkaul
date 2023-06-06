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

			// 描画オブジェクトを作成
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


		// 静的ウィンドウプロシージャ
		LRESULT CALLBACK Control::wndproc_static(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam)
		{
			// 自身のインスタンスのポインタをウィンドウに設定したユーザーデータから取得
			// (静的メンバ関数内ではthisポインタおよび非静的メンバが使用できないため)
			Control* app = (Control*)::GetWindowLongPtr(hwnd_, GWLP_USERDATA);

			// ウィンドウがまだ作成されていない場合
			if (!app) {
				// ここでウィンドウが作成される場合
				if (message == WM_CREATE) {
					// lParamに格納されているLPCREATESTRUCTからユーザーデータにアクセスできるためそこから取得
					app = (Control*)((LPCREATESTRUCT)lparam)->lpCreateParams;

					if (app) {
						::SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)app);
						return app->wndproc(hwnd_, message, wparam, lparam);
					}
				}
				return ::DefWindowProc(hwnd_, message, wparam, lparam);
			}
			// ウィンドウが存在する場合
			else
				// メンバ関数のウィンドウプロシージャを参照
				return app->wndproc(hwnd_, message, wparam, lparam);
		}


		// ステータスを設定
		void Control::set_status(Status status_)
		{
			status = status_;
			redraw();
		}


		// ステータスを追加
		void Control::add_status(Status status_)
		{
			status = (Status)((uint32_t)status | (uint32_t)status_);
			redraw();
		}


		// ラウンドエッジを描画
		void Control::draw_round_edge()
		{
			// 描画オブジェクトが存在し、描画中である場合
			if (p_graphics && p_graphics->is_drawing()) {
				RECT rect;

				if (::GetClientRect(hwnd, &rect)) {
					// 始点の配列
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
						// 角を丸くするフラグが立っている場合
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

						// 90d回転
						angle += 90.f;
						pt.rotate(90.);
					}
				}
			}
		}
	}
}