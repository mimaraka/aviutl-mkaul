#define NOMINMAX
#include "graphics_gdiplus.hpp"
#include "bitmap_gdiplus.hpp"
#include <strconv2/strconv2.h>



namespace mkaul {
	namespace graphics {
		// 描画環境の用意
		bool GdiplusGraphics::startup() noexcept {
			Gdiplus::GdiplusStartupInput gsi;

			auto status_ = Gdiplus::GdiplusStartup(
				&gdiplus_token_,
				&gsi,
				NULL
			);

			if (status_ == Gdiplus::Ok)
				return true;
			else return false;
		}


		// 描画環境の破棄
		void GdiplusGraphics::shutdown() noexcept {
			Gdiplus::GdiplusShutdown(gdiplus_token_);
		}


		// Strokeの情報をPenに反映
		void GdiplusGraphics::get_gdip_pen(
			const ColorF& color,
			const Stroke& stroke,
			_Out_ Gdiplus::Pen* p_pen
		) noexcept {
			ColorI8 col_i8{ color };
			if (stroke.width < 1.f) {
				col_i8.set_a((int)(col_i8.get_a() * (0.3f + stroke.width * 0.7f)));
			}
			p_pen->SetColor(
				Gdiplus::Color(
					(BYTE)col_i8.get_a(),
					(BYTE)col_i8.get_r(),
					(BYTE)col_i8.get_g(),
					(BYTE)col_i8.get_b()
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


		// Brushの色を反映
		void GdiplusGraphics::get_gdip_brush(
			const ColorF& color,
			_Out_ Gdiplus::SolidBrush* p_brush
		) noexcept {
			ColorI8 col_i8{ color };

			p_brush->SetColor(
				Gdiplus::Color(
					(BYTE)col_i8.get_a(),
					(BYTE)col_i8.get_r(),
					(BYTE)col_i8.get_g(),
					(BYTE)col_i8.get_b()
				)
			);
		}


		void GdiplusGraphics::get_gdip_font(
			const Font& font,
			_Out_ Gdiplus::Font** pp_font
		) noexcept {
			Gdiplus::FontStyle font_style = Gdiplus::FontStyleRegular;
			if ((bool)(font.style & flag::FontStyle::Italic)) {
				font_style = (Gdiplus::FontStyle)(font_style | Gdiplus::FontStyleItalic);
			}
			if (500 < font.weight) {
				font_style = (Gdiplus::FontStyle)(font_style | Gdiplus::FontStyleBold);
			}
			if ((bool)(font.style & flag::FontStyle::UnderLine)) {
				font_style = (Gdiplus::FontStyle)(font_style | Gdiplus::FontStyleUnderline);
			}
			auto f = Gdiplus::Font{ ::sjis_to_wide(font.family_name).c_str(), font.height, font_style };
			*pp_font = f.Clone();
		}


		// 初期化(インスタンス毎)
		bool GdiplusGraphics::init(HWND hwnd) noexcept {
			if (::IsWindow(hwnd)) {
				hwnd_ = hwnd;
				return true;
			}
			else return false;
		}


		void GdiplusGraphics::release() noexcept {
			p_bitmap_buffer_.reset();
			p_graphics_buffer_.reset();
		}


		// 描画開始
		bool GdiplusGraphics::begin_draw() noexcept {
			if (!drawing_) {
				RECT rect;
				if (::GetClientRect(hwnd_, &rect)) {
					p_bitmap_buffer_ = std::make_unique<Gdiplus::Bitmap>(rect.right, rect.bottom);
					p_graphics_buffer_ = std::make_unique<Gdiplus::Graphics>(p_bitmap_buffer_.get());

					if (p_bitmap_buffer_ and p_graphics_buffer_) {
						p_graphics_buffer_->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
						p_graphics_buffer_->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
						drawing_ = true;
						return true;
					}
				}
			}
			return false;
		}


		// 描画終了(&バッファ送信)
		bool GdiplusGraphics::end_draw() noexcept {
			bool result = false;

			if (drawing_) {
				if (p_graphics_buffer_ and p_bitmap_buffer_) {
					PAINTSTRUCT ps;
					HDC hdc = ::BeginPaint(hwnd_, &ps);
					Gdiplus::Graphics p_graphics{hdc};
					p_graphics.DrawImage(p_bitmap_buffer_.get(), 0, 0);

					::EndPaint(hwnd_, &ps);
					drawing_ = false;

					result = true;
				}
				else result = false;
			}
			return result;
		}


		// 背景を塗りつぶし
		void GdiplusGraphics::fill_background(
			const ColorF& color
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::SolidBrush brush{Gdiplus::Color{0}};
				RECT rect;

				get_gdip_brush(color, &brush);

				if (::GetClientRect(hwnd_, &rect)) {
					// 境界付近が切れるので広めにとる
					p_graphics_buffer_->FillRectangle(
						&brush,
						Gdiplus::Rect{
							rect.left - 1,
							rect.top - 1,
							rect.right + 1,
							rect.bottom + 1
						}
					);
				}
			}
		}


		// 線を描画
		void GdiplusGraphics::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				p_graphics_buffer_->DrawLine(
					&pen,
					pt_from.to<Gdiplus::PointF>(),
					pt_to.to<Gdiplus::PointF>()
				);
			}
		}


		// 線を描画(複数)
		void GdiplusGraphics::draw_lines(
			const Point<float>* points,
			size_t n_points,
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				auto gdiplus_points = new Gdiplus::PointF[n_points];

				for (size_t i = 0; i < n_points; i++) {
					gdiplus_points[i].X = points[i].x;
					gdiplus_points[i].Y = points[i].y;
				}

				p_graphics_buffer_->DrawLines(
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
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				p_graphics_buffer_->DrawBezier(
					&pen,
					point_0.to<Gdiplus::PointF>(),
					point_1.to<Gdiplus::PointF>(),
					point_2.to<Gdiplus::PointF>(),
					point_3.to<Gdiplus::PointF>()
				);
			}
		}


		// ベジェ曲線を描画(複数)
		void GdiplusGraphics::draw_beziers(
			const Point<float>* points,
			size_t n_points,
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				auto gdip_points = new Gdiplus::PointF[n_points];

				for (size_t i = 0; i < n_points; i++) {
					gdip_points[i].X = points[i].x;
					gdip_points[i].Y = points[i].y;
				}

				p_graphics_buffer_->DrawBeziers(
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
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);
				
				// 角丸矩形を描画
				if (0 < round_radius_x or 0 < round_radius_y) {
					float angle = 180.f;
					Gdiplus::GraphicsPath path;

					// 半径のリミット
					round_radius_x = std::min(round_radius_x, rect.get_width() * 0.5f);
					round_radius_y = std::min(round_radius_y, rect.get_height() * 0.5f);

					// 左上
					path.AddArc(
						Gdiplus::RectF{
							std::min(rect.left, rect.right),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					// 右上
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF{
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					// 右下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF{
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					// 左下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF{
							std::min(rect.left, rect.right),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					path.CloseAllFigures();
					p_graphics_buffer_->DrawPath(&pen, &path);
				}
				// 矩形を描画
				else {
					p_graphics_buffer_->DrawRectangle(
						&pen,
						Gdiplus::RectF{
							rect.left,
							rect.top,
							rect.get_width(),
							rect.get_height()
						}
					);
				}
			}
		}


		// 矩形を描画(塗り)
		void GdiplusGraphics::fill_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const ColorF& color
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::SolidBrush brush{Gdiplus::Color{0}};

				get_gdip_brush(color, &brush);
				
				// 角丸矩形を描画
				if (0 < round_radius_x or 0 < round_radius_y) {
					float angle = 180.f;
					Gdiplus::GraphicsPath path;

					// 半径のリミット
					round_radius_x = std::min(round_radius_x, rect.get_width() * 0.5f);
					round_radius_y = std::min(round_radius_y, rect.get_height() * 0.5f);

					// 左上
					path.AddArc(
						Gdiplus::RectF{
							std::min(rect.left, rect.right),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					// 右上
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF{
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::min(rect.top, rect.bottom),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					// 右下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF{
							std::max(rect.left, rect.right) - (round_radius_x * 2.f),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					// 左下
					angle += 90.f;
					path.AddArc(
						Gdiplus::RectF{
							std::min(rect.left, rect.right),
							std::max(rect.top, rect.bottom) - (round_radius_y * 2.f),
							round_radius_x * 2.f,
							round_radius_y * 2.f
						},
						angle,
						90.f
					);

					path.CloseAllFigures();
					p_graphics_buffer_->FillPath(&brush, &path);
				}
				// 矩形を描画
				else {
					p_graphics_buffer_->FillRectangle(
						&brush,
						Gdiplus::RectF{
							rect.left,
							rect.top,
							rect.get_width(),
							rect.get_height()
						}
					);
				}
			}
		}


		// 楕円を描画(線)(中心点指定)
		void GdiplusGraphics::draw_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				radius_x = std::abs(radius_x);
				radius_y = std::abs(radius_y);

				Gdiplus::RectF rect_f{
					point.x - radius_x,
					point.y - radius_y,
					radius_x * 2.f,
					radius_y * 2.f
				};

				p_graphics_buffer_->DrawEllipse(&pen, rect_f);
			}
		}


		// 楕円を描画(線)(矩形指定)
		void GdiplusGraphics::draw_ellipse(
			const Rectangle<float>& rectangle,
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				Gdiplus::RectF rect_f{
					rectangle.left,
					rectangle.top,
					rectangle.right,
					rectangle.bottom
				};

				p_graphics_buffer_->DrawEllipse(&pen, rect_f);
			}
		}


		// 楕円を描画(塗り)(中心点指定)
		void GdiplusGraphics::fill_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const ColorF& color
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::SolidBrush brush{Gdiplus::Color{0}};
				get_gdip_brush(color, &brush);

				radius_x = std::abs(radius_x);
				radius_y = std::abs(radius_y);

				Gdiplus::RectF rect_f{
					point.x - radius_x,
					point.y - radius_y,
					radius_x * 2.f,
					radius_y * 2.f
				};

				p_graphics_buffer_->FillEllipse(&brush, rect_f);
			}
		}


		// 楕円を描画(塗り)(矩形指定)
		void GdiplusGraphics::fill_ellipse(
			const Rectangle<float>& rectangle,
			const ColorF& color
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::SolidBrush brush{Gdiplus::Color{0}};
				get_gdip_brush(color, &brush);

				Gdiplus::RectF rect_f{
					rectangle.left,
					rectangle.top,
					rectangle.right,
					rectangle.bottom
				};

				p_graphics_buffer_->FillEllipse(&brush, rect_f);
			}
		}


		// パスを描画(線)
		void GdiplusGraphics::draw_path(
			const Path* p_path,
			const ColorF& color,
			const Stroke& stroke
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::Pen pen{Gdiplus::Color{0}};
				get_gdip_pen(color, stroke, &pen);

				p_graphics_buffer_->DrawPath(
					&pen,
					p_path->get_data<Gdiplus::GraphicsPath*>(0)
				);
			}
		}


		// パスを描画(塗り)
		void GdiplusGraphics::fill_path(
			const Path* p_path,
			const ColorF& color
		) noexcept {
			if (drawing_ and p_graphics_buffer_) {
				Gdiplus::SolidBrush brush{Gdiplus::Color{0}};
				get_gdip_brush(color, &brush);

				p_graphics_buffer_->FillPath(
					&brush,
					p_path->get_data<Gdiplus::GraphicsPath*>(0)
				);
			}
		}


		// 空のビットマップを作成
		std::unique_ptr<Bitmap> GdiplusGraphics::create_bitmap(
			const Size<unsigned>& size,
			const ColorF& color
		) noexcept {
			auto p_gdip_bitmap = new Gdiplus::Bitmap{ (INT)size.width, (INT)size.height };

			if (p_gdip_bitmap and p_gdip_bitmap->GetLastStatus() == Gdiplus::Ok) {
				auto p_bitmap = std::make_unique<GdiplusBitmap>();
				p_bitmap->set_data(p_gdip_bitmap);
				return p_bitmap;
			}
			else return nullptr;
		}


		// ファイルからビットマップを作成
		std::unique_ptr<Bitmap> GdiplusGraphics::load_bitmap_from_filename(
			const std::filesystem::path& path
		) noexcept {
			Gdiplus::Bitmap* p_gdip_bitmap = nullptr;
			p_gdip_bitmap = new Gdiplus::Bitmap{ path.c_str() };

			if (p_gdip_bitmap and p_gdip_bitmap->GetWidth() != 0 and p_gdip_bitmap->GetHeight() != 0) {
				auto p_bitmap = std::make_unique<GdiplusBitmap>();
				p_bitmap->set_data(p_gdip_bitmap);
				return p_bitmap;
			}
			else return nullptr;
		}


		// リソースからビットマップを作成
		std::unique_ptr<Bitmap> GdiplusGraphics::load_bitmap_from_resource(
			HINSTANCE hinst,
			const char* res_name,
			const char* res_type
		) noexcept {
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
					auto hresult = ::CreateStreamOnHGlobal(res_buf_handle, FALSE, &p_stream);

					// ストリームの作成に成功した場合
					if (SUCCEEDED(hresult)) {
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
			if (p_gdip_bitmap and p_gdip_bitmap->GetLastStatus() == Gdiplus::Ok) {
				auto p_bitmap = std::make_unique<GdiplusBitmap>();
				p_bitmap->set_data(p_gdip_bitmap);
				return p_bitmap;
			}
			else return nullptr;
		}


		// ビットマップを描画(アンカーポイント指定)
		void GdiplusGraphics::draw_bitmap(
			const Bitmap* bitmap,
			const Point<float>& point,
			const AnchorPosition& anchor_pos,
			float opacity
		) noexcept {
			auto p_gdip_bitmap = bitmap->get_data<Gdiplus::Bitmap*>();
			float width = static_cast<float>(p_gdip_bitmap->GetWidth());
			float height = static_cast<float>(p_gdip_bitmap->GetHeight());
			float x, y;

			// アンカー位置 (水平方向)
			switch (anchor_pos.horizontal) {
			case AnchorPosition::Horizontal::Left:
				x = point.x;
				break;

			case AnchorPosition::Horizontal::Right:
				x = point.x - width;
				break;

			default:
				x = point.x - width * 0.5f;
			}

			// アンカー位置 (垂直方向)
			switch (anchor_pos.vertical) {
			case AnchorPosition::Vertical::Top:
				y = point.y;
				break;

			case AnchorPosition::Vertical::Bottom:
				y = point.y - height;
				break;

			default:
				y = point.y - height * 0.5f;
			}

			Gdiplus::ColorMatrix color_matrix{
				1.f, 0.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 0.f, opacity, 0.f,
				0.f, 0.f, 0.f, 0.f, 1.f
			};

			Gdiplus::ImageAttributes image_attributes;
			image_attributes.SetColorMatrix(
				&color_matrix,
				Gdiplus::ColorMatrixFlagsDefault,
				Gdiplus::ColorAdjustTypeBitmap
			);
	
			p_graphics_buffer_->DrawImage(
				p_gdip_bitmap,
				Gdiplus::RectF{ x, y, width, height }
			);
		}


		// ビットマップを描画(矩形指定)
		void GdiplusGraphics::draw_bitmap(
			const Bitmap* bitmap,
			const Rectangle<float>& rect,
			float opacity
		) noexcept {
			Gdiplus::ColorMatrix color_matrix{
				1.f, 0.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 0.f, opacity, 0.f,
				0.f, 0.f, 0.f, 0.f, 1.f
			};

			Gdiplus::ImageAttributes image_attributes;
			image_attributes.SetColorMatrix(
				&color_matrix,
				Gdiplus::ColorMatrixFlagsDefault,
				Gdiplus::ColorAdjustTypeBitmap
			);

			p_graphics_buffer_->DrawImage(
				bitmap->get_data<Gdiplus::Bitmap*>(),
				Gdiplus::RectF{
					rect.left,
					rect.top,
					rect.right - rect.left,
					rect.bottom - rect.top
				},
				0.f, 0.f, (Gdiplus::REAL)bitmap->get_width(), (Gdiplus::REAL)bitmap->get_height(),
				Gdiplus::UnitPixel, &image_attributes
			);
		}


		// テキストを描画(アンカーポイント指定)
		void GdiplusGraphics::draw_text(
			const std::string& text,
			const Point<float>& point,
			const Font& font,
			const AnchorPosition& anchor_pos,
			const ColorF& color
		) noexcept {
			Gdiplus::SolidBrush brush{ Gdiplus::Color{0} };
			get_gdip_brush(color, &brush);

			Gdiplus::Font* p_gdip_font = nullptr;
			get_gdip_font(font, &p_gdip_font);

			Gdiplus::StringFormat string_format;
			string_format.SetAlignment(Gdiplus::StringAlignmentCenter);
			string_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			
			p_graphics_buffer_->DrawString(
				::sjis_to_wide(text).c_str(),
				-1,
				p_gdip_font,
				point.to<Gdiplus::PointF>(),
				&brush
			);
			delete p_gdip_font;
		}


		// テキストを描画(矩形指定)
		void GdiplusGraphics::draw_text(
			const std::string& text,
			const Rectangle<float>& rect,
			const Font& font,
			const AnchorPosition& anchor_pos,
			bool fit_size,
			const ColorF& color
		) noexcept {
			Gdiplus::SolidBrush brush{ Gdiplus::Color{0} };
			get_gdip_brush(color, &brush);

			Gdiplus::Font* p_gdip_font = nullptr;
			get_gdip_font(font, &p_gdip_font);

			Gdiplus::StringFormat string_format;
			string_format.SetAlignment(Gdiplus::StringAlignmentCenter);
			string_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			string_format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsNoFitBlackBox);
			string_format.SetTrimming(Gdiplus::StringTrimmingNone);

			auto character_range = Gdiplus::CharacterRange{ 0, (int)text.size() };
			string_format.SetMeasurableCharacterRanges(1, &character_range);

			Gdiplus::Region region;
			RECT tmp;
			if (::GetClientRect(hwnd_, &tmp)) {
				Gdiplus::RectF rect_wnd{
					0, 0,
					(Gdiplus::REAL)tmp.right,
					(Gdiplus::REAL)tmp.bottom
				};
				p_graphics_buffer_->MeasureCharacterRanges(
					::sjis_to_wide(text).c_str(),
					-1,
					p_gdip_font,
					rect_wnd,
					&string_format,
					1,
					&region
				);
				Gdiplus::RectF rect_text;
				region.GetBounds(&rect_text, p_graphics_buffer_.get());

				Gdiplus::SizeF size_rect, size_rect_text;
				rect_wnd.GetSize(&size_rect);
				rect_text.GetSize(&size_rect_text);

				if (fit_size and size_rect.Width < size_rect_text.Width) {
					delete p_gdip_font;
					Font new_font{
						font.height* (size_rect.Width / size_rect_text.Width),
						font.family_name,
						font.style,
						font.weight
					};
					draw_text(text, rect, new_font, anchor_pos, false, color);
				}
				else {
					p_graphics_buffer_->DrawString(
						::sjis_to_wide(text).c_str(),
						-1,
						p_gdip_font,
						rect_wnd,
						&string_format,
						&brush
					);
					delete p_gdip_font;
				}
			}
		}
	}
}