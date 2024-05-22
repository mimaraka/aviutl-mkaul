#include "graphics_directx.hpp"
#include "bitmap_directx.hpp"
#include <strconv2.h>



namespace mkaul {
	namespace graphics {
		// 描画環境の用意
		bool DirectxGraphics::startup() noexcept {
			HRESULT hresult;

			// ID2D1Factoryの作成
			hresult = ::D2D1CreateFactory(
				D2D1_FACTORY_TYPE_MULTI_THREADED,
				&p_factory_
			);

			if (SUCCEEDED(hresult)) {
				// IWICImagingFactoryの作成
				hresult = ::CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					reinterpret_cast<void**>(&p_imaging_factory_)
				);
			}
			if (SUCCEEDED(hresult)) {
				// IDWriteFactoryの作成
				hresult = ::DWriteCreateFactory(
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
		void DirectxGraphics::shutdown() noexcept {
			dx_release(&p_imaging_factory_);
			dx_release(&p_write_factory_);
			dx_release(&p_factory_);
		}


		// Stroke -> ID2D1StrokeStyle*
		void DirectxGraphics::apply_stroke_style(
			const Stroke& stroke,
			_Out_ ID2D1StrokeStyle** pp_stroke_style
		) noexcept {
			// widthが1より小さいときにdashの間隔が狭くなるバグ？がある
			if (stroke.width < 1.f and 0.f < stroke.width) {
				for (int i = 0; i < stroke.dashes_count; i++) {
					stroke.custom_dashes[i] /= stroke.width;
				}
			}
			
			*pp_stroke_style = nullptr;
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


		bool DirectxGraphics::apply_text_layout(
			const std::string& text,
			const Font& font,
			const D2D1_SIZE_F& target_size,
			const AnchorPosition& anchor_pos,
			_Out_ IDWriteTextLayout** pp_text_layout
		) noexcept {
			HRESULT hresult = S_OK;
			IDWriteTextFormat* p_text_format = nullptr;
			DWRITE_FONT_STYLE font_style = DWRITE_FONT_STYLE_NORMAL;
			DWRITE_TEXT_ALIGNMENT text_alignment;
			DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment;
			*pp_text_layout = nullptr;

			if (static_cast<bool>(font.style & flag::FontStyle::Italic)) {
				font_style = DWRITE_FONT_STYLE_ITALIC;
			}
			hresult = p_write_factory_->CreateTextFormat(
				::sjis_to_wide(font.family_name).c_str(),
				NULL,
				(DWRITE_FONT_WEIGHT)font.weight,
				font_style,
				DWRITE_FONT_STRETCH_NORMAL,
				font.height * 1.66f, // フォントの高さを補正
				L"",
				&p_text_format
			);
			if (SUCCEEDED(hresult)) {
				hresult = p_write_factory_->CreateTextLayout(
					::sjis_to_wide(text).c_str(),
					(uint32_t)text.size(),
					p_text_format,
					target_size.width,
					target_size.height,
					pp_text_layout
				);
				dx_release(&p_text_format);
			}
			if (SUCCEEDED(hresult)) {
				// 下線
				if (static_cast<bool>(font.style & flag::FontStyle::UnderLine)) {
					(*pp_text_layout)->SetUnderline(TRUE, { 0, (uint32_t)text.size() });
				}

				// アンカー位置 (水平方向)
				switch (anchor_pos.horizontal) {
				case AnchorPosition::Horizontal::Left:
					text_alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
					break;

				case AnchorPosition::Horizontal::Right:
					text_alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
					break;

				default:
					text_alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				}

				// アンカー位置 (垂直方向)
				switch (anchor_pos.vertical) {
				case AnchorPosition::Vertical::Top:
					paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
					break;

				case AnchorPosition::Vertical::Bottom:
					paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
					break;

				default:
					paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				}

				(*pp_text_layout)->SetTextAlignment(text_alignment);
				(*pp_text_layout)->SetParagraphAlignment(paragraph_alignment);
				// TODO: Wrapをするオプションを付ける
				(*pp_text_layout)->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
				return true;
			}
			return false;
		}


		// 初期化
		bool DirectxGraphics::init(HWND hwnd) noexcept {
			RECT rect;

			if (::GetClientRect(hwnd, &rect)) {
				HRESULT hresult = E_FAIL;
				D2D1_SIZE_U pixel_size;

				hwnd_ = hwnd;

				pixel_size = {
				(unsigned)(rect.right - rect.left),
				(unsigned)(rect.bottom - rect.top)
				};

				if (p_factory_ and p_write_factory_ and p_imaging_factory_) {
					// HwndRenderTargetを作成
					hresult = p_factory_->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(
							D2D1_RENDER_TARGET_TYPE_DEFAULT,
							D2D1::PixelFormat(),
							96.f, 96.f
						),
						D2D1::HwndRenderTargetProperties(hwnd, pixel_size),
						&p_render_target_
					);

					if (SUCCEEDED(hresult) and p_render_target_) {
						// ブラシを作成
						if (!p_brush_) p_render_target_->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &p_brush_);

						return true;
					}
				}
			}
			return false;
		}


		void DirectxGraphics::release() noexcept {
			dx_release(&p_brush_);
			dx_release(&p_render_target_);
		}


		// 描画開始
		bool DirectxGraphics::begin_draw() noexcept {
			if (!drawing_) {
				auto hdc = ::BeginPaint(hwnd_, &paint_struct_);

				if (hdc) {
					p_render_target_->BeginDraw();
					p_render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
					drawing_ = true;

					return true;
				}
			}
			return false;
		}


		// 描画終了
		bool DirectxGraphics::end_draw() noexcept {
			if (drawing_) {
				auto hresult = p_render_target_->EndDraw();
					::EndPaint(hwnd_, &paint_struct_);
				paint_struct_ = { 0 };
				drawing_ = false;

				return SUCCEEDED(hresult);
			}
			return false;
		}


		// リサイズ
		bool DirectxGraphics::resize() noexcept {
			RECT rect;

			if (::GetClientRect(hwnd_, &rect)) {
				D2D1_SIZE_U pixel_size = {
					(uint32_t)(rect.right - rect.left),
					(uint32_t)(rect.bottom - rect.top)
				};

				if (p_render_target_) {
					auto hresult = p_render_target_->Resize(pixel_size);
					return SUCCEEDED(hresult);
				}
			}
			return false;
		}


		// 背景を塗りつぶし
		void DirectxGraphics::fill_background(
			const ColorF& color
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
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
		) noexcept {
			if (drawing_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				apply_stroke_style(stroke, &stroke_style);

				if (p_render_target_ and p_brush_) {
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
		) noexcept {
			if (drawing_) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_lines = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hresult = E_FAIL;
				apply_stroke_style(stroke, &stroke_style);

				size_t n_lines = n_pts - 1;
				auto d2d1_pts = new D2D1_POINT_2F[n_lines];

				for (size_t i = 0; i < n_lines; i++)
					d2d1_pts[i] = pts[i + 1].to<D2D1_POINT_2F>();

				hresult = p_factory_->CreatePathGeometry(&path_lines);

				if (SUCCEEDED(hresult))
					hresult = path_lines->Open(&sink);

				if (SUCCEEDED(hresult)) {
					sink->BeginFigure(
						pts[0].to<D2D1_POINT_2F>(),
						D2D1_FIGURE_BEGIN_HOLLOW
					);
					sink->AddLines(d2d1_pts, n_lines);
					sink->EndFigure(D2D1_FIGURE_END_OPEN);

					hresult = sink->Close();
				}

				if (SUCCEEDED(hresult) and p_brush_) {
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
		) noexcept {
			if (drawing_) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_bezier = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hresult = E_FAIL;
				apply_stroke_style(stroke, &stroke_style);

				hresult = p_factory_->CreatePathGeometry(&path_bezier);

				if (SUCCEEDED(hresult))
					hresult = path_bezier->Open(&sink);

				if (SUCCEEDED(hresult)) {
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
					hresult = sink->Close();
				}
				if (SUCCEEDED(hresult)) {
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
		) noexcept {
			if (drawing_) {
				ID2D1GeometrySink* sink = nullptr;
				ID2D1PathGeometry* path_bezier = nullptr;
				ID2D1StrokeStyle* stroke_style = nullptr;
				HRESULT hresult = E_FAIL;
				apply_stroke_style(stroke, &stroke_style);

				size_t bezier_count = (n_pts - 1) / 3;
				auto bezier_segments = new D2D1_BEZIER_SEGMENT[bezier_count];

				for (size_t i = 0; i < bezier_count; i++) {
					bezier_segments[i] = D2D1::BezierSegment(
						pts[i * 3 + 1].to<D2D1_POINT_2F>(),
						pts[i * 3 + 2].to<D2D1_POINT_2F>(),
						pts[i * 3 + 3].to<D2D1_POINT_2F>()
					);
				}

				hresult = p_factory_->CreatePathGeometry(&path_bezier);

				if (SUCCEEDED(hresult))
					path_bezier->Open(&sink);

				if (SUCCEEDED(hresult)) {
					sink->BeginFigure(
						pts[0].to<D2D1_POINT_2F>(),
						D2D1_FIGURE_BEGIN_HOLLOW
					);

					sink->AddBeziers(
						bezier_segments,
						bezier_count
					);

					sink->EndFigure(D2D1_FIGURE_END_OPEN);
					hresult = sink->Close();
				}

				if (SUCCEEDED(hresult) and p_render_target_ and p_brush_) {
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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				apply_stroke_style(stroke, &stroke_style);
				p_brush_->SetColor(color.d2d1_colorf());

				const D2D1_RECT_F rect_f = D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				);

				// 角丸矩形を描画
				if (0 < round_radius_x or 0 < round_radius_y) {
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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				const D2D1_RECT_F rect_f = D2D1::RectF(
					rect.left,
					rect.top,
					rect.right,
					rect.bottom
				);

				// 角丸矩形を描画
				if (0 < round_radius_x or 0 < round_radius_y) {
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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				apply_stroke_style(stroke, &stroke_style);

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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				apply_stroke_style(stroke, &stroke_style);

				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->DrawEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(
							(rect.left + rect.right) * 0.5f,
							(rect.top + rect.bottom) * 0.5f
						),
						rect.get_width() * 0.5f,
						rect.get_height() * 0.5f
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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->FillEllipse(
					D2D1::Ellipse(
						D2D1::Point2F(
							(rect.left + rect.right) * 0.5f,
							(rect.top + rect.bottom) * 0.5f
						),
						rect.get_width() * 0.5f,
						rect.get_height() * 0.5f
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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				ID2D1StrokeStyle* stroke_style = nullptr;
				apply_stroke_style(stroke, &stroke_style);

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
		) noexcept {
			if (drawing_ and p_render_target_ and p_brush_) {
				p_brush_->SetColor(color.d2d1_colorf());

				p_render_target_->FillGeometry(
					p_path->get_data<ID2D1PathGeometry*>(0),
					p_brush_
				);
			}
		}


		// 空のビットマップを作成
		std::unique_ptr<Bitmap> DirectxGraphics::create_bitmap(
			const Size<unsigned>& size,
			const ColorF& color
		) noexcept {
			if (p_render_target_) {
				ID2D1Bitmap* p_d2d1_bitmap = nullptr;

				HRESULT hresult = p_render_target_->CreateBitmap(
					D2D1::SizeU(size.width, size.height),
					D2D1::BitmapProperties(),
					&p_d2d1_bitmap
				);

				if (SUCCEEDED(hresult)) {
					auto p_bitmap = std::make_unique<DirectxBitmap>();
					p_bitmap->set_data(p_d2d1_bitmap);
					return p_bitmap;
				}

				dx_release(&p_d2d1_bitmap);
			}
			return nullptr;
		}


		// ファイルからビットマップを作成
		std::unique_ptr<Bitmap> DirectxGraphics::load_bitmap_from_filename(
			const std::filesystem::path& path
		) noexcept {
			IWICBitmapDecoder* p_decoder = nullptr;
			IWICBitmapFrameDecode* p_source = nullptr;
			IWICFormatConverter* p_converter = nullptr;
			HRESULT hresult = E_FAIL;
			ID2D1Bitmap* p_d2d1_bitmap = nullptr;
			std::unique_ptr<DirectxBitmap> p_bitmap = nullptr;

			// デコーダを生成
			hresult = p_imaging_factory_->CreateDecoderFromFilename(
				path.c_str(),
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&p_decoder
			);

			if (SUCCEEDED(hresult)) {
				// ビットマップのフレームを取得
				hresult = p_decoder->GetFrame(0, &p_source);
			}
			if (SUCCEEDED(hresult)) {
				// フォーマットコンバータを生成
				hresult = p_imaging_factory_->CreateFormatConverter(&p_converter);
			}
			if (SUCCEEDED(hresult)) {
				// フォーマットコンバータ初期化
				hresult = p_converter->Initialize(
					p_source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}
			if (SUCCEEDED(hresult)) {
				// ビットマップ作成
				hresult = p_render_target_->CreateBitmapFromWicBitmap(
					p_converter,
					NULL,
					&p_d2d1_bitmap
				);
			}
			if (SUCCEEDED(hresult)) {
				p_bitmap = std::make_unique<DirectxBitmap>();
				p_bitmap->set_data(p_d2d1_bitmap);
			}
			else {
				dx_release(&p_d2d1_bitmap);
			}

			dx_release(&p_converter);
			dx_release(&p_source);
			dx_release(&p_decoder);

			return std::move(p_bitmap);
		}


		// リソースからビットマップを作成
		std::unique_ptr<Bitmap> DirectxGraphics::load_bitmap_from_resource(
			HINSTANCE hinst,
			const char* res_name,
			const char* res_type
		) noexcept {
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
			HRESULT hresult = S_OK;

			// ビットマップ(Direct2D)のポインタ
			ID2D1Bitmap* p_d2d1_bitmap = nullptr;
			std::unique_ptr<Bitmap> p_bitmap = nullptr;

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
				hresult = p_imaging_factory_->CreateStream(&p_stream);

			// ストリームの作成に成功した場合
			if (SUCCEEDED(hresult)) {
				// ストリームをメモリのデータで初期化
				hresult = p_stream->InitializeFromMemory(
					reinterpret_cast<BYTE*>(p_res_data),
					res_data_size
				);
			}
			// ストリームの初期化に成功した場合
			if (SUCCEEDED(hresult)) {
				// ストリームからデコーダを作成
				hresult = p_imaging_factory_->CreateDecoderFromStream(
					p_stream,
					NULL,
					WICDecodeMetadataCacheOnLoad,
					&p_decoder
				);
			}
			// デコーダの作成に成功した場合
			if (SUCCEEDED(hresult)) {
				// ビットマップのフレームを取得
				hresult = p_decoder->GetFrame(0, &p_source);
			}
			// フレームの取得に成功した場合
			if (SUCCEEDED(hresult)) {
				// フォーマットコンバータを作成
				hresult = p_imaging_factory_->CreateFormatConverter(&p_converter);
			}
			// フォーマットコンバータの作成に成功した場合
			if (SUCCEEDED(hresult)) {
				// フォーマットコンバータの初期化
				hresult = p_converter->Initialize(
					p_source,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}
			// フォーマットコンバータの初期化に成功した場合
			if (SUCCEEDED(hresult)) {
				if (p_render_target_) {
					// ビットマップを作成
					hresult = p_render_target_->CreateBitmapFromWicBitmap(
						p_converter,
						NULL,
						&p_d2d1_bitmap
					);
				}
				else {
					hresult = E_FAIL;
				}
			}
			// ビットマップの作成に成功した場合
			if (SUCCEEDED(hresult)) {
				p_bitmap = std::make_unique<DirectxBitmap>();
				p_bitmap->set_data(p_d2d1_bitmap);
			}
			// ビットマップの作成に失敗した場合
			else {
				// ビットマップを開放
				dx_release(&p_d2d1_bitmap);
			}

			// コンバータ・フレーム・デコーダ・ストリームを開放
			dx_release(&p_converter);
			dx_release(&p_source);
			dx_release(&p_decoder);
			dx_release(&p_stream);

			return std::move(p_bitmap);
		}



		// ビットマップを描画(アンカーポイント指定)
		void DirectxGraphics::draw_bitmap(
			const Bitmap* p_bitmap,
			const Point<float>& point,
			const AnchorPosition& anchor_pos,
			float opacity
		) noexcept {
			if (drawing_) {
				auto p_d2d1_bitmap = p_bitmap->get_data<ID2D1Bitmap*>();
				D2D1_RECT_F rect_f;
				float left, top, right, bottom;

				if (p_d2d1_bitmap) {
					auto size_u = p_d2d1_bitmap->GetPixelSize();

					// アンカー位置 (水平方向)
					switch (anchor_pos.horizontal) {
					case AnchorPosition::Horizontal::Left:
						left = point.x;
						right = point.x + size_u.width;
						break;

					case AnchorPosition::Horizontal::Right:
						left = point.x - size_u.width;
						right = point.x;
						break;

					default:
						left = point.x - size_u.width * 0.5f;
						right = point.x + size_u.width * 0.5f;
					}

					// アンカー位置 (垂直方向)
					switch (anchor_pos.vertical) {
					case AnchorPosition::Vertical::Top:
						top = point.y;
						bottom = point.y + size_u.height;
						break;

					case AnchorPosition::Vertical::Bottom:
						top = point.y - size_u.height;
						bottom = point.y;
						break;

					default:
						top = point.y - size_u.height * 0.5f;
						bottom = point.y + size_u.height * 0.5f;
					}

					if (p_render_target_) {
						p_render_target_->DrawBitmap(
							p_d2d1_bitmap,
							D2D1::RectF(left, top, right, bottom),
							opacity
						);
					}
				}
			}
		}


		// ビットマップを描画(矩形指定)
		void DirectxGraphics::draw_bitmap(
			const Bitmap* p_bitmap,
			const Rectangle<float>& rect,
			float opacity
		) noexcept {
			auto p_d2d1_bitmap = p_bitmap->get_data<ID2D1Bitmap*>();

			if (drawing_ and p_d2d1_bitmap and p_render_target_) {
				p_render_target_->DrawBitmap(
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


		// テキストを描画(アンカーポイント指定)
		void DirectxGraphics::draw_text(
			const std::string& text,
			const Point<float>& point,
			const Font& font,
			const AnchorPosition& anchor_pos,
			const ColorF& color
		) noexcept {
			
		}

		// テキストを描画(矩形指定)
		void DirectxGraphics::draw_text(
			const std::string& text,
			const Rectangle<float>& rect,
			const Font& font,
			const AnchorPosition& anchor_pos,
			bool fit_size,
			const ColorF& color
		) noexcept {
			HRESULT hresult = S_OK;
			IDWriteTextLayout* p_text_layout = nullptr;
			D2D1_SIZE_F target_size = p_render_target_->GetSize();

			if (drawing_ and apply_text_layout(
				text,
				font,
				D2D1::SizeF(
					rect.get_width(),
					rect.get_height()
				),
				anchor_pos,
				&p_text_layout
			)) {
				DWRITE_TEXT_METRICS text_metrics;
				hresult = p_text_layout->GetMetrics(&text_metrics);

				if (SUCCEEDED(hresult)) {
					if (fit_size and target_size.width < text_metrics.width) {
						auto new_font = Font{
							font.height * target_size.width / text_metrics.width,
							font.family_name, font.style, font.weight
						};
						draw_text(text, rect, new_font, anchor_pos, false, color);
					}
					else if (p_render_target_ and p_brush_) {
						p_brush_->SetColor(color.d2d1_colorf());
						p_render_target_->DrawTextLayout(
							D2D1::Point2F(0, 0),
							p_text_layout,
							p_brush_
						);
					}
				}
			}
			dx_release(&p_text_layout);
		}
	}
}