#include "graphics_directx.hpp"



namespace mkaul {
	namespace graphics {
		// 描画環境の用意
		bool DirectxGraphics::startup()
		{
			HRESULT hr;

			// ID2D1Factoryの作成
			hr = ::D2D1CreateFactory(
				D2D1_FACTORY_TYPE_MULTI_THREADED,
				&p_factory_
			);

			if (SUCCEEDED(hr)) {
				// IWICImagingFactoryの作成
				hr = ::CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					reinterpret_cast<void**>(&p_imaging_factory_)
				);
			}
			if (SUCCEEDED(hr)) {
				// IDWriteFactoryの作成
				hr = ::DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory),
					reinterpret_cast<IUnknown**>(&p_write_factory_)
				);

				return true;
			}

			dx_release(&p_factory_);
			dx_release(&p_imaging_factory_);
			dx_release(&p_write_factory_);

			return false;
		}


		// 描画環境の破棄
		void DirectxGraphics::shutdown()
		{
			dx_release(&p_imaging_factory_);
			dx_release(&p_write_factory_);
			dx_release(&p_factory_);
		}


		// Stroke -> ID2D1StrokeStyle*
		void DirectxGraphics::stroke_to_d2d_strokestyle(
			const Stroke& stroke,
			ID2D1StrokeStyle** pp_stroke_style
		)
		{
			if (p_factory_) p_factory_->CreateStrokeStyle(
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


		// 初期化
		bool DirectxGraphics::init(HWND hwnd)
		{
			RECT rect;

			if (::GetClientRect(hwnd, &rect)) {
				HRESULT hr = E_FAIL;
				D2D1_SIZE_U pixel_size;

				hwnd_ = hwnd;

				pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
				};

				if (p_factory_ && p_write_factory_ && p_imaging_factory_) {
					// HwndRenderTargetを作成
					hr = p_factory_->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(hwnd, pixel_size),
						&p_render_target_
					);

					if (SUCCEEDED(hr) && p_render_target_) {
						// ブラシを作成
						if (!p_brush_) p_render_target_->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &p_brush_);

						return true;
					}
				}
			}
			return false;
		}


		// 終了
		void DirectxGraphics::exit()
		{
			dx_release(&p_brush_);
			dx_release(&p_render_target_);
		}


		// 描画開始
		bool DirectxGraphics::begin_draw()
		{
			if (!drawing_) {
				auto hdc = ::BeginPaint(hwnd_, &paint_struct_);

				if (hdc) {
					p_render_target_->BeginDraw();
					drawing_ = true;

					return true;
				}
			}
			return false;
		}


		// 描画終了
		bool DirectxGraphics::end_draw()
		{
			if (drawing_) {
				auto hr = p_render_target_->EndDraw();
				::EndPaint(hwnd_, &paint_struct_);
				paint_struct_ = { 0 };
				drawing_ = false;

				return SUCCEEDED(hr);
			}
			return false;
		}


		// リサイズ
		bool DirectxGraphics::resize()
		{
			RECT rect;

			if (::GetClientRect(hwnd_, &rect)) {
				D2D1_SIZE_U pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
				};

				if (p_render_target_) {
					auto hr = p_render_target_->Resize(pixel_size);
					return SUCCEEDED(hr);
				}
			}
			return false;
		}


		// 背景を塗りつぶし
		void DirectxGraphics::fill_background(
			const ColorF& color
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				RECT rect;

				if (::GetClientRect(hwnd_, &rect)) {
					p_brush_->SetColor(color.d2d1_colorf());

					p_render_target_->FillRectangle(
						D2D1::RectF(
							(float)rect.left,
							(float)rect.top,
							(float)rect.right,
							(float)rect.bottom
						),
						p_brush_
					);
				}
			}
		}


		// 線を描画
		void DirectxGraphics::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				if (p_render_target_ && p_brush_) {
					p_brush_->SetColor(color.d2d1_colorf());

					p_render_target_->DrawLine(
						pt_from.to<D2D1_POINT_2F>(),
						pt_to.to<D2D1_POINT_2F>(),
						p_brush_,
						stroke.width,
						stroke_style
					);
				}
				dx_release(&stroke_style);
			}
		}


		// 線を描画(複数)
		void DirectxGraphics::draw_lines(
			const Point<float>* pts,
			size_t n_pts,
			const ColorF& color,
			const Stroke& stroke 
		)
		{
			if (drawing_) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_lines = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hr = E_FAIL;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				size_t n_lines = n_pts - 1;
				auto d2d1_pts = new D2D1_POINT_2F[n_lines];

				for (size_t i = 0; i < n_lines; i++)
					d2d1_pts[i] = pts[i + 1].to<D2D1_POINT_2F>();

				hr = p_factory_->CreatePathGeometry(&path_lines);

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

				if (SUCCEEDED(hr) && p_brush_) {
					p_brush_->SetColor(color.d2d1_colorf());

					if (p_render_target_) p_render_target_->DrawGeometry(
						path_lines,
						p_brush_,
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


		// ベジェ曲線を描画
		void DirectxGraphics::draw_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2,
			const Point<float>& pt_3,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_bezier = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hr = E_FAIL;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				hr = p_factory_->CreatePathGeometry(&path_bezier);

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
					p_brush_->SetColor(color.d2d1_colorf());

					if (p_render_target_) p_render_target_->DrawGeometry(
						path_bezier,
						p_brush_,
						stroke.width,
						stroke_style
					);
				}

				dx_release(&sink);
				dx_release(&path_bezier);
				dx_release(&stroke_style);
			}
		}


		// ベジェ曲線を描画(複数)
		void DirectxGraphics::draw_beziers(
			const Point<float>* pts,
			size_t n_pts,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_) {
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

				hr = p_factory_->CreatePathGeometry(&path_bezier);

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

				if (SUCCEEDED(hr) && p_render_target_ && p_brush_) {
					p_brush_->SetColor(color.d2d1_colorf());

					p_render_target_->DrawGeometry(
						path_bezier,
						p_brush_,
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


		// 矩形を描画(線)
		void DirectxGraphics::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);
				p_brush_->SetColor(color.d2d1_colorf());

				const D2D1_RECT_F rect_f = D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				);

				// 角丸矩形を描画
				if (0 < round_radius_x || 0 < round_radius_y) {
					D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(
						rect_f,
						round_radius_x,
						round_radius_y
					);

					p_render_target_->DrawRoundedRectangle(
						rounded_rect,
						p_brush_,
						stroke.width,
						stroke_style
					);
				}
				// 矩形を描画
				else {
					p_render_target_->DrawRectangle(
						rect_f,
						p_brush_,
						stroke.width,
						stroke_style
					);
				}

				dx_release(&stroke_style);
			}
		}


		// 矩形を描画(塗り)
		void DirectxGraphics::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const ColorF& color
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				const D2D1_RECT_F rect_f = D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				);

				// 角丸矩形を描画
				if (0 < round_radius_x || 0 < round_radius_y) {
					D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(
						rect_f,
						round_radius_x,
						round_radius_y
					);

					p_render_target_->FillRoundedRectangle(
						rounded_rect,
						p_brush_
					);
				}
				// 矩形を描画
				else {
					p_render_target_->FillRectangle(
						rect_f,
						p_brush_
					);
				}
			}
		}


		// 楕円を描画(線)(中心点指定)
		void DirectxGraphics::draw_ellipse(
			const Point<float>& pt,
			float radius_x,
			float radius_y,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->DrawEllipse(
					D2D1::Ellipse(
						pt.to<D2D1_POINT_2F>(),
						radius_x,
						radius_y
					),
					p_brush_,
					stroke.width,
					stroke_style
				);

				dx_release(&stroke_style);
			}
		}


		// 楕円を描画(線)(矩形指定)
		void DirectxGraphics::draw_ellipse(
			const Rectangle<float>& rect,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->DrawEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(
							(rect.left + rect.right) * 0.5f,
							(rect.top + rect.bottom) * 0.5f
						),
						(rect.right - rect.left) * 0.5f,
						(rect.bottom - rect.top) * 0.5f
					),
					p_brush_,
					stroke.width,
					stroke_style
				);

				dx_release(&stroke_style);
			}
		}


		// 楕円を描画(塗り)(中心点指定)
		void DirectxGraphics::fill_ellipse(
			const Point<float>& pt,
			float radius_x,
			float radius_y,
			const ColorF& color
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->FillEllipse(
					D2D1::Ellipse(
						pt.to<D2D1_POINT_2F>(),
						radius_x,
						radius_y
					),
					p_brush_
				);
			}
		}


		// 楕円を描画(塗り)(矩形指定)
		void DirectxGraphics::fill_ellipse(
			const Rectangle<float>& rect,
			const ColorF& color
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->FillEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(
							(rect.left + rect.right) * 0.5f,
							(rect.top + rect.bottom) * 0.5f
						),
						(rect.right - rect.left) * 0.5f,
						(rect.bottom - rect.top) * 0.5f
					),
					p_brush_
				);
			}
		}


		// パスを描画(線)
		void DirectxGraphics::draw_path(
			const Path* p_path,
			const ColorF& color,
			const Stroke& stroke
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				stroke_to_d2d_strokestyle(stroke, &stroke_style);

				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->DrawGeometry(
					p_path->get_data<ID2D1PathGeometry*>(0),
					p_brush_,
					stroke.width,
					stroke_style
				);
			}
		}


		// パスを描画(塗り)
		void DirectxGraphics::fill_path(
			const Path* p_path,
			const ColorF& color
		)
		{
			if (drawing_ && p_render_target_ && p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->FillGeometry(
					p_path->get_data<ID2D1PathGeometry*>(0),
					p_brush_
				);
			}
		}


		// 空のビットマップを作成
		bool DirectxGraphics::initialize_bitmap(
			Bitmap* p_bitmap,
			const Size<unsigned>& size,
			const ColorF& color
		)
		{
			if (p_render_target_) {
				ID2D1Bitmap* p_d2d1_bitmap = nullptr;

				HRESULT hr = p_render_target_->CreateBitmap(
					D2D1::SizeU(size.width, size.height),
					D2D1::BitmapProperties(),
					&p_d2d1_bitmap
				);

				if (SUCCEEDED(hr)) {
					p_bitmap->set_data(p_d2d1_bitmap);
					return true;
				}

				dx_release(&p_d2d1_bitmap);
			}
			return false;
		}


		// ファイルからビットマップを作成
		bool DirectxGraphics::load_bitmap_from_filename(
			Bitmap* p_bitmap,
			const std::filesystem::path& path
		)
		{
			IWICBitmapDecoder* p_decoder = nullptr;
			IWICBitmapFrameDecode* p_source = nullptr;
			IWICFormatConverter* p_converter = nullptr;
			HRESULT hr = E_FAIL;
			ID2D1Bitmap* p_d2d1_bitmap = nullptr;

			// デコーダを生成
			hr = p_imaging_factory_->CreateDecoderFromFilename(
				path.c_str(),
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&p_decoder
			);

			if (SUCCEEDED(hr)) {
				// ビットマップのフレームを取得
				hr = p_decoder->GetFrame(0, &p_source);
			}
			if (SUCCEEDED(hr)) {
				// フォーマットコンバータを生成
				hr = p_imaging_factory_->CreateFormatConverter(&p_converter);
			}
			if (SUCCEEDED(hr)) {
				// フォーマットコンバータ初期化
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
				// ビットマップ作成
				hr = p_render_target_->CreateBitmapFromWicBitmap(
					p_converter,
					NULL,
					&p_d2d1_bitmap
				);
			}

			if (SUCCEEDED(hr))
				p_bitmap->set_data(p_d2d1_bitmap);
			else
				dx_release(&p_d2d1_bitmap);

			dx_release(&p_converter);
			dx_release(&p_source);
			dx_release(&p_decoder);

			return SUCCEEDED(hr);
		}


		// リソースからビットマップを作成
		bool DirectxGraphics::load_bitmap_from_resource(
			Bitmap* p_bitmap,
			HINSTANCE hinst,
			const char* res_name,
			const char* res_type
		)
		{
			// ビットマップデコーダ
			IWICBitmapDecoder* p_decoder = nullptr;
			// ビットマップのフレーム
			IWICBitmapFrameDecode* p_source = nullptr;
			// ストリーム
			IWICStream* p_stream = nullptr;
			// コンバータ
			IWICFormatConverter* p_converter = nullptr;

			//IWICBitmapScaler* p_scaler = nullptr;

			// リソースのハンドル
			HRSRC img_res_handle = nullptr;
			// リソースデータのハンドル
			HGLOBAL img_res_data_handle = nullptr;
			// リソースデータ
			void* p_res_data = nullptr;
			// リソースデータのサイズ
			DWORD res_data_size = 0;
			HRESULT hr = S_OK;

			// ビットマップ(Direct2D)のポインタ
			ID2D1Bitmap* p_d2d1_bitmap = nullptr;

			// リソースを探す
			img_res_handle = ::FindResourceA(
				hinst,
				res_name,
				res_type
			);

			// リソースが存在する場合
			if (img_res_handle) {
				// リソースのハンドルを取得
				img_res_data_handle = ::LoadResource(
					hinst,
					img_res_handle
				);
			}
			// リソースのハンドルが存在する場合
			if (img_res_data_handle)
				// リソースをロック(リソースのポインタを取得)
					// 実際にはメモリをロックしないらしい
				p_res_data = ::LockResource(img_res_data_handle);

			// リソースのポインタが存在する場合
			if (p_res_data)
				// リソースのサイズを取得
				res_data_size = ::SizeofResource(hinst, img_res_handle);

			// リソースのサイズが存在する場合
			if (res_data_size)
				// ストリームを作成
				hr = p_imaging_factory_->CreateStream(&p_stream);

			// ストリームの作成に成功した場合
			if (SUCCEEDED(hr)) {
				// ストリームをメモリのデータで初期化
				hr = p_stream->InitializeFromMemory(
					reinterpret_cast<BYTE*>(p_res_data),
					res_data_size
				);
			}
			// ストリームの初期化に成功した場合
			if (SUCCEEDED(hr)) {
				// ストリームからデコーダを作成
				hr = p_imaging_factory_->CreateDecoderFromStream(
					p_stream,
					NULL,
					WICDecodeMetadataCacheOnLoad,
					&p_decoder
				);
			}
			// デコーダの作成に成功した場合
			if (SUCCEEDED(hr))
				// ビットマップのフレームを取得
				hr = p_decoder->GetFrame(0, &p_source);
			
			// フレームの取得に成功した場合
			if (SUCCEEDED(hr))
				// フォーマットコンバータを作成
				hr = p_imaging_factory_->CreateFormatConverter(&p_converter);
			
			// フォーマットコンバータの作成に成功した場合
			if (SUCCEEDED(hr)) {
				// フォーマットコンバータの初期化
				hr = p_converter->Initialize(
					p_source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}

			// フォーマットコンバータの初期化に成功した場合
			if (SUCCEEDED(hr)) {
				if (p_render_target_) {
					// ビットマップを作成
					hr = p_render_target_->CreateBitmapFromWicBitmap(
						p_converter,
						NULL,
						&p_d2d1_bitmap
					);
				}
				else hr = E_FAIL;
			}

			// ビットマップの作成に成功した場合
			if (SUCCEEDED(hr))
				p_bitmap->set_data(p_d2d1_bitmap);
			// ビットマップの作成に失敗した場合
			else
				// ビットマップを開放
				dx_release(&p_d2d1_bitmap);

			// コンバータ・フレーム・デコーダ・ストリームを開放
			dx_release(&p_converter);
			dx_release(&p_source);
			dx_release(&p_decoder);
			dx_release(&p_stream);

			return SUCCEEDED(hr);
		}



		// ビットマップを描画(アンカーポイント指定)
		void DirectxGraphics::draw_bitmap(
			const Bitmap* p_bitmap,
			const Point<float>& point,
			AnchorPosition anchor_pos,
			float opacity
		)
		{
			if (drawing_) {
				auto p_d2d1_bitmap = p_bitmap->get_data<ID2D1Bitmap*>();
				D2D1_RECT_F rect_f;

				if (p_d2d1_bitmap) {
					auto size_u = p_d2d1_bitmap->GetPixelSize();

					// アンカーポイントの位置
					switch (anchor_pos) {
					case AnchorPosition::Center:
					default:
						rect_f = D2D1::RectF(
							point.x - size_u.width * 0.5f,
							point.y - size_u.height * 0.5f,
							point.x + size_u.width * 0.5f,
							point.y + size_u.height * 0.5f
						);
						break;

					case AnchorPosition::Left:
						rect_f = D2D1::RectF(
							point.x,
							point.y - size_u.height * 0.5f,
							point.x + size_u.width,
							point.y + size_u.height * 0.5f
						);
						break;

					case AnchorPosition::Top:
						rect_f = D2D1::RectF(
							point.x - size_u.width * 0.5f,
							point.y,
							point.x + size_u.width * 0.5f,
							point.y + size_u.height
						);
						break;

					case AnchorPosition::Right:
						rect_f = D2D1::RectF(
							point.x - size_u.width,
							point.y - size_u.height * 0.5f,
							point.x,
							point.y + size_u.height * 0.5f
						);
						break;

					case AnchorPosition::Bottom:
						rect_f = D2D1::RectF(
							point.x - size_u.width * 0.5f,
							point.y - size_u.height,
							point.x + size_u.width * 0.5f,
							point.y
						);
						break;

					case AnchorPosition::LeftTop:
						rect_f = D2D1::RectF(
							point.x,
							point.y,
							point.x + size_u.width,
							point.y + size_u.height
						);
						break;

					case AnchorPosition::RightTop:
						rect_f = D2D1::RectF(
							point.x - size_u.width,
							point.y,
							point.x,
							point.y + size_u.height
						);
						break;

					case AnchorPosition::LeftBottom:
						rect_f = D2D1::RectF(
							point.x,
							point.y - size_u.height,
							point.x + size_u.width,
							point.y
						);
						break;

					case AnchorPosition::RightBottom:
						rect_f = D2D1::RectF(
							point.x - size_u.width,
							point.y - size_u.height,
							point.x,
							point.y
						);
						break;
					}

					if (p_render_target_) p_render_target_->DrawBitmap(
						p_d2d1_bitmap,
						rect_f,
						opacity
					);
				}
			}
		}


		// ビットマップを描画(矩形指定)
		void DirectxGraphics::draw_bitmap(
			const Bitmap* p_bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			auto p_d2d1_bitmap = p_bitmap->get_data<ID2D1Bitmap*>();

			if (drawing_ && p_d2d1_bitmap && p_render_target_) p_render_target_->DrawBitmap(
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


		// テキストを描画(アンカーポイント指定)
		void DirectxGraphics::draw_text(
			const std::string& text,
			const Point<float>& point,
			const Font& font,
			AnchorPosition anchor_pos,
			const ColorF& color
		)
		{
			
		}

		// テキストを描画(矩形指定)
		void DirectxGraphics::draw_text(
			const std::string& text,
			const Rectangle<float>& rect,
			const Font& font,
			AnchorPosition anchor_pos,
			bool fit_size,
			const ColorF& color
		)
		{

		}
	}
}