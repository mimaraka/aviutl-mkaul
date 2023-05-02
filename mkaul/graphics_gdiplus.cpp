//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

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


		ULONG_PTR Graphics_Gdiplus::gdiplus_token = NULL;

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
			else
				return false;
		}


		// 描画環境の破棄
		inline void Graphics_Gdiplus::shutdown()
		{
			Gdiplus::GdiplusShutdown(gdiplus_token);
		}


		// Strokeの情報をPenに反映
		inline void Graphics_Gdiplus::apply_pen_style(Gdiplus::Pen* p_pen, const Color_F& col_f, const Stroke& stroke)
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
		inline bool Graphics_Gdiplus::init(HWND hwnd)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);
			p_bitmap_buffer = new Gdiplus::Bitmap(rect_wnd.right, rect_wnd.bottom);
		}


		// 終了(インスタンス毎)
		inline void Graphics_Gdiplus::exit()
		{
			if (p_bitmap_buffer) {
				delete p_bitmap_buffer;
				p_bitmap_buffer = nullptr;
			}
		}


		// 描画開始
		inline void Graphics_Gdiplus::begin_draw()
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
		inline bool Graphics_Gdiplus::end_draw()
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
				Gdiplus::Pen pen(0);
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
				Gdiplus::Pen pen(0);
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
				Gdiplus::Pen pen(0);
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
				Gdiplus::Pen pen(0);
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
				Gdiplus::Pen pen(0);
				apply_pen_style(&pen, color_f, stroke);
				
				// 角丸矩形を描画
				if (round_radius_x > 0 || round_radius_y > 0) {

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
				Gdiplus::Pen pen(0);
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
				Gdiplus::Pen pen(0);
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
			const char* resource
		)
		{
			wchar_t* wc;
			size_t size_resource = strlen(resource) + 1;
			// char -> wchar_t
			if (HIWORD(resource)) {
				wc = new wchar_t[size_resource];
				size_t size;
				::mbstowcs_s(&size, wc, size_resource, resource, size_resource);
			}
			// 上位ワードが0(識別子が16ビット数値の場合)
			else {
				wc = (wchar_t*)resource;
			}

			Gdiplus::Bitmap* p_gdip_bitmap = nullptr;
			p_gdip_bitmap = Gdiplus::Bitmap::FromResource(::GetModuleHandle(NULL), wc);

			if (HIWORD(resource)) delete[] wc;

			if (p_gdip_bitmap && p_gdip_bitmap->GetWidth() != 0 && p_gdip_bitmap->GetHeight() != 0) {
				p_bitmap->data = p_gdip_bitmap;
				return true;
			}
			else return false;
		}


		// ビットマップを描画(アンカーポイント指定)
		void Graphics_Gdiplus::draw_bitmap(
			const Bitmap& bitmap,
			const Point<float>& point,
			Anchor_Position anchor_pos,
			float opacity
		)
		{
			auto p_gdip_bitmap = reinterpret_cast<Gdiplus::Bitmap*>(bitmap.data);
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
					point.x + width * 0.5f,
					point.y + height * 0.5f
				);
				break;

			case Anchor_Position::Left:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height * 0.5f,
					point.x + width,
					point.y + height * 0.5f
				);
				break;

			case Anchor_Position::Top:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y,
					point.x + width * 0.5f,
					point.y + height
				);
				break;

			case Anchor_Position::Right:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y - height * 0.5f,
					point.x,
					point.y + height * 0.5f
				);
				break;

			case Anchor_Position::Bottom:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height,
					point.x + width * 0.5f,
					point.y
				);
				break;

			case Anchor_Position::Left_Top:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y,
					point.x + width,
					point.y + height
				);
				break;

			case Anchor_Position::Right_Top:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y,
					point.x,
					point.y + height
				);
				break;

			case Anchor_Position::Left_Bottom:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height,
					point.x + width,
					point.y
				);
				break;

			case Anchor_Position::Right_Bottom:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y - height,
					point.x,
					point.y
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
			const Bitmap& bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			p_graphics_buffer->DrawImage(
				reinterpret_cast<Gdiplus::Bitmap*>(bitmap.data),
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