#include "control.hpp"



namespace mkaul {
	namespace ui {
		// コントロールを作成
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

			// 描画オブジェクトを作成
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


		// 静的ウィンドウプロシージャ
		LRESULT CALLBACK Control::wndproc_static(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
		{
			// 自身のインスタンスのポインタをウィンドウに設定したユーザーデータから取得
			// (静的メンバ関数内ではthisポインタおよび非静的メンバが使用できないため)
			Control* app = (Control*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);

			// ウィンドウがまだ作成されていない場合
			if (!app) {
				// ここでウィンドウが作成される場合
				if (message == WM_CREATE) {
					// lParamに格納されているLPCREATESTRUCTからユーザーデータにアクセスできるためそこから取得
					app = (Control*)((LPCREATESTRUCT)lparam)->lpCreateParams;

					if (app) {
						::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
						return app->wndproc(hwnd, message, wparam, lparam);
					}
				}
				return ::DefWindowProc(hwnd, message, wparam, lparam);
			}
			// ウィンドウが存在する場合
			else
				// メンバ関数のウィンドウプロシージャを参照
				return app->wndproc(hwnd, message, wparam, lparam);
		}


		// ステータスを設定
		void Control::set_status(flag::Status status)
		{
			status_ = status;
			redraw();
		}


		// ステータスを追加
		void Control::add_status(flag::Status status)
		{
			status_ |= status;
			redraw();
		}


		// ラウンドエッジを描画
		void Control::draw_round_edge()
		{
			// 描画オブジェクトが存在し、描画中である場合
			if (p_graphics_ and p_graphics_->is_drawing()) {
				RECT rect;

				if (::GetClientRect(hwnd_, &rect)) {
					// 始点の配列
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
						// 角を丸くするフラグが立っている場合
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

						// 90d回転
						angle += 90.f;
						pt.rotate(90.);
					}
				}
			}
		}
	}
}