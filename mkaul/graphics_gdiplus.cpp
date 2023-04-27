//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics_gdiplus.hpp"



namespace mkaul {
	namespace graphics {
		void Bitmap_Gdiplus::create(size_t width_, size_t height_)
		{
			p_bitmap = new Gdiplus::Bitmap((int)width_, (int)height_);
		}

		void Bitmap_Gdiplus::release()
		{
			delete p_bitmap;
		}

		size_t Bitmap_Gdiplus::get_width()
		{
			return p_bitmap->GetWidth();
		}

		size_t Bitmap_Gdiplus::get_height()
		{
			return p_bitmap->GetHeight();
		}


		ULONG_PTR Graphics_Gdiplus::gdiplus_token = NULL;

		// `ζΒ«ΜpΣ
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

		// `ζΒ«Μjό
		inline void Graphics_Gdiplus::shutdown()
		{
			Gdiplus::GdiplusShutdown(gdiplus_token);
		}

		// StrokeΜξρπPenΙ½f
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

		// ϊ»(CX^X)
		inline bool Graphics_Gdiplus::init(HWND hwnd)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);
			p_bitmap_buffer = new Gdiplus::Bitmap(rect_wnd.right, rect_wnd.bottom);
		}

		// IΉ(CX^X)
		inline void Graphics_Gdiplus::exit()
		{
			if (p_bitmap_buffer) {
				delete p_bitmap_buffer;
				p_bitmap_buffer = nullptr;
			}
		}

		// `ζJn
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

		// `ζIΉ(&obt@M)
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


		// όπ`ζ
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


		// όπ`ζ(‘)
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

				auto gdiplus_points = new Gdiplus::Point[n_points];

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


		// xWFΘόπ`ζ
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


		// xWFΘόπ`ζ(‘)
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

				auto gdiplus_points = new Gdiplus::Point[n_points];

				for (size_t i = 0; i < n_points; i++) {
					gdiplus_points[i].X = points[i].x;
					gdiplus_points[i].Y = points[i].y;
				}

				p_graphics_buffer->DrawBeziers(
					&pen,
					gdiplus_points,
					(int)n_points
				);

				delete[] gdiplus_points;
			}
		}


		// ι`π`ζ(ό)
		void Graphics_Gdiplus::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x = 0,
			float round_radius_y = 0,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(0);
				apply_pen_style(&pen, color_f, stroke);
				
				// pΫι`π`ζ
				if (round_radius_x > 0 || round_radius_y > 0) {

				}
				// ι`π`ζ
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


		// ι`π`ζ(hθ)
		void Graphics_Gdiplus::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x = 0,
			float round_radius_y = 0,
			const Color_F& color_f = 0
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						color_f.a,
						color_f.r,
						color_f.g,
						color_f.b
					)
				);
				
				// pΫι`π`ζ
				if (round_radius_x > 0 || round_radius_y > 0) {

				}
				// ι`π`ζ
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


		// Θ~π`ζ(ό)(S_wθ)
		void Graphics_Gdiplus::draw_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
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


		// Θ~π`ζ(ό)(ι`wθ)
		void Graphics_Gdiplus::draw_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
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


		// Θ~π`ζ(hθ)(S_wθ)
		void Graphics_Gdiplus::fill_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& color_f
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						color_f.a,
						color_f.r,
						color_f.g,
						color_f.b
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


		// Θ~π`ζ(hθ)(ι`wθ)
		void Graphics_Gdiplus::fill_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& color_f
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						color_f.a,
						color_f.r,
						color_f.g,
						color_f.b
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
	}
}