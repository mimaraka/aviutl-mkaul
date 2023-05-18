//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "common.hpp"
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


		// �J��
		void Path_Gdiplus::release()
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);
			if (p_path)
				delete p_path;
			data[0] = nullptr;
		}


		// �p�X�̊J�n
		bool Path_Gdiplus::begin(const Point<float>& pt)
		{
			if (!data[0]) {
				auto p_path = new Gdiplus::GraphicsPath();
				data[0] = p_path;

				pt_last = pt;

				return true;
			}
			else return false;
		}


		// �p�X�̏I��
		void Path_Gdiplus::end(Figure_End fe)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (fe == Figure_End::Closed)
				p_path->CloseAllFigures();
		}


		// �ʂ�ǉ�
		void Path_Gdiplus::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);
			constexpr float pi = std::numbers::pi_v<float>;
			Point<float> pt_ofs_start, pt_o;

			// �p�x��-2�� ~ 2�΂͈̔͂Ɏ��߂�
			angle_sweep = std::fmodf(angle_sweep, pi * 2.f);

			ellipse_pos(size, angle_start, &pt_ofs_start);
			pt_o = pt_last - pt_ofs_start;

			if (p_path) p_path->AddArc(
				Gdiplus::RectF(
					pt_o.x - size.width,
					pt_o.y - size.height,
					size.width * 2.f,
					size.height * 2.f
				),
				-rad2deg(angle_start),
				-rad2deg(angle_sweep)
			);
		}


		// ����ǉ�
		void Path_Gdiplus::add_line(
			const Point<float>& pt
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (p_path) p_path->AddLine(
				Gdiplus::PointF(pt_last.x, pt_last.y),
				Gdiplus::PointF(pt.x, pt.y)
			);
		}


		// �x�W�F�Ȑ���ǉ�
		void Path_Gdiplus::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (p_path) p_path->AddBezier(
				Gdiplus::PointF(pt_last.x, pt_last.y),
				Gdiplus::PointF(pt_0.x, pt_0.y),
				Gdiplus::PointF(pt_1.x, pt_1.y),
				Gdiplus::PointF(pt_2.x, pt_2.y)
			);
		}


		// �`����̗p��
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
			else return false;
		}


		// �`����̔j��
		void Graphics_Gdiplus::shutdown()
		{
			Gdiplus::GdiplusShutdown(gdiplus_token);
		}


		// Stroke�̏���Pen�ɔ��f
		void Graphics_Gdiplus::apply_pen_style(Gdiplus::Pen* p_pen, const Color_F& col_f, const Stroke& stroke)
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


		// ������(�C���X�^���X��)
		bool Graphics_Gdiplus::init(HWND hwnd_)
		{
			hwnd = hwnd_;
			return true;
		}


		// �I��(�C���X�^���X��)
		void Graphics_Gdiplus::exit()
		{
			if (p_bitmap_buffer) {
				delete p_bitmap_buffer;
				p_bitmap_buffer = nullptr;
			}
		}


		// �`��J�n
		void Graphics_Gdiplus::begin_draw()
		{
			delete p_graphics_buffer;
			delete p_bitmap_buffer;

			RECT rect_wnd;
			::GetClientRect(hwnd, &rect_wnd);
			p_bitmap_buffer = new Gdiplus::Bitmap(rect_wnd.right, rect_wnd.bottom);
			p_graphics_buffer = new Gdiplus::Graphics(p_bitmap_buffer);
			p_graphics_buffer->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		}


		// �`��I��(&�o�b�t�@���M)
		bool Graphics_Gdiplus::end_draw()
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


		// ����`��
		void Graphics_Gdiplus::draw_line(
			const Point<float>& pt_from,
			const Point<float>& pt_to,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, col_f, stroke);

				p_graphics_buffer->DrawLine(
					&pen,
					Gdiplus::PointF(pt_from.x, pt_from.y),
					Gdiplus::PointF(pt_to.x, pt_to.y)
				);
			}
		}


		// ����`��(����)
		void Graphics_Gdiplus::draw_lines(
			const Point<float>* points,
			size_t n_points,
			const Color_F& col_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
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


		// �x�W�F�Ȑ���`��
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
				Gdiplus::Pen pen(Gdiplus::Color(0));
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


		// �x�W�F�Ȑ���`��(����)
		void Graphics_Gdiplus::draw_beziers(
			const Point<float>* points,
			size_t n_points,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
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


		// ��`��`��(��)
		void Graphics_Gdiplus::draw_rectangle(
			const Rectangle<float>& rect,
			float round_radius_x,
			float round_radius_y,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
				apply_pen_style(&pen, color_f, stroke);
				
				// �p�ۋ�`��`��
				if (round_radius_x > 0 || round_radius_y > 0) {
					float angle = 180.f;
					Gdiplus::GraphicsPath path;

					// ���a�̃��~�b�g
					round_radius_x = std::min(round_radius_x, std::abs(rect.right - rect.left) * 0.5f);
					round_radius_y = std::min(round_radius_y, std::abs(rect.bottom - rect.top) * 0.5f);

					// ����
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

					// �E��
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

					// �E��
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

					// ����
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
				// ��`��`��
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


		// ��`��`��(�h��)
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
				
				// �p�ۋ�`��`��
				if (round_radius_x > 0 || round_radius_y > 0) {
					float angle = 180.f;
					Gdiplus::GraphicsPath path;

					// ���a�̃��~�b�g
					round_radius_x = std::min(round_radius_x, std::abs(rect.right - rect.left) * 0.5f);
					round_radius_y = std::min(round_radius_y, std::abs(rect.bottom - rect.top) * 0.5f);

					// ����
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

					// �E��
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

					// �E��
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

					// ����
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
				// ��`��`��
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


		// �ȉ~��`��(��)(���S�_�w��)
		void Graphics_Gdiplus::draw_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
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


		// �ȉ~��`��(��)(��`�w��)
		void Graphics_Gdiplus::draw_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& color_f,
			const Stroke& stroke
		)
		{
			if (p_graphics_buffer) {
				Gdiplus::Pen pen(Gdiplus::Color(0));
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


		// �ȉ~��`��(�h��)(���S�_�w��)
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


		// �ȉ~��`��(�h��)(��`�w��)
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


		// ��̃r�b�g�}�b�v���쐬
		void Graphics_Gdiplus::initialize_bitmap(
			Bitmap* p_bitmap,
			const Size<unsigned>& size,
			Color_F color_f
		)
		{
			p_bitmap->data = new Gdiplus::Bitmap(size.width, size.height);
		}


		// �t�@�C������r�b�g�}�b�v���쐬
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


		// ���\�[�X����r�b�g�}�b�v���쐬
		bool Graphics_Gdiplus::load_bitmap_from_resource(
			Bitmap* p_bitmap,
			HINSTANCE hinst,
			const char* res_name,
			const char* res_type
		)
		{
			wchar_t* wc = nullptr;
			// �r�b�g�}�b�v(GDI+)�̃|�C���^
			Gdiplus::Bitmap* p_gdip_bitmap = nullptr;

			// ���\�[�X�^�C�v��BITMAP�̂Ƃ�
			if (res_type == RT_BITMAP) {
				// ���\�[�X���ʎq��������̏ꍇ
				if (HIWORD(res_name)) {
					// const char* ���� wchar_t* �ɕϊ�
					size_t size_resource = strlen(res_name) + 1;
					wc = new wchar_t[size_resource];

					size_t size;
					::mbstowcs_s(&size, wc, size_resource, res_name, size_resource);
				}
				// ���\�[�X���ʎq�����l�̏ꍇ
				else
					wc = reinterpret_cast<wchar_t*>(const_cast<char*>(res_name));

				p_gdip_bitmap = Gdiplus::Bitmap::FromResource(hinst, wc);

				if (HIWORD(res_name)) delete[] wc;
			}
			// ����ӊO�̂Ƃ�
			else {
				// ���\�[�X�̃n���h��
				HRSRC res_handle = nullptr;
				// ���\�[�X�̃T�C�Y
				DWORD res_size = 0;
				// ���\�[�X�f�[�^�̃n���h��
				HGLOBAL res_data_handle = nullptr;
				// ���\�[�X�f�[�^
				void* res_data = nullptr;
				// �������m�ۗp���\�[�X�n���h��
				HGLOBAL res_buf_handle = nullptr;
				// �m�ۂ��郁�����̃|�C���^(�����Ƀ��\�[�X�f�[�^��z�u����)
				void* res_buf_data = nullptr;
				// �X�g���[���̃|�C���^
				IStream* p_stream = nullptr;

				// ���\�[�X��T��
				res_handle = ::FindResource(hinst, res_name, res_type);

				// ���\�[�X�����������ꍇ
				if (res_handle)
					// ���\�[�X�̃T�C�Y���擾
					res_size = ::SizeofResource(hinst, res_handle);

				// �����E�C���X�댟�m�|�C���g
				// ���\�[�X�̃T�C�Y�����݂���ꍇ
				if (res_size)
					// ���\�[�X�̃n���h�����擾
					res_data_handle = ::LoadResource(hinst, res_handle);

				// ���\�[�X�̃n���h�������݂���ꍇ
				if (res_data_handle)
					// ���\�[�X�����b�N(���\�[�X�̃|�C���^���擾)
					// ���ۂɂ̓����������b�N���Ȃ��炵��
					res_data = ::LockResource(res_data_handle);

				// ���\�[�X�̃|�C���^�����݂���ꍇ
				if (res_data)
					// �������̊m��
					res_buf_handle = ::GlobalAlloc(GMEM_MOVEABLE, res_size);

				// ���������m�ۂł����ꍇ
				if (res_buf_handle)
					// �������̃��b�N
					res_buf_data = ::GlobalLock(res_buf_handle);

				// �����������b�N�ł����ꍇ
				if (res_buf_data) {
					// ���b�N�����������̈�Ƀ��\�[�X�̃f�[�^���R�s�[
					::CopyMemory(res_buf_data, res_data, res_size);
					// ����������X�g���[�����쐬
					auto hr = ::CreateStreamOnHGlobal(res_buf_handle, FALSE, &p_stream);

					// �X�g���[���̍쐬�ɐ��������ꍇ
					if (SUCCEEDED(hr)) {
						// �X�g���[������r�b�g�}�b�v���쐬
						p_gdip_bitmap = Gdiplus::Bitmap::FromStream(p_stream);

						// �X�g���[�����J��
						if (p_stream) p_stream->Release();
					}
				}

				// �������̉����E�J��
				if (res_buf_handle) {
					::GlobalUnlock(res_buf_handle);
					::GlobalFree(res_buf_handle);
				}
			}

			// �r�b�g�}�b�v�̍쐬�ɐ��������ꍇ
			if (p_gdip_bitmap && p_gdip_bitmap->GetLastStatus() == Gdiplus::Ok) {
				p_bitmap->data = p_gdip_bitmap;
				return true;
			}
			else return false;
		}


		// �r�b�g�}�b�v��`��(�A���J�[�|�C���g�w��)
		void Graphics_Gdiplus::draw_bitmap(
			const Bitmap* bitmap,
			const Point<float>& point,
			Anchor_Position anchor_pos,
			float opacity
		)
		{
			auto p_gdip_bitmap = reinterpret_cast<Gdiplus::Bitmap*>(bitmap->data);
			Gdiplus::RectF rect_f;
			int width = p_gdip_bitmap->GetWidth();
			int height = p_gdip_bitmap->GetHeight();

			// �A���J�[�|�C���g�̈ʒu
			switch (anchor_pos) {
			case Anchor_Position::Center:
			default:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Left:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Top:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Right:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y - height * 0.5f,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Bottom:
				rect_f = Gdiplus::RectF(
					point.x - width * 0.5f,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Left_Top:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Right_Top:
				rect_f = Gdiplus::RectF(
					point.x - width,
					point.y,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Left_Bottom:
				rect_f = Gdiplus::RectF(
					point.x,
					point.y - height,
					(Gdiplus::REAL)width,
					(Gdiplus::REAL)height
				);
				break;

			case Anchor_Position::Right_Bottom:
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


		// �r�b�g�}�b�v��`��(��`�w��)
		void Graphics_Gdiplus::draw_bitmap(
			const Bitmap* bitmap,
			const Rectangle<float>& rect,
			float opacity
		)
		{
			p_graphics_buffer->DrawImage(
				reinterpret_cast<Gdiplus::Bitmap*>(bitmap->data),
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