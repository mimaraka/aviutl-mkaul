//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics_directx.hpp"



namespace mkaul {
	namespace graphics {
		void Bitmap_Directx::release()
		{
			p_bitmap->Release();
		}

		size_t Bitmap_Directx::get_width()
		{
			return (p_bitmap->GetPixelSize()).width;
		}

		size_t Bitmap_Directx::get_height()
		{
			return (p_bitmap->GetPixelSize()).height;
		}



		// �`����̗p��
		bool Graphics_Directx::startup()
		{
			HRESULT hr;

			// ID2D1Factory�̍쐬
			hr = ::D2D1CreateFactory(
				D2D1_FACTORY_TYPE_MULTI_THREADED,
				&p_factory
			);
			if (FAILED(hr))
				return false;

			// IWICImagingFactory�̍쐬
			hr = ::CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				reinterpret_cast<void**>(&p_imaging_factory)
			);
			if (FAILED(hr))
				return false;

			// IDWriteFactory�̍쐬
			hr = ::DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&p_write_factory)
			);
			if (FAILED(hr))
				return false;

			return true;
		}


		// �`����̔j��
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


		// �t�@�N�g���[�擾
		auto Graphics_Directx::get_factory()
		{
			return p_factory;
		}

		auto Graphics_Directx::get_write_factory()
		{
			return p_write_factory;
		}

		auto Graphics_Directx::get_imaging_factory()
		{
			return p_imaging_factory;
		}

		auto Graphics_Directx::get_render_target()
		{
			return p_render_target;
		}


		// ������
		bool Graphics_Directx::init(HWND hwnd_)
		{
			HRESULT hresult;
			D2D1_SIZE_U pixel_size;
			RECT rect;

			hwnd = hwnd_;

			::GetClientRect(hwnd, &rect);

			pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
			};

			if (!p_factory || !p_write_factory || !p_imaging_factory)
				return false;

			// HwndRenderTarget���쐬
			hresult = p_factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, pixel_size),
				&p_render_target
			);

			if (FAILED(hresult) || !p_render_target)
				return false;

			// �u���V���쐬
			if (!p_brush)
				p_render_target->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &p_brush);

			return true;
		}


		// �I��
		inline void Graphics_Directx::exit()
		{
			release(&p_brush);
			release(&p_render_target);
		}


		// �`��J�n
		inline void Graphics_Directx::begin_draw()
		{
			auto hdc = ::BeginPaint(hwnd, &ps);
			p_render_target->BeginDraw();
		}


		// �`��I��
		inline bool Graphics_Directx::end_draw()
		{
			HRESULT hresult = p_render_target->EndDraw();
			::EndPaint(hwnd, &ps);
			if (FAILED(hresult))
				return false;
			else return true;
		}


		// ���T�C�Y
		void Graphics_Directx::resize()
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


		// ����`��
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


		// ����`��(����)
		void Graphics_Directx::draw_lines(
			const Point<float>* p_points,
			size_t n_points,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			if (p_render_target) {
				p_brush->SetColor(color_f.d2d1_colorf());

				p_render_target->DrawLine(
					D2D1::Point2F(pt_from.x, pt_from.y),
					D2D1::Point2F(pt_to.x, pt_to.y),
					p_brush,
					stroke.width,
					stroke_style
				);
			}
		}


		// ��`��`��(��)
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

				// �p�ۋ�`��`��
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
				// ��`��`��
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


		// ��`��`��(�h��)
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

				// �p�ۋ�`��`��
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
				// ��`��`��
				else {
					p_render_target->FillRectangle(
						rect_f,
						p_brush
					);
				}
			}
		}


		// �ȉ~��`��(��)(���S�_�w��)
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


		// �ȉ~��`��(��)(��`�w��)
		void Graphics_Directx::draw_ellipse(
			const Rectangle<float>& rect,
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


		// �ȉ~��`��(�h��)(���S�_�w��)
		void Graphics_Directx::fill_ellipse(
			const Point<float>& pt,
			float radius_x,
			float radius_y,
			const Color_F& color_f = 0
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


		// �ȉ~��`��(�h��)(��`�w��)
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


		// ��̃r�b�g�}�b�v���쐬
		void Graphics_Directx::create_bitmap(
			Bitmap* p_bitmap,
			const Size<unsigned>& size,
			Color_F color_f = 0
		)
		{
			p_render_target->CreateBitmap(
				D2D1::SizeU(size.width, size.height),
				D2D1::BitmapProperties(),
				reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data))
			);
		}


		// �t�@�C������r�b�g�}�b�v���쐬
		bool Graphics_Directx::load_bitmap_from_filename(
			Bitmap* p_bitmap,
			const std::filesystem::path& path
		)
		{
			IWICBitmapDecoder* p_decoder = nullptr;
			IWICBitmapFrameDecode* p_source = nullptr;
			IWICFormatConverter* p_converter = nullptr;
			bool result = true;
			HRESULT hr = S_OK;

			// �f�R�[�_�𐶐�
			hr = p_imaging_factory->CreateDecoderFromFilename(
				path.c_str(),
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&p_decoder
			);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			// �r�b�g�}�b�v�̃t���[�����擾
			hr = p_decoder->GetFrame(0, &p_source);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			// �t�H�[�}�b�g�R���o�[�^�𐶐�
			hr = p_imaging_factory->CreateFormatConverter(&p_converter);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			// �t�H�[�}�b�g�R���o�[�^������
			hr = p_converter->Initialize(
				p_source,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);

			hr = p_render_target->CreateBitmapFromWicBitmap(
				p_converter,
				NULL,
				reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data))
			);

			if (FAILED(hr)) {
				result = false;
				release(reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data)));
			}

		end:
			release(&p_converter);
			release(&p_source);
			release(&p_decoder);

			return result;
		}


		// ���\�[�X����r�b�g�}�b�v���쐬
		bool Graphics_Directx::load_bitmap_from_resource(
			Bitmap* p_bitmap,
			const std::wstring& resource_name,
			const std::wstring& resource_type
		)
		{
			IWICBitmapDecoder* p_decoder = nullptr;
			IWICBitmapFrameDecode* p_source = nullptr;
			IWICStream* p_stream = nullptr;
			IWICFormatConverter* p_converter = nullptr;
			IWICBitmapScaler* p_scaler = nullptr;

			HRSRC img_res_handle = NULL;
			HGLOBAL img_res_data_handle = NULL;
			void* p_img_file = nullptr;
			DWORD img_file_size = 0;
			HRESULT hr = S_OK;
			const HMODULE module_handle = ::GetModuleHandle(NULL);
			bool result = true;

			img_res_handle = ::FindResource(
				module_handle,
				resource_name.c_str(),
				resource_type.c_str()
			);

			if (!img_res_handle) return false;

			img_res_data_handle = ::LoadResource(
				module_handle,
				img_res_handle
			);

			if (!img_res_data_handle) return false;

			p_img_file = ::LockResource(img_res_data_handle);

			if (!p_img_file) return false;

			img_file_size = ::SizeofResource(module_handle, img_res_handle);

			if (!img_file_size) return false;

			hr = p_imaging_factory->CreateStream(&p_stream);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			hr = p_stream->InitializeFromMemory(
				reinterpret_cast<BYTE*>(p_img_file),
				img_file_size
			);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			hr = p_imaging_factory->CreateDecoderFromStream(
				p_stream,
				NULL,
				WICDecodeMetadataCacheOnLoad,
				&p_decoder
			);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			hr = p_decoder->GetFrame(0, &p_source);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			hr = p_imaging_factory->CreateFormatConverter(&p_converter);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			hr = p_converter->Initialize(
				p_source,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);

			if (FAILED(hr)) {
				result = false;
				goto end;
			}

			hr = p_render_target->CreateBitmapFromWicBitmap(
				p_converter,
				NULL,
				reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data))
			);

			if (FAILED(hr)) {
				result = false;
				release(reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data)));
			}

		end:
			release(&p_converter);
			release(&p_source);
			release(&p_decoder);
			release(&p_stream);

			return result;
		}



		// �r�b�g�}�b�v��`��(�A���J�[�|�C���g�w��)
		void Graphics_Directx::draw_bitmap(
			const Bitmap& bitmap,
			const Point<float>& point,
			Anchor_Position anchor_pos,
			float opacity = 1.f
		)
		{
			auto p_d2d1_bitmap = reinterpret_cast<ID2D1Bitmap*>(bitmap.data);
			D2D1_RECT_F rect_f;
			auto size_u = p_d2d1_bitmap->GetPixelSize();

			// �A���J�[�|�C���g�̈ʒu
			switch (anchor_pos) {
			case Anchor_Position::Left:
				rect_f = D2D1::RectF(
					point.x,
					point.y - size_u.height * 0.5f,
					point.x + size_u.width,
					point.y + size_u.height * 0.5f
				);
				break;

			case Anchor_Position::Top:
				rect_f = D2D1::RectF(
					point.x - size_u.width * 0.5f,
					point.y,
					point.x + size_u.width * 0.5f,
					point.y + size_u.height
				);
				break;

			case Anchor_Position::Right:
				rect_f = D2D1::RectF(
					point.x - size_u.width,
					point.y - size_u.height * 0.5f,
					point.x,
					point.y + size_u.height * 0.5f
				);
				break;

			case Anchor_Position::Bottom:
				rect_f = D2D1::RectF(
					point.x - size_u.width * 0.5f,
					point.y - size_u.height,
					point.x + size_u.width * 0.5f,
					point.y
				);
				break;

			case Anchor_Position::Left_Top:
				rect_f = D2D1::RectF(
					point.x,
					point.y,
					point.x + size_u.width,
					point.y + size_u.height
				);
				break;

			case Anchor_Position::Right_Top:
				rect_f = D2D1::RectF(
					point.x - size_u.width,
					point.y,
					point.x,
					point.y + size_u.height
				);
				break;

			case Anchor_Position::Left_Bottom:
				rect_f = D2D1::RectF(
					point.x,
					point.y - size_u.height,
					point.x + size_u.width,
					point.y
				);
				break;

			case Anchor_Position::Right_Bottom:
				rect_f = D2D1::RectF(
					point.x - size_u.width,
					point.y - size_u.height,
					point.x,
					point.y
				);
				break;

			case Anchor_Position::Center:
			default:
				rect_f = D2D1::RectF(
					point.x - size_u.width * 0.5f,
					point.y - size_u.height * 0.5f,
					point.x + size_u.width * 0.5f,
					point.y + size_u.height * 0.5f
				);
				break;
			}

			p_render_target->DrawBitmap(
				p_d2d1_bitmap,
				rect_f,
				opacity
			);
		}


		// �r�b�g�}�b�v��`��(��`�w��)
		void Graphics_Directx::draw_bitmap(
			const Bitmap& bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			p_render_target->DrawBitmap(
				reinterpret_cast<ID2D1Bitmap*>(bitmap.data),
				D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				),
				opacity
			);
		}
	}
}