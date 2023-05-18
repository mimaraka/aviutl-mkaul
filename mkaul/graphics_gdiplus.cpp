//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "common.hpp"
#include "graphics_gdiplus.hpp"



namespace mkaul {
	namespace graphics {
		void Bitmap_Gdiplus::release()
		{
			delete reinterpret_cast<Gdiplus::Bitmap*>(data);
		}


		size_t Bitmap_Gdiplus::get_width()
		{
			return reinterpret_cast<Gdiplus::Bitmap*>(data)->GetWidth();
		}


		size_t Bitmap_Gdiplus::get_height()
		{
			return reinterpret_cast<Gdiplus::Bitmap*>(data)->GetHeight();
		}


		// 開放
		void Path_Gdiplus::release()
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);
			if (p_path)
				delete p_path;
			data[0] = nullptr;
		}


		// パスの開始
		bool Path_Gdiplus::begin(const Point<float>& pt)
		{
			if (!data[0]) {
				auto p_path = new Gdiplus::GraphicsPath();
				data[0] = p_path;

				pt_last = pt;

				return true;
			}
			else return false;
		}


		// パスの終了
		void Path_Gdiplus::end(Figure_End fe)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (fe == Figure_End::Closed)
				p_path->CloseAllFigures();
		}


		// 弧を追加
		void Path_Gdiplus::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);
			constexpr float pi = std::numbers::pi_v<float>;
			Point<float> pt_ofs_start, pt_o;

			// 角度を-2π ~ 2πの範囲に収める
			angle_sweep = std::fmodf(angle_sweep, pi * 2.f);

			ellipse_pos(size, angle_start, &pt_ofs_start);
			pt_o = pt_last - pt_ofs_start;

			if (p_path) p_path->AddArc(
				Gdiplus::RectF(
					pt_o.x - size.width,
					pt_o.y - size.height,
					size.width * 2.f,
					size.height * 2.f
				),
				-rad2deg(angle_start),
				-rad2deg(angle_sweep)
			);
		}


		// 線を追加
		void Path_Gdiplus::add_line(
			const Point<float>& pt
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (p_path) p_path->AddLine(
				Gdiplus::PointF(pt_last.x, pt_last.y),
				Gdiplus::PointF(pt.x, pt.y)
			);
		}


		// ベジェ曲線を追加
		void Path_Gdiplus::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (p_path) p_path->AddBezier(
				Gdiplus::PointF(pt_last.x, pt_last.y),
				Gdiplus::PointF(pt_0.x, pt_0.y),
				Gdiplus::PointF(pt_1.x, pt_1.y),
				Gdiplus::PointF(pt_2.x, pt_2.y)
			);
		}


		// 描画環境の用意
		bool Graphics_Gdiplus::startup()
		{
			Gdiplus::GdiplusStartupInput gsi;

			auto status = Gdiplus::GdiplusStartup(
				&gdiplus_token,
				&gsi,
				NULL
			);

			if (status == Gdiplus::Ok)
				return true;
			else return false;
		}


		// 描画環境の破棄
		void Graphics_Gdiplus::shutdown()
		{
			Gdiplus::GdiplusShutdown(gdiplus_token);
		}


		// Strokeの情報をPenに反映
		void Graphics_Gdiplus::apply_pen_style(Gdiplus::Pen* p_pen, const Color_F& col_f, const Stroke& stroke)
		{
			Color_I8 col_i8(col_f);
			p_pen->SetColor(
				Gdiplus::Color(
					col_i8.a,
					col_i8.r,
					col_i8.g,
					col_i8.b
				)
			);

			p_pen->SetWidth((Gdiplus::REAL)stroke.width);
			p_pen->SetStartCap((Gdiplus::LineCap)stroke.start_cap);
			p_pen->SetEndCap((Gdiplus::LineCap)stroke.end_cap);

			auto dash_cap = stroke.dash_cap;
			if (dash_cap == Stroke::Cap_Style::Square)
				dash_cap = Stroke::Cap_Style::Flat;
			p_pen->SetDashCap((Gdiplus::DashCap)dash_cap);

			p_pen->SetDashStyle((Gdiplus::DashStyle)stroke.dash_style);
			p_pen->SetDashOffset((Gdiplus::REAL)stroke.dash_offset);
			p_pen->SetDashPattern((Gdiplus::REAL*)stroke.custom_dashes, stroke.dashes_count);
			p_pen->SetLineJoin((Gdiplus::LineJoin)stroke.line_join);
		}


		// 初期化(インスタンス毎)
		bool Graphics_Gdiplus::init(HWND hwnd_)
		{
			hwnd = hwnd_;
			return true;
		}


		// 終了(インスタンス毎)
		void Graphics_Gdiplus::exit()
		{
			if (p_bitmap_buffer) {
				delete p_bitmap_buffer;
				p_bitmap_buffer = nullptr;
			}
		}


		// 描画開始
		void Graphics_Gdiplus::begin_draw()
		{
			delete p_graphics_buffer;
			delete p_bitmap_buffer;

			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);
			p_bitmap_buffer = new Gdiplus::Bitmap(rect_wnd.right, rect_wnd.bottom);
			p_graphics_buffer = new Gdiplus::Graphics(p_bitmap_buffer);
			p_graphics_buffer->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		}


		// 描画終了(&バッファ送信)
		bool Graphics_Gdiplus::end_draw()
		{
			if (p_graphics_buffer) {
				delete p_graphics_buffer;
				p_graphics_buffer = nullptr;

				if (p_bitmap_buffer) {
					PAINTSTRUCT ps;
					HDC hdc = ::BeginPaint(hwnd, &ps);
					Gdiplus::Graphics p_graphics(hdc);
					p_graphics.DrawImage(p_bitmap_buffer, 0, 0);

					::EndPaint(hwnd, &ps);

					delete p_bitmap_buffer;
					p_bitmap_buffer = nullptr;

					return true;
				}
				else return false;
			}
			else return false;
		}


		// 線を描画
		void Graphics_Gdiplus::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

				p_graphics_buffer->DrawLine(
					&pen,
					Gdiplus::PointF(pt_from.x, pt_from.y),
					Gdiplus::PointF(pt_to.x, pt_to.y)
				);
			}
		}


		// 線を描画(複数)
		void Graphics_Gdiplus::draw_lines(
			const Point<float>* points,
			size_t n_points,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

				auto gdiplus_points = new Gdiplus::PointF[n_points];

				for (size_t i = 0; i < n_points; i++) {
					gdiplus_points[i].X = points[i].x;
					gdiplus_points[i].Y = points[i].y;
				}

				p_graphics_buffer->DrawLines(
					&pen,
					gdiplus_points,
					(int)n_points
				);

				delete[] gdiplus_points;
			}
		}


		// ベジェ曲線を描画
		void Graphics_Gdiplus::draw_bezier(
			const Point<float>& point_0,
			const Point<float>& point_1,
			const Point<float>& point_2,
			const Point<float>& point_3,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, color_f, stroke);

				p_graphics_buffer->DrawBezier(
					&pen,
					Gdiplus::PointF(point_0.x, point_0.y),
					Gdiplus::PointF(point_1.x, point_1.y),
					Gdiplus::PointF(point_2.x, point_2.y),
					Gdiplus::PointF(point_3.x, point_3.y)
				);
			}
		}


		// ベジェ曲線を描画(複数)
		void Graphics_Gdiplus::draw_beziers(
			const Point<float>* points,
			size_t n_points,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, color_f, stroke);

				auto gdip_points = new Gdiplus::PointF[n_points];

				for (size_t i = 0; i < n_points; i++) {
					gdip_points[i].X = points[i].x;
					gdip_points[i].Y = points[i].y;
				}

				p_graphics_buffer->DrawBeziers(
					&pen,
					gdip_points,
					(int)n_points
				);

				delete[] gdip_points;
			}
		}


		// 矩形を描画(線)
		void Graphics_Gdiplus::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, color_f, stroke);
				
				// 角丸矩形を描画
				if (round_radius_x > 0 || round_radius_y > 0) {
					float angle = 180.f;
					Gdiplus::GraphicsPath path;

					// 半径のリミット
					round_radius_x = std::min(round_radius_x, std::abs(rect.right - rect.left) * 0.5f);
					round_radius_y = std::min(round_radius_y, std::abs(rect.bottom - rect.top) * 0.5f);

					// 左上
					path.AddArc(
						Gdiplus::RectF(
							std::min(rect.left, rect.right),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					// 右上
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF(
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					// 右下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF(
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					// 左下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF(
							std::min(rect.left, rect.right),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					path.CloseAllFigures();
					p_graphics_buffer->DrawPath(&pen, &path);
				}
				// 矩形を描画
				else {
					p_graphics_buffer->DrawRectangle(
						&pen,
						Gdiplus::RectF(
							rect.left,
							rect.top,
							rect.right - rect.left,
							rect.bottom - rect.top
						)
					);
				}
			}
		}


		// 矩形を描画(塗り)
		void Graphics_Gdiplus::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const Color_F& col_f
		)
		{
			if (p_graphics_buffer) {
				Color_I8 col_i8(col_f);
				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						(BYTE)col_i8.a,
						(BYTE)col_i8.r,
						(BYTE)col_i8.g,
						(BYTE)col_i8.b
					)
				);
				
				// 角丸矩形を描画
				if (round_radius_x > 0 || round_radius_y > 0) {
					float angle = 180.f;
					Gdiplus::GraphicsPath path;

					// 半径のリミット
					round_radius_x = std::min(round_radius_x, std::abs(rect.right - rect.left) * 0.5f);
					round_radius_y = std::min(round_radius_y, std::abs(rect.bottom - rect.top) * 0.5f);

					// 左上
					path.AddArc(
						Gdiplus::RectF(
							std::min(rect.left, rect.right),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					// 右上
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF(
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					// 右下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF(
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					// 左下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF(
							std::min(rect.left, rect.right),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						),
						angle,
						90.f
					);

					path.CloseAllFigures();
					p_graphics_buffer->FillPath(&brush, &path);
				}
				// 矩形を描画
				else {
					p_graphics_buffer->FillRectangle(
						&brush,
						Gdiplus::RectF(
							rect.left,
							rect.top,
							rect.right - rect.left,
							rect.bottom - rect.top
						)
					);
				}
			}
		}


		// 楕円を描画(線)(中心点指定)
		void Graphics_Gdiplus::draw_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, color_f, stroke);

				Gdiplus::RectF rect_f(
					point.x - radius_x,
					point.y - radius_y,
					point.x + radius_x,
					point.y + radius_y
				);

				p_graphics_buffer->DrawEllipse(&pen, rect_f);
			}
		}


		// 楕円を描画(線)(矩形指定)
		void Graphics_Gdiplus::draw_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, color_f, stroke);

				Gdiplus::RectF rect_f(
					rectangle.left,
					rectangle.top,
					rectangle.right,
					rectangle.bottom
				);

				p_graphics_buffer->DrawEllipse(&pen, rect_f);
			}
		}


		// 楕円を描画(塗り)(中心点指定)
		void Graphics_Gdiplus::fill_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& col_f
		)
		{
			if (p_graphics_buffer) {
				Color_I8 col_i8(col_f);
				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						(BYTE)col_i8.a,
						(BYTE)col_i8.r,
						(BYTE)col_i8.g,
						(BYTE)col_i8.b
					)
				);

				Gdiplus::RectF rect_f(
					point.x - radius_x,
					point.y - radius_y,
					point.x + radius_x,
					point.y + radius_y
				);

				p_graphics_buffer->FillEllipse(&brush, rect_f);
			}
		}


		// 楕円を描画(塗り)(矩形指定)
		void Graphics_Gdiplus::fill_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& col_f
		)
		{
			if (p_graphics_buffer) {
				Color_I8 col_i8(col_f);

				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						(BYTE)col_i8.a,
						(BYTE)col_i8.r,
						(BYTE)col_i8.g,
						(BYTE)col_i8.b
					)
				);

				Gdiplus::RectF rect_f(
					rectangle.left,
					rectangle.top,
					rectangle.right,
					rectangle.bottom
				);

				p_graphics_buffer->FillEllipse(&brush, rect_f);
			}
		}


		// 空のビットマップを作成
		void Graphics_Gdiplus::initialize_bitmap(
			Bitmap* p_bitmap,
			const Size<unsigned>& size,
			Color_F color_f
		)
		{
			p_bitmap->data = new Gdiplus::Bitmap(size.width, size.height);
		}


		// ファイルからビットマップを作成
		bool Graphics_Gdiplus::load_bitmap_from_filename(
			Bitmap* p_bitmap,
			const std::filesystem::path& path
		)
		{
			Gdiplus::Bitmap* p_gdip_bitmap = nullptr;
			p_gdip_bitmap = new Gdiplus::Bitmap(path.c_str());
			if (p_gdip_bitmap && p_gdip_bitmap->GetWidth() != 0 && p_gdip_bitmap->GetHeight() != 0) {
				p_bitmap->data = p_gdip_bitmap;
				return true;
			}
			else return false;
		}


		// リソースからビットマップを作成
		bool Graphics_Gdiplus::load_bitmap_from_resource(
			Bitmap* p_bitmap,
			HINSTANCE hinst,
			const char* res_name,
			const char* res_type
		)
		{
			wchar_t* wc = nullptr;
			// ビットマップ(GDI+)のポインタ
			Gdiplus::Bitmap* p_gdip_bitmap = nullptr;

			// リソースタイプがBITMAPのとき
			if (res_type == RT_BITMAP) {
				// リソース識別子が文字列の場合
				if (HIWORD(res_name)) {
					// const char* から wchar_t* に変換
					size_t size_resource = strlen(res_name) + 1;
					wc = new wchar_t[size_resource];

					size_t size;
					::mbstowcs_s(&size, wc, size_resource, res_name, size_resource);
				}
				// リソース識別子が数値の場合
				else
					wc = reinterpret_cast<wchar_t*>(const_cast<char*>(res_name));

				p_gdip_bitmap = Gdiplus::Bitmap::FromResource(hinst, wc);

				if (HIWORD(res_name)) delete[] wc;
			}
			// それ意外のとき
			else {
				// リソースのハンドル
				HRSRC res_handle = nullptr;
				// リソースのサイズ
				DWORD res_size = 0;
				// リソースデータのハンドル
				HGLOBAL res_data_handle = nullptr;
				// リソースデータ
				void* res_data = nullptr;
				// メモリ確保用リソースハンドル
				HGLOBAL res_buf_handle = nullptr;
				// 確保するメモリのポインタ(ここにリソースデータを配置する)
				void* res_buf_data = nullptr;
				// ストリームのポインタ
				IStream* p_stream = nullptr;

				// リソースを探す
				res_handle = ::FindResource(hinst, res_name, res_type);

				// リソースが見つかった場合
				if (res_handle)
					// リソースのサイズを取得
					res_size = ::SizeofResource(hinst, res_handle);

				// ここウイルス誤検知ポイント
				// リソースのサイズが存在する場合
				if (res_size)
					// リソースのハンドルを取得
					res_data_handle = ::LoadResource(hinst, res_handle);

				// リソースのハンドルが存在する場合
				if (res_data_handle)
					// リソースをロック(リソースのポインタを取得)
					// 実際にはメモリをロックしないらしい
					res_data = ::LockResource(res_data_handle);

				// リソースのポインタが存在する場合
				if (res_data)
					// メモリの確保
					res_buf_handle = ::GlobalAlloc(GMEM_MOVEABLE, res_size);

				// メモリが確保できた場合
				if (res_buf_handle)
					// メモリのロック
					res_buf_data = ::GlobalLock(res_buf_handle);

				// メモリがロックできた場合
				if (res_buf_data) {
					// ロックしたメモリ領域にリソースのデータをコピー
					::CopyMemory(res_buf_data, res_data, res_size);
					// メモリからストリームを作成
					auto hr = ::CreateStreamOnHGlobal(res_buf_handle, FALSE, &p_stream);

					// ストリームの作成に成功した場合
					if (SUCCEEDED(hr)) {
						// ストリームからビットマップを作成
						p_gdip_bitmap = Gdiplus::Bitmap::FromStream(p_stream);

						// ストリームを開放
						if (p_stream) p_stream->Release();
					}
				}

				// メモリの解除・開放
				if (res_buf_handle) {
					::GlobalUnlock(res_buf_handle);
					::GlobalFree(res_buf_handle);
				}
			}

			// ビットマップの作成に成功した場合
			if (p_gdip_bitmap && p_gdip_bitmap->GetLastStatus() == Gdiplus::Ok) {
				p_bitmap->data = p_gdip_bitmap;
				return true;
			}
			else return false;
		}


		// ビットマップを描画(アンカーポイント指定)
		void Graphics_Gdiplus::draw_bitmap(
			const Bitmap* bitmap,
			const Point<float>& point,
			Anchor_Position anchor_pos,
			float opacity
		)
		{
			auto p_gdip_bitmap = reinterpret_cast<Gdiplus::Bitmap*>(bitmap->data);
			Gdiplus::RectF rect_f;
			int width = p_gdip_bitmap->GetWidth();
			int height = p_gdip_bitmap->GetHeight();

			// アンカーポイントの位置
			switch (anchor_pos) {
			case Anchor_Position::Center:
			default:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Left:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Top:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Right:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Bottom:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Left_Top:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Right_Top:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Left_Bottom:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Right_Bottom:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;
			}

			p_graphics_buffer->DrawImage(
				p_gdip_bitmap,
				rect_f
			);
		}


		// ビットマップを描画(矩形指定)
		void Graphics_Gdiplus::draw_bitmap(
			const Bitmap* bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			p_graphics_buffer->DrawImage(
				reinterpret_cast<Gdiplus::Bitmap*>(bitmap->data),
				Gdiplus::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				)
			);
		}
	}
}