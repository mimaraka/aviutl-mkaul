//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_gdiplus.hpp"



namespace mkaul {
	namespace graphics {
		ULONG_PTR Graphics_Gdiplus::gdiplus_token = NULL;

		// •`‰æŠÂ‹«‚Ì—pˆÓ
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

		// •`‰æŠÂ‹«‚Ì”jŠü
		inline void Graphics_Gdiplus::shutdown()
		{
			Gdiplus::GdiplusShutdown(gdiplus_token);
		}

		// Stroke‚Ìî•ñ‚ğPen‚É”½‰f
		inline void Graphics_Gdiplus::apply_pen_style(Gdiplus::Pen* p_pen, const Color_I8& col_i8, const Stroke& stroke)
		{
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

		// ‰Šú‰»(ƒCƒ“ƒXƒ^ƒ“ƒX–ˆ)
		inline bool Graphics_Gdiplus::init_instance(HWND hwnd)
		{
			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);
			p_bitmap_buffer = new Gdiplus::Bitmap(rect_wnd.right, rect_wnd.bottom);
		}

		// I—¹(ƒCƒ“ƒXƒ^ƒ“ƒX–ˆ)
		inline bool Graphics_Gdiplus::exit_instance()
		{
			if (p_bitmap_buffer) {
				delete p_bitmap_buffer;
				p_bitmap_buffer = nullptr;
				return true;
			}
			else return false;
		}

		// •`‰æŠJn
		inline void Graphics_Gdiplus::begin_draw(HWND hwnd)
		{
			delete p_graphics_buffer;
			delete p_bitmap_buffer;

			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);
			p_bitmap_buffer = new Gdiplus::Bitmap(rect_wnd.right, rect_wnd.bottom);
			p_graphics_buffer = new Gdiplus::Graphics(p_bitmap_buffer);
			p_graphics_buffer->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		}

		// •`‰æI—¹(&ƒoƒbƒtƒ@‘—M)
		inline bool Graphics_Gdiplus::end_draw(HWND hwnd)
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


		// ü‚ğ•`‰æ
		void Graphics_Gdiplus::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const Color_I8& col_i8,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(0);
				apply_pen_style(&pen, col_i8, stroke);

				p_graphics_buffer->DrawLine(
					&pen,
					Gdiplus::PointF(pt_from.x, pt_from.y),
					Gdiplus::PointF(pt_to.x, pt_to.y)
				);
			}
		}


		// ü‚ğ•`‰æ(•¡”)
		void Graphics_Gdiplus::draw_lines(
			const Point<float>* p_pt,
			size_t n_points,
			const Color_I8& col_i8,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(0);
				apply_pen_style(&pen, col_i8, stroke);

				auto pts = new Gdiplus::Point[n_points];

				for (size_t i = 0; i < n_points; i++) {
					pts[i].X = p_pt[i].x;
					pts[i].Y = p_pt[i].y;
				}

				p_graphics_buffer->DrawLines(
					&pen,
					pts,
					(int)n_points
				);

				delete pts;
			}
		}


		// ‹éŒ`‚ğ•`‰æ(ü)
		void Graphics_Gdiplus::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x = 0,
			float round_radius_y = 0,
			const Color_I8& col_i8 = 0,
			const Stroke& stroke = Stroke()
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(0);
				apply_pen_style(&pen, col_i8, stroke);
				
				// ŠpŠÛ‹éŒ`‚ğ•`‰æ
				if (round_radius_x > 0 || round_radius_y > 0) {

				}
				// ‹éŒ`‚ğ•`‰æ
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


		// ‹éŒ`‚ğ•`‰æ(“h‚è)
		void Graphics_Gdiplus::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x = 0,
			float round_radius_y = 0,
			const Color_I8& col_i8 = 0
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::SolidBrush brush(
					Gdiplus::Color(
						col_i8.a,
						col_i8.r,
						col_i8.g,
						col_i8.b
					)
				);
				
				// ŠpŠÛ‹éŒ`‚ğ•`‰æ
				if (round_radius_x > 0 || round_radius_y > 0) {

				}
				// ‹éŒ`‚ğ•`‰æ
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
	}
}