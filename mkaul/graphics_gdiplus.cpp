//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "common.hpp"
#include "graphics_gdiplus.hpp"



namespace mkaul {
	namespace graphics {
		void GdiplusBitmap::release()
		{
			gdip_release(reinterpret_cast<Gdiplus::Bitmap**>(&data));
		}


		size_t GdiplusBitmap::get_width() const
		{
			return reinterpret_cast<Gdiplus::Bitmap*>(data)->GetWidth();
		}


		size_t GdiplusBitmap::get_height() const
		{
			return reinterpret_cast<Gdiplus::Bitmap*>(data)->GetHeight();
		}


		// 開放
		void GdiplusPath::release()
		{
			gdip_release(reinterpret_cast<Gdiplus::GraphicsPath**>(&data[0]));
		}


		// パスの開始
		bool GdiplusPath::begin(const Point<float>& pt)
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
		void GdiplusPath::end(FigureEnd fe)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (fe == FigureEnd::Closed)
				p_path->CloseAllFigures();
		}


		// 弧を追加
		void GdiplusPath::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);
			Point<float> pt_ofs_start, pt_ofs_end, pt_o;

			// 角度を-360d ~ 360dの範囲に収める
			angle_sweep = std::fmodf(angle_sweep, 360.f);

			ellipse_pos(size, angle_start, &pt_ofs_start);
			ellipse_pos(size, angle_start + angle_sweep, &pt_ofs_end);
			pt_o = pt_last - pt_ofs_start;
			// 最後の点を更新
			pt_last = pt_o + pt_ofs_end;

			// 座標のずれ補正
			if (p_path) p_path->AddArc(
				Gdiplus::RectF(
					pt_o.x - size.width - 0.5f,
					pt_o.y - size.height - 0.5f,
					size.width * 2.f,
					size.height * 2.f
				),
				angle_start - 90.f,
				angle_sweep
			);
		}


		// 線を追加
		void GdiplusPath::add_line(
			const Point<float>& pt
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			// 座標のずれ補正
			if (p_path) p_path->AddLine(
				Gdiplus::PointF(pt_last.x - 0.5f, pt_last.y - 0.5f),
				Gdiplus::PointF(pt.x - 0.5f, pt.y - 0.5f)
			);
			// 最後の点を更新
			pt_last = pt;
		}


		// ベジェ曲線を追加
		void GdiplusPath::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			// 座標のずれ補正
			if (p_path) p_path->AddBezier(
				Gdiplus::PointF(pt_last.x - 0.5f, pt_last.y - 0.5f),
				Gdiplus::PointF(pt_0.x - 0.5f, pt_0.y - 0.5f),
				Gdiplus::PointF(pt_1.x - 0.5f, pt_1.y - 0.5f),
				Gdiplus::PointF(pt_2.x - 0.5f, pt_2.y - 0.5f)
			);
			// 最後の点を更新
			pt_last = pt_2;
		}


		// 描画環境の用意
		bool GdiplusGraphics::startup()
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
		void GdiplusGraphics::shutdown()
		{
			Gdiplus::GdiplusShutdown(gdiplus_token);
		}


		// Strokeの情報をPenに反映
		void GdiplusGraphics::apply_pen_style(Gdiplus::Pen* p_pen, const ColorF& col_f, const Stroke& stroke)
		{
			ColorI8 col_i8(col_f);
			p_pen->SetColor(
				Gdiplus::Color(
					(BYTE)col_i8.a,
					(BYTE)col_i8.r,
					(BYTE)col_i8.g,
					(BYTE)col_i8.b
				)
			);

			p_pen->SetWidth((Gdiplus::REAL)stroke.width);
			p_pen->SetStartCap((Gdiplus::LineCap)stroke.start_cap);
			p_pen->SetEndCap((Gdiplus::LineCap)stroke.end_cap);

			auto dash_cap = stroke.dash_cap;
			if (dash_cap == Stroke::CapStyle::Square)
				dash_cap = Stroke::CapStyle::Flat;
			p_pen->SetDashCap((Gdiplus::DashCap)dash_cap);

			p_pen->SetDashStyle((Gdiplus::DashStyle)stroke.dash_style);
			p_pen->SetDashOffset((Gdiplus::REAL)stroke.dash_offset);
			p_pen->SetDashPattern((Gdiplus::REAL*)stroke.custom_dashes, stroke.dashes_count);
			p_pen->SetLineJoin((Gdiplus::LineJoin)stroke.line_join);
		}


		// 初期化(インスタンス毎)
		bool GdiplusGraphics::init(HWND hwnd_)
		{
			if (::IsWindow(hwnd_)) {
				hwnd = hwnd_;
				return true;
			}
			else return false;
		}


		// 終了(インスタンス毎)
		void GdiplusGraphics::exit()
		{
			gdip_release(&p_graphics_buffer);
			gdip_release(&p_bitmap_buffer);
		}


		// 描画開始
		bool GdiplusGraphics::begin_draw()
		{
			if (!drawing) {
				gdip_release(&p_graphics_buffer);
				gdip_release(&p_bitmap_buffer);

				RECT rect;

				if (::GetClientRect(hwnd, &rect)) {
					p_bitmap_buffer = new Gdiplus::Bitmap(rect.right, rect.bottom);
					p_graphics_buffer = new Gdiplus::Graphics(p_bitmap_buffer);
				}

				if (p_bitmap_buffer && p_graphics_buffer) {
					p_graphics_buffer->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
					drawing = true;

					return true;
				}
				else {
					gdip_release(&p_graphics_buffer);
					gdip_release(&p_bitmap_buffer);
				}
			}
			return false;
		}


		// 描画終了(&バッファ送信)
		bool GdiplusGraphics::end_draw()
		{
			bool result = false;

			if (drawing) {
				if (p_graphics_buffer && p_bitmap_buffer) {
					PAINTSTRUCT ps;
					HDC hdc = ::BeginPaint(hwnd, &ps);
					Gdiplus::Graphics p_graphics(hdc);
					p_graphics.DrawImage(p_bitmap_buffer, 0, 0);

					::EndPaint(hwnd, &ps);
					drawing = false;

					result = true;
				}
				else result = false;

				gdip_release(&p_graphics_buffer);
				gdip_release(&p_bitmap_buffer);
			}
			return result;
		}


		// 背景を塗りつぶし
		void GdiplusGraphics::fill_background(
			const ColorF& col_f
		)
		{
			if (drawing && p_graphics_buffer) {
				ColorI8 col_i8(col_f);
				RECT rect;

				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						(BYTE)col_i8.a,
						(BYTE)col_i8.r,
						(BYTE)col_i8.g,
						(BYTE)col_i8.b
					)
				);

				if (::GetClientRect(hwnd, &rect)) {
					// 境界付近が切れるので広めにとる
					p_graphics_buffer->FillRectangle(
						&brush,
						Gdiplus::Rect(
							rect.left - 1,
							rect.top - 1,
							rect.right + 1,
							rect.bottom + 1
						)
					);
				}
			}
		}


		// 線を描画
		void GdiplusGraphics::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
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
		void GdiplusGraphics::draw_lines(
			const Point<float>* points,
			size_t n_points,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
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
		void GdiplusGraphics::draw_bezier(
			const Point<float>& point_0,
			const Point<float>& point_1,
			const Point<float>& point_2,
			const Point<float>& point_3,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

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
		void GdiplusGraphics::draw_beziers(
			const Point<float>* points,
			size_t n_points,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

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
		void GdiplusGraphics::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);
				
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
		void GdiplusGraphics::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const ColorF& col_f
		)
		{
			if (drawing && p_graphics_buffer) {
				ColorI8 col_i8(col_f);
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
		void GdiplusGraphics::draw_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

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
		void GdiplusGraphics::draw_ellipse(
			const Rectangle<float>& rectangle,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

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
		void GdiplusGraphics::fill_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const ColorF& col_f
		)
		{
			if (drawing && p_graphics_buffer) {
				ColorI8 col_i8(col_f);
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
		void GdiplusGraphics::fill_ellipse(
			const Rectangle<float>& rectangle,
			const ColorF& col_f
		)
		{
			if (drawing && p_graphics_buffer) {
				ColorI8 col_i8(col_f);

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


		// パスを描画(線)
		void GdiplusGraphics::draw_path(
			const Path* p_path,
			const ColorF& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

				p_graphics_buffer->DrawPath(
					&pen,
					p_path->get_data<Gdiplus::GraphicsPath*>(0)
				);
			}
		}


		// パスを描画(塗り)
		void GdiplusGraphics::fill_path(
			const Path* p_path,
			const ColorF& col_f
		)
		{
			if (drawing && p_graphics_buffer) {
				ColorI8 col_i8(col_f);

				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						(BYTE)col_i8.a,
						(BYTE)col_i8.r,
						(BYTE)col_i8.g,
						(BYTE)col_i8.b
					)
				);

				p_graphics_buffer->FillPath(
					&brush,
					p_path->get_data<Gdiplus::GraphicsPath*>(0)
				);
			}
		}


		// 空のビットマップを作成
		bool GdiplusGraphics::initialize_bitmap(
			Bitmap* p_bitmap,
			const Size<unsigned>& size,
			ColorF col_f
		)
		{
			auto p_gdip_bitmap = new Gdiplus::Bitmap(size.width, size.height);
			if (p_gdip_bitmap && p_gdip_bitmap->GetLastStatus() == Gdiplus::Ok) {
				p_bitmap->set_data(p_gdip_bitmap);
				return true;
			}
			else return false;
		}


		// ファイルからビットマップを作成
		bool GdiplusGraphics::load_bitmap_from_filename(
			Bitmap* p_bitmap,
			const std::filesystem::path& path
		)
		{
			Gdiplus::Bitmap* p_gdip_bitmap = nullptr;
			p_gdip_bitmap = new Gdiplus::Bitmap(path.c_str());
			if (p_gdip_bitmap && p_gdip_bitmap->GetWidth() != 0 && p_gdip_bitmap->GetHeight() != 0) {
				p_bitmap->set_data(p_gdip_bitmap);
				return true;
			}
			else return false;
		}


		// リソースからビットマップを作成
		bool GdiplusGraphics::load_bitmap_from_resource(
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
				p_bitmap->set_data(p_gdip_bitmap);
				return true;
			}
			else return false;
		}


		// ビットマップを描画(アンカーポイント指定)
		void GdiplusGraphics::draw_bitmap(
			const Bitmap* bitmap,
			const Point<float>& point,
			AnchorPosition anchor_pos,
			float opacity
		)
		{
			auto p_gdip_bitmap = bitmap->get_data<Gdiplus::Bitmap*>();
			Gdiplus::RectF rect_f;
			int width = p_gdip_bitmap->GetWidth();
			int height = p_gdip_bitmap->GetHeight();

			// アンカーポイントの位置
			switch (anchor_pos) {
			case AnchorPosition::Center:
			default:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::Left:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::Top:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::Right:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::Bottom:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::LeftTop:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::RightTop:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::LeftBottom:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case AnchorPosition::RightBottom:
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
		void GdiplusGraphics::draw_bitmap(
			const Bitmap* bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			p_graphics_buffer->DrawImage(
				bitmap->get_data<Gdiplus::Bitmap*>(),
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