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
			auto p_bitmap = reinterpret_cast<ID2D1Bitmap*>(data);
			dx_release(&p_bitmap);
		}

		size_t Bitmap_Directx::get_width()
		{
			return (reinterpret_cast<ID2D1Bitmap*>(data)->GetPixelSize()).width;
		}

		size_t Bitmap_Directx::get_height()
		{
			return (reinterpret_cast<ID2D1Bitmap*>(data)->GetPixelSize()).height;
		}


		void Path_Directx::release()
		{
			auto p_path = reinterpret_cast<ID2D1PathGeometry*>(data[0]);
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			dx_release(&p_sink);
			dx_release(&p_path);
		}


		bool Path_Directx::begin(const Point<float>& pt)
		{
			auto pp_path = reinterpret_cast<ID2D1PathGeometry**>(&data[0]);
			auto pp_sink = reinterpret_cast<ID2D1GeometrySink**>(&data[1]);
			data[0] = nullptr;
			data[1] = nullptr;

			auto hr = Graphics_Directx::get_factory()->CreatePathGeometry(pp_path);

			if (SUCCEEDED(hr))
				hr = (*pp_path)->Open(pp_sink);

			if (SUCCEEDED(hr)) {
				(*pp_sink)->BeginFigure(
					pt.to<D2D1_POINT_2F>(),
					D2D1_FIGURE_BEGIN_FILLED
				);
				pt_last = pt;

				return true;
			}

			// ���s�����ꍇ
			dx_release(pp_sink);
			dx_release(pp_path);

			return false;
		}


		void Path_Directx::end(Figure_End fe)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			p_sink->EndFigure((D2D1_FIGURE_END)fe);
			p_sink->Close();
		}


		// �ʂ�ǉ�
		void Path_Directx::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);
			D2D1_SWEEP_DIRECTION sd;
			D2D1_ARC_SIZE as;

			// �ʂ̊J�n�E�I���n�_(�ȉ~�̒��S����̑��΍��W)
			// �ʂ̏I���n�_
			// �ȉ~�̒��S
			Point<float> pt_ofs_start, pt_ofs_end, pt_end, pt_o;

			// �p�x��-360d ~ 360d�͈̔͂Ɏ��߂�
			angle_sweep = std::fmodf(angle_sweep, 360.f);

			ellipse_pos(size, angle_start, &pt_ofs_start);
			ellipse_pos(size, angle_start + angle_sweep, &pt_ofs_end);

			pt_o = pt_last - pt_ofs_start;
			pt_end = pt_o + pt_ofs_end;

			// ���v���
			if (angle_sweep > 0) {
				sd = D2D1_SWEEP_DIRECTION_CLOCKWISE;

				// �ʂ̊p�x�̑傫����180d���傫���ꍇ
				if (angle_sweep > 180.f)
					as = D2D1_ARC_SIZE_LARGE;
				// �ʂ̊p�x�̑傫�����΂�菬�����ꍇ
				else
					as = D2D1_ARC_SIZE_SMALL;
			}
			// �����v���
			else {
				sd = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

				// �ʂ̊p�x�̑傫����180d���傫���ꍇ
				if (angle_sweep < -180.f)
					as = D2D1_ARC_SIZE_LARGE;
				// �ʂ̊p�x�̑傫�����΂�菬�����ꍇ
				else
					as = D2D1_ARC_SIZE_SMALL;
			}


			p_sink->AddArc(
				D2D1::ArcSegment(
					pt_end.to<D2D1_POINT_2F>(),
					size.to<D2D1_SIZE_F>(),
					0.f,
					sd,
					as
				)
			);
		}


		// ����ǉ�
		void Path_Directx::add_line(
			const Point<float>& pt
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			p_sink->AddLine(
				pt.to<D2D1_POINT_2F>()
			);
		}


		// �x�W�F�Ȑ���ǉ�
		void Path_Directx::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			p_sink->AddBezier(
				D2D1::BezierSegment(
					pt_0.to<D2D1_POINT_2F>(),
					pt_1.to<D2D1_POINT_2F>(),
					pt_2.to<D2D1_POINT_2F>()
				)
			);
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

			if (SUCCEEDED(hr)) {
				// IWICImagingFactory�̍쐬
				hr = ::CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					reinterpret_cast<void**>(&p_imaging_factory)
				);
			}
			if (SUCCEEDED(hr)) {
				// IDWriteFactory�̍쐬
				hr = ::DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory),
					reinterpret_cast<IUnknown**>(&p_write_factory)
				);

				return true;
			}

			dx_release(&p_factory);
			dx_release(&p_imaging_factory);
			dx_release(&p_write_factory);

			return false;
		}


		// �`����̔j��
		void Graphics_Directx::shutdown()
		{
			dx_release(&p_imaging_factory);
			dx_release(&p_write_factory);
			dx_release(&p_factory);
		}


		// Stroke -> ID2D1StrokeStyle*
		void Graphics_Directx::stroke_to_d2d_strokestyle(
			const Stroke& stroke,
			ID2D1StrokeStyle** pp_stroke_style
		)
		{
			if (p_factory) p_factory->CreateStrokeStyle(
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


		// ������
		bool Graphics_Directx::init(HWND hwnd_)
		{
			RECT rect;

			if (::GetClientRect(hwnd_, &rect)) {
				HRESULT hr = E_FAIL;
				D2D1_SIZE_U pixel_size;

				hwnd = hwnd_;

				pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
				};

				if (p_factory && p_write_factory && p_imaging_factory) {
					// HwndRenderTarget���쐬
					hr = p_factory->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(hwnd, pixel_size),
						&p_render_target
					);

					if (SUCCEEDED(hr) && p_render_target) {
						// �u���V���쐬
						if (!p_brush) p_render_target->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &p_brush);

						return true;
					}
				}
			}
			return false;
		}


		// �I��
		void Graphics_Directx::exit()
		{
			dx_release(&p_brush);
			dx_release(&p_render_target);
		}


		// �`��J�n
		bool Graphics_Directx::begin_draw()
		{
			if (!drawing) {
				auto hdc = ::BeginPaint(hwnd, &ps);

				if (hdc) {
					p_render_target->BeginDraw();
					drawing = true;

					return true;
				}
			}
			return false;
		}


		// �`��I��
		bool Graphics_Directx::end_draw()
		{
			if (drawing) {
				auto hr = p_render_target->EndDraw();
				::EndPaint(hwnd, &ps);
				ps = { 0 };
				drawing = false;

				return SUCCEEDED(hr);
			}
			return false;
		}


		// ���T�C�Y
		bool Graphics_Directx::resize()
		{
			RECT rect;

			if (::GetClientRect(hwnd, &rect)) {
				D2D1_SIZE_U pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
				};

				if (p_render_target) {
					auto hr = p_render_target->Resize(pixel_size);
					return SUCCEEDED(hr);
				}
			}
			return false;
		}


		// �w�i��h��Ԃ�
		void Graphics_Directx::fill_background(
			const Color_F& col_f
		)
		{
			if (drawing && p_render_target && p_brush) {
				RECT rect;

				if (::GetClientRect(hwnd, &rect)) {
					p_brush->SetColor(col_f.d2d1_colorf());

					p_render_target->FillRectangle(
						D2D1::RectF(
							(float)rect.left,
							(float)rect.top,
							(float)rect.right,
							(float)rect.bottom
						),
						p_brush
					);
				}
			}
		}


		// ����`��
		void Graphics_Directx::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (drawing) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				if (p_render_target && p_brush) {
					p_brush->SetColor(col_f.d2d1_colorf());

					p_render_target->DrawLine(
						pt_from.to<D2D1_POINT_2F>(),
						pt_to.to<D2D1_POINT_2F>(),
						p_brush,
						stroke.width,
						stroke_style
					);
				}
				dx_release(&stroke_style);
			}
		}


		// ����`��(����)
		void Graphics_Directx::draw_lines(
			const Point<float>* pts,
			size_t n_pts,
			const Color_F& col_f,
			const Stroke& stroke 
		)
		{
			if (drawing) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_lines = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hr = E_FAIL;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				size_t n_lines = n_pts - 1;
				auto d2d1_pts = new D2D1_POINT_2F[n_lines];

				for (size_t i = 0; i < n_lines; i++)
					d2d1_pts[i] = pts[i + 1].to<D2D1_POINT_2F>();

				hr = p_factory->CreatePathGeometry(&path_lines);

				if (SUCCEEDED(hr))
					hr = path_lines->Open(&sink);

				if (SUCCEEDED(hr)) {
					sink->BeginFigure(
						pts[0].to<D2D1_POINT_2F>(),
						D2D1_FIGURE_BEGIN_HOLLOW
					);
					sink->AddLines(d2d1_pts, n_lines);
					sink->EndFigure(D2D1_FIGURE_END_OPEN);

					hr = sink->Close();
				}

				if (SUCCEEDED(hr) && p_brush) {
					p_brush->SetColor(col_f.d2d1_colorf());

					if (p_render_target) p_render_target->DrawGeometry(
						path_lines,
						p_brush,
						stroke.width,
						stroke_style
					);
				}

				delete[] d2d1_pts;
				dx_release(&sink);
				dx_release(&path_lines);
				dx_release(&stroke_style);
			}
		}


		// �x�W�F�Ȑ���`��
		void Graphics_Directx::draw_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2,
			const Point<float>& pt_3,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (drawing) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_bezier = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hr = E_FAIL;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				hr = p_factory->CreatePathGeometry(&path_bezier);

				if (SUCCEEDED(hr))
					hr = path_bezier->Open(&sink);

				if (SUCCEEDED(hr)) {
					sink->BeginFigure(
						pt_0.to<D2D1_POINT_2F>(),
						D2D1_FIGURE_BEGIN_HOLLOW
					);

					sink->AddBezier(D2D1::BezierSegment(
						pt_1.to<D2D1_POINT_2F>(),
						pt_2.to<D2D1_POINT_2F>(),
						pt_3.to<D2D1_POINT_2F>()
					));

					sink->EndFigure(D2D1_FIGURE_END_OPEN);
					hr = sink->Close();
				}
				if (SUCCEEDED(hr)) {
					p_brush->SetColor(col_f.d2d1_colorf());

					if (p_render_target) p_render_target->DrawGeometry(
						path_bezier,
						p_brush,
						stroke.width,
						stroke_style
					);
				}

				dx_release(&sink);
				dx_release(&path_bezier);
				dx_release(&stroke_style);
			}
		}


		// �x�W�F�Ȑ���`��(����)
		void Graphics_Directx::draw_beziers(
			const Point<float>* pts,
			size_t n_pts,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (drawing) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_bezier = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hr = E_FAIL;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				size_t bezier_count = (n_pts - 1) / 3;
				auto bezier_segments = new D2D1_BEZIER_SEGMENT[bezier_count];

				for (size_t i = 0; i < bezier_count; i++) {
					bezier_segments[i] = D2D1::BezierSegment(
						pts[i * 3 + 1].to<D2D1_POINT_2F>(),
						pts[i * 3 + 2].to<D2D1_POINT_2F>(),
						pts[i * 3 + 3].to<D2D1_POINT_2F>()
					);
				}

				hr = p_factory->CreatePathGeometry(&path_bezier);

				if (SUCCEEDED(hr))
					path_bezier->Open(&sink);

				if (SUCCEEDED(hr)) {
					sink->BeginFigure(
						pts[0].to<D2D1_POINT_2F>(),
						D2D1_FIGURE_BEGIN_HOLLOW
					);

					sink->AddBeziers(
						bezier_segments,
						bezier_count
					);

					sink->EndFigure(D2D1_FIGURE_END_OPEN);
					hr = sink->Close();
				}

				if (SUCCEEDED(hr) && p_render_target && p_brush) {
					p_brush->SetColor(col_f.d2d1_colorf());

					p_render_target->DrawGeometry(
						path_bezier,
						p_brush,
						stroke.width,
						stroke_style
					);
				}

				delete[] bezier_segments;
				dx_release(&sink);
				dx_release(&path_bezier);
				dx_release(&stroke_style);
			}
		}


		// ��`��`��(��)
		void Graphics_Directx::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_render_target && p_brush) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);
				p_brush->SetColor(col_f.d2d1_colorf());

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

				dx_release(&stroke_style);
			}
		}


		// ��`��`��(�h��)
		void Graphics_Directx::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const Color_F& col_f
		)
		{
			if (drawing && p_render_target && p_brush) {
				p_brush->SetColor(col_f.d2d1_colorf());

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
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_render_target && p_brush) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush->SetColor(col_f.d2d1_colorf());

				p_render_target->DrawEllipse(
					D2D1::Ellipse(
						pt.to<D2D1_POINT_2F>(),
						radius_x,
						radius_y
					),
					p_brush,
					stroke.width,
					stroke_style
				);

				dx_release(&stroke_style);
			}
		}


		// �ȉ~��`��(��)(��`�w��)
		void Graphics_Directx::draw_ellipse(
			const Rectangle<float>& rect,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (drawing && p_render_target && p_brush) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush->SetColor(col_f.d2d1_colorf());

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

				dx_release(&stroke_style);
			}
		}


		// �ȉ~��`��(�h��)(���S�_�w��)
		void Graphics_Directx::fill_ellipse(
			const Point<float>& pt,
			float radius_x,
			float radius_y,
			const Color_F& col_f
		)
		{
			if (drawing && p_render_target && p_brush) {
				p_brush->SetColor(col_f.d2d1_colorf());

				p_render_target->FillEllipse(
					D2D1::Ellipse(
						pt.to<D2D1_POINT_2F>(),
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
			const Color_F& col_f
		)
		{
			if (drawing && p_render_target && p_brush) {
				p_brush->SetColor(col_f.d2d1_colorf());

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
		bool Graphics_Directx::initialize_bitmap(
			Bitmap* p_bitmap,
			const Size<unsigned>& size,
			Color_F col_f
		)
		{
			if (p_render_target) {
				HRESULT hr = E_FAIL;
				hr = p_render_target->CreateBitmap(
					D2D1::SizeU(size.width, size.height),
					D2D1::BitmapProperties(),
					reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data))
				);

				if (SUCCEEDED(hr)) return true;

				dx_release(reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data)));
			}
			return false;
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
			HRESULT hr = E_FAIL;
			auto pp_dxbitmap = reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data));
			p_bitmap->data = nullptr;

			// �f�R�[�_�𐶐�
			hr = p_imaging_factory->CreateDecoderFromFilename(
				path.c_str(),
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&p_decoder
			);

			if (SUCCEEDED(hr)) {
				// �r�b�g�}�b�v�̃t���[�����擾
				hr = p_decoder->GetFrame(0, &p_source);
			}
			if (SUCCEEDED(hr)) {
				// �t�H�[�}�b�g�R���o�[�^�𐶐�
				hr = p_imaging_factory->CreateFormatConverter(&p_converter);
			}
			if (SUCCEEDED(hr)) {
				// �t�H�[�}�b�g�R���o�[�^������
				hr = p_converter->Initialize(
					p_source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}
			if (SUCCEEDED(hr)) {
				// �r�b�g�}�b�v�쐬
				hr = p_render_target->CreateBitmapFromWicBitmap(
					p_converter,
					NULL,
					pp_dxbitmap
				);
			}

			if (FAILED(hr)) dx_release(pp_dxbitmap);

			dx_release(&p_converter);
			dx_release(&p_source);
			dx_release(&p_decoder);

			return SUCCEEDED(hr);
		}


		// ���\�[�X����r�b�g�}�b�v���쐬
		bool Graphics_Directx::load_bitmap_from_resource(
			Bitmap* p_bitmap,
			HINSTANCE hinst,
			const char* res_name,
			const char* res_type
		)
		{
			// �r�b�g�}�b�v�f�R�[�_
			IWICBitmapDecoder* p_decoder = nullptr;
			// �r�b�g�}�b�v�̃t���[��
			IWICBitmapFrameDecode* p_source = nullptr;
			// �X�g���[��
			IWICStream* p_stream = nullptr;
			// �R���o�[�^
			IWICFormatConverter* p_converter = nullptr;

			//IWICBitmapScaler* p_scaler = nullptr;

			// ���\�[�X�̃n���h��
			HRSRC img_res_handle = nullptr;
			// ���\�[�X�f�[�^�̃n���h��
			HGLOBAL img_res_data_handle = nullptr;
			// ���\�[�X�f�[�^
			void* p_res_data = nullptr;
			// ���\�[�X�f�[�^�̃T�C�Y
			DWORD res_data_size = 0;
			HRESULT hr = S_OK;

			// �r�b�g�}�b�v(Direct2D)�̃|�C���^
			auto pp_dxbitmap = reinterpret_cast<ID2D1Bitmap**>(&(p_bitmap->data));
			p_bitmap->data = nullptr;

			// ���\�[�X��T��
			img_res_handle = ::FindResourceA(
				hinst,
				res_name,
				res_type
			);

			// ���\�[�X�����݂���ꍇ
			if (img_res_handle) {
				// ���\�[�X�̃n���h�����擾
				img_res_data_handle = ::LoadResource(
					hinst,
					img_res_handle
				);
			}
			// ���\�[�X�̃n���h�������݂���ꍇ
			if (img_res_data_handle)
				// ���\�[�X�����b�N(���\�[�X�̃|�C���^���擾)
					// ���ۂɂ̓����������b�N���Ȃ��炵��
				p_res_data = ::LockResource(img_res_data_handle);

			// ���\�[�X�̃|�C���^�����݂���ꍇ
			if (p_res_data)
				// ���\�[�X�̃T�C�Y���擾
				res_data_size = ::SizeofResource(hinst, img_res_handle);

			// ���\�[�X�̃T�C�Y�����݂���ꍇ
			if (res_data_size)
				// �X�g���[�����쐬
				hr = p_imaging_factory->CreateStream(&p_stream);

			// �X�g���[���̍쐬�ɐ��������ꍇ
			if (SUCCEEDED(hr)) {
				// �X�g���[�����������̃f�[�^�ŏ�����
				hr = p_stream->InitializeFromMemory(
					reinterpret_cast<BYTE*>(p_res_data),
					res_data_size
				);
			}
			// �X�g���[���̏������ɐ��������ꍇ
			if (SUCCEEDED(hr)) {
				// �X�g���[������f�R�[�_���쐬
				hr = p_imaging_factory->CreateDecoderFromStream(
					p_stream,
					NULL,
					WICDecodeMetadataCacheOnLoad,
					&p_decoder
				);
			}
			// �f�R�[�_�̍쐬�ɐ��������ꍇ
			if (SUCCEEDED(hr))
				// �r�b�g�}�b�v�̃t���[�����擾
				hr = p_decoder->GetFrame(0, &p_source);
			
			// �t���[���̎擾�ɐ��������ꍇ
			if (SUCCEEDED(hr))
				// �t�H�[�}�b�g�R���o�[�^���쐬
				hr = p_imaging_factory->CreateFormatConverter(&p_converter);
			
			// �t�H�[�}�b�g�R���o�[�^�̍쐬�ɐ��������ꍇ
			if (SUCCEEDED(hr)) {
				// �t�H�[�}�b�g�R���o�[�^�̏�����
				hr = p_converter->Initialize(
					p_source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}

			// �t�H�[�}�b�g�R���o�[�^�̏������ɐ��������ꍇ
			if (SUCCEEDED(hr)) {
				if (p_render_target) {
					// �r�b�g�}�b�v���쐬
					hr = p_render_target->CreateBitmapFromWicBitmap(
						p_converter,
						NULL,
						pp_dxbitmap
					);
				}
				else hr = E_FAIL;
			}

			// �r�b�g�}�b�v�̍쐬�Ɏ��s�����ꍇ
			if (FAILED(hr))
				// �r�b�g�}�b�v���J��
				dx_release(pp_dxbitmap);

			// �R���o�[�^�E�t���[���E�f�R�[�_�E�X�g���[�����J��
			dx_release(&p_converter);
			dx_release(&p_source);
			dx_release(&p_decoder);
			dx_release(&p_stream);

			return SUCCEEDED(hr);
		}



		// �r�b�g�}�b�v��`��(�A���J�[�|�C���g�w��)
		void Graphics_Directx::draw_bitmap(
			const Bitmap* bitmap,
			const Point<float>& point,
			Anchor_Position anchor_pos,
			float opacity
		)
		{
			if (drawing) {
				auto p_d2d1_bitmap = reinterpret_cast<ID2D1Bitmap*>(bitmap->data);
				D2D1_RECT_F rect_f;

				if (p_d2d1_bitmap) {
					auto size_u = p_d2d1_bitmap->GetPixelSize();

					// �A���J�[�|�C���g�̈ʒu
					switch (anchor_pos) {
					case Anchor_Position::Center:
					default:
						rect_f = D2D1::RectF(
							point.x - size_u.width * 0.5f,
							point.y - size_u.height * 0.5f,
							point.x + size_u.width * 0.5f,
							point.y + size_u.height * 0.5f
						);
						break;

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
					}

					if (p_render_target) p_render_target->DrawBitmap(
						p_d2d1_bitmap,
						rect_f,
						opacity
					);
				}
			}
		}


		// �r�b�g�}�b�v��`��(��`�w��)
		void Graphics_Directx::draw_bitmap(
			const Bitmap* bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			auto p_d2d1_bitmap = reinterpret_cast<ID2D1Bitmap*>(bitmap->data);

			if (drawing && p_d2d1_bitmap && p_render_target) p_render_target->DrawBitmap(
				p_d2d1_bitmap,
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