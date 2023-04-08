//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_directx.hpp"



namespace mkaul {
	namespace graphics {
		ID2D1Factory* Graphics_Directx::p_factory = nullptr;
		IDWriteFactory* Graphics_Directx::p_write_factory = nullptr;
		IWICImagingFactory* Graphics_Directx::p_imaging_factory = nullptr;

		// •`‰æŠÂ‹«‚Ì—pˆÓ
		bool Graphics_Directx::startup()
		{
			HRESULT hresult;

			// ID2D1Factory‚Ìì¬
			hresult = ::D2D1CreateFactory(
				D2D1_FACTORY_TYPE_MULTI_THREADED,
				&p_factory
			);
			if (FAILED(hresult))
				return false;

			// IWICImagingFactory‚Ìì¬
			hresult = ::CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				reinterpret_cast<void**>(&p_imaging_factory)
			);
			if (FAILED(hresult))
				return false;

			// IDWriteFactory‚Ìì¬
			hresult = ::DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&p_write_factory)
			);
			if (FAILED(hresult))
				return false;

			return true;
		}


		// •`‰æŠÂ‹«‚Ì”jŠü
		void Graphics_Directx::shutdown()
		{
			release(&p_imaging_factory);
			release(&p_write_factory);
			release(&p_factory);
		}


		// Stroke -> ID2D1StrokeStyle*
		void Graphics_Directx::stroke_to_d2d_strokestyle(
			const Stroke& stroke,
			ID2D1StrokeStyle** pp_stroke_style
		)
		{
			if (p_factory) {
				p_factory->CreateStrokeStyle(
					D2D1::StrokeStyleProperties(
						(D2D1_CAP_STYLE)stroke.start_cap,
						(D2D1_CAP_STYLE)stroke.end_cap,
						(D2D1_CAP_STYLE)stroke.dash_cap,
						(D2D1_LINE_JOIN)stroke.line_join,
						10.f,
						(D2D1_DASH_STYLE)stroke.dash_style,
						stroke.dash_offset
					),
					stroke.custom_dashes,
					stroke.dashes_count,
					pp_stroke_style
				);
			}
		}


		// ‰Šú‰»
		bool Graphics_Directx::init_instance(HWND hwnd)
		{
			HRESULT hresult;
			D2D1_SIZE_U pixel_size;
			RECT rect;

			::GetClientRect(hwnd, &rect);

			pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
			};

			if (!p_factory || !p_write_factory || !p_imaging_factory)
				return false;

			// HwndRenderTarget‚ğì¬
			hresult = p_factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, pixel_size),
				&p_render_target
			);

			if (FAILED(hresult) || !p_render_target)
				return false;

			// ƒuƒ‰ƒV‚ğì¬
			if (!p_brush)
				p_render_target->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &p_brush);

			return true;
		}


		// I—¹
		inline void Graphics_Directx::exit_instance()
		{
			release(&p_brush);
			release(&p_render_target);
		}


		// •`‰æŠJn
		inline void Graphics_Directx::begin_draw(HWND hwnd)
		{
			auto hdc = ::BeginPaint(hwnd, &ps);
			p_render_target->BeginDraw();
		}


		// •`‰æI—¹
		inline bool Graphics_Directx::end_draw(HWND hwnd)
		{
			HRESULT hresult = p_render_target->EndDraw();
			::EndPaint(hwnd, &ps);
			if (FAILED(hresult))
				return false;
			else return true;
		}


		// ƒŠƒTƒCƒY
		void Graphics_Directx::resize(HWND hwnd)
		{
			RECT rect;
			::GetClientRect(hwnd, &rect);
			D2D1_SIZE_U pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
			};
			if (p_render_target)
				p_render_target->Resize(pixel_size);
		}


		// ü‚ğ•`‰æ
		void Graphics_Directx::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			ID2D1StrokeStyle* stroke_style;
			stroke_to_d2d_strokestyle(stroke, &stroke_style);

			if (p_render_target) {
				p_brush->SetColor(color_f.d2d1_colorf());

				p_render_target->DrawLine(
					D2D1::Point2F(pt_from.x, pt_from.y),
					D2D1::Point2F(pt_to.x, pt_to.y),
					p_brush,
					stroke.width,
					stroke_style
				);

				release(&stroke_style);
			}
		}


		// ü‚ğ•`‰æ(•¡”)
		void Graphics_Directx::draw_lines(
			Point<float>* p_points,
			size_t n_points,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_render_target) {
				p_brush->SetColor(col_f.d2d1_colorf());

				p_render_target->DrawLine(
					D2D1::Point2F(pt_from.x, pt_from.y),
					D2D1::Point2F(pt_to.x, pt_to.y),
					p_brush,
					stroke.width,
					stroke_style
				);
			}
		}


		// ‹éŒ`‚ğ•`‰æ(ü)
		void Graphics_Directx::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x = 0.f,
			float round_radius_y = 0.f,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			if (p_render_target) {
				ID2D1StrokeStyle* stroke_style;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);
				p_brush->SetColor(color_f.d2d1_colorf());

				const D2D1_RECT_F rect_f = D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				);

				// ŠpŠÛ‹éŒ`‚ğ•`‰æ
				if (round_radius_x > 0 || round_radius_y > 0) {
					D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(
						rect_f,
						round_radius_x,
						round_radius_y
					);
					
					p_render_target->DrawRoundedRectangle(
						rounded_rect,
						p_brush,
						stroke.width,
						stroke_style
					);
				}
				// ‹éŒ`‚ğ•`‰æ
				else {
					p_render_target->DrawRectangle(
						rect_f,
						p_brush,
						stroke.width,
						stroke_style
					);
				}

				release(&stroke_style);
			}
		}


		// ‹éŒ`‚ğ•`‰æ(“h‚è)
		void Graphics_Directx::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x = 0.f,
			float round_radius_y = 0.f,
			const Color_F& color_f = 0
		)
		{
			if (p_render_target) {
				p_brush->SetColor(color_f.d2d1_colorf());

				const D2D1_RECT_F rect_f = D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				);

				// ŠpŠÛ‹éŒ`‚ğ•`‰æ
				if (round_radius_x > 0 || round_radius_y > 0) {
					D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(
						rect_f,
						round_radius_x,
						round_radius_y
					);

					p_render_target->FillRoundedRectangle(
						rounded_rect,
						p_brush
					);
				}
				// ‹éŒ`‚ğ•`‰æ
				else {
					p_render_target->FillRectangle(
						rect_f,
						p_brush
					);
				}
			}
		}


		// ‘È‰~‚ğ•`‰æ(ü)(’†S“_w’è)
		void Graphics_Directx::draw_ellipse(
			const Point<float>& pt,
			float radius_x,
			float radius_y,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			if (p_render_target) {
				ID2D1StrokeStyle* stroke_style;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush->SetColor(color_f.d2d1_colorf());

				p_render_target->DrawEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(pt.x, pt.y),
						radius_x,
						radius_y
					),
					p_brush,
					stroke.width,
					stroke_style
				);

				release(&stroke_style);
			}
		}


		// ‘È‰~‚ğ•`‰æ(ü)(‹éŒ`w’è)
		void Graphics_Directx::draw_ellipse(
			const Rectangle<float>& rect,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_render_target) {
				ID2D1StrokeStyle* stroke_style;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush->SetColor(color_f.d2d1_colorf());

				p_render_target->DrawEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(
							(rect.left + rect.right) * 0.5f,
							(rect.top + rect.bottom) * 0.5f
						),
						(rect.right - rect.left) * 0.5f,
						(rect.bottom - rect.top) * 0.5f
					),
					p_brush,
					stroke.width,
					stroke_style
				);

				release(&stroke_style);
			}
		}


		// ‘È‰~‚ğ•`‰æ(“h‚è)(’†S“_w’è)
		void Graphics_Directx::fill_ellipse(
			const Point<float>& pt,
			const Color_F& color_f,
			float radius_x,
			float radius_y
		)
		{
			if (p_render_target) {
				p_brush->SetColor(color_f.d2d1_colorf());

				p_render_target->FillEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(pt.x, pt.y),
						radius_x,
						radius_y
					),
					p_brush
				);
			}
		}


		// ‘È‰~‚ğ•`‰æ(“h‚è)(‹éŒ`w’è)
		void Graphics_Directx::fill_ellipse(
			const Rectangle<float>& rect,
			const Color_F& color_f
		)
		{
			if (p_render_target) {
				p_brush->SetColor(color_f.d2d1_colorf());

				p_render_target->FillEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(
							(rect.left + rect.right) * 0.5f,
							(rect.top + rect.bottom) * 0.5f
						),
						(rect.right - rect.left) * 0.5f,
						(rect.bottom - rect.top) * 0.5f
					),
					p_brush
				);
			}
		}
	}
}