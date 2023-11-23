//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "point.hpp"
#include "size.hpp"
#include "rectangle.hpp"
#include "color.hpp"
#include "util.hpp"
#include <string>
#include <filesystem>
#include <Windows.h>



namespace mkaul {
	namespace graphics {
		// �X�g���[�N
		struct Stroke {
		public:
			float width;
			enum class DashStyle {
				Solid,
				Dash,
				Dot,
				DashDot,
				DashDotDot,
				Custom
			} dash_style;
			enum class CapStyle {
				Flat,
				Square,
				Round,
				Triangle
			} start_cap, end_cap, dash_cap;
			enum class LineJoin {
				Miter,
				Bevel,
				Round
			} line_join;
			float* custom_dashes;
			int dashes_count;
			float dash_offset;

			Stroke(
				float width_ = 1.f,
				DashStyle dash_style_ = DashStyle::Solid,
				CapStyle start_cap_ = CapStyle::Round,
				CapStyle end_cap_ = CapStyle::Round,
				CapStyle dash_cap_ = CapStyle::Round,
				LineJoin line_join_ = LineJoin::Round,
				float* custom_dashes_ = nullptr,
				int dashes_count_ = 0,
				float dash_offset_ = 0.f
			) :
				width(width_),
				dash_style(dash_style_),
				start_cap(start_cap_),
				end_cap(end_cap_),
				dash_cap(dash_cap_),
				line_join(line_join_),
				custom_dashes(custom_dashes_),
				dashes_count(dashes_count_),
				dash_offset(dash_offset_)
			{}
		};


		// �t�H���g
		struct Font {
			enum class FontStyle {
				None
			} style;

			std::string family_name;
			float height;
			int weight;

			Font(
				const std::string& family_name_ = "���C���I",
				float height_ = 24.f,
				FontStyle style_ = FontStyle::None,
				int weight_ = 500
			) :
				style(style_),
				family_name(family_name_),
				height(height_),
				weight(weight_)
			{}
		};


		// �r�b�g�}�b�v(���ۃN���X)
		struct Bitmap {
		protected:
			void* data;

		public:
			Bitmap() :
				data(nullptr)
			{}

			// �r�b�g�}�b�v��j��
			virtual void release() = 0;

			// �f�[�^���擾
			template <typename Ptr>
			Ptr get_data() const
			{
				return reinterpret_cast<Ptr>(data);
			}
			// �f�[�^��ݒ�
			void set_data(void* ptr) { data = ptr; };

			// ���E�������擾
			virtual size_t get_width() const = 0;
			virtual size_t get_height() const = 0;
		};


		// �A���J�[�|�C���g�̈ʒu
		enum class AnchorPosition {
			Center,
			Left,
			Top,
			Right,
			Bottom,
			LeftTop,
			RightTop,
			LeftBottom,
			RightBottom
		};


		// �p�X
		struct Path {
		protected:
			void* data[2];
			Point<float> pt_last;

			void ellipse_pos(
				const Size<float>& size,
				float angle,
				Point<float>* p_pt
			);

		public:
			Path() :
				data(),
				pt_last()
			{}

			enum class FigureEnd {
				Open,
				Closed
			};

			virtual void release() = 0;

			template <typename Ptr>
			Ptr get_data(int idx) const
			{
				return reinterpret_cast<Ptr>(data[idx]);
			}

			virtual bool begin(const Point<float>& pt) = 0;
			virtual void end(FigureEnd fe = FigureEnd::Closed) = 0;

			// �ʂ�ǉ�
			virtual void add_arc(
				const Size<float>& size,
				float angle_start,
				float angle_sweep
			) = 0;

			// ����ǉ�
			virtual void add_line(
				const Point<float>& pt
			) = 0;

			// �x�W�F��ǉ�
			virtual void add_bezier(
				const Point<float>& pt_0,
				const Point<float>& pt_1,
				const Point<float>& pt_2
			) = 0;
		};


		// �p�xangle�ɂ���ȉ~��̓_
		// �p�x�͎��v��12���̕�����0d�Ƃ������v���̓x��
		inline void Path::ellipse_pos(
			const Size<float>& size,
			float angle,
			Point<float>* p_pt
		)
		{
			float rad = deg2rad(-angle + 90.f);
			float x = size.width * size.height / std::sqrt(size.height * size.height + size.width * size.width * (float)std::pow(std::tan(rad), 2)) * sign(std::cos(rad));
			float y = -x * std::tan(rad);

			p_pt->x = x;
			p_pt->y = y;
		}


		// �O���t�B�b�N�X���ۃN���X
		class Graphics {
		protected:
			HWND hwnd;
			bool drawing;

		public:
			Graphics():
				hwnd(NULL),
				drawing(false)
			{}

			virtual bool init(HWND hwnd_) = 0;
			virtual void exit() = 0;

			virtual bool begin_draw() = 0;
			virtual bool end_draw() = 0;

			bool is_drawing() { return drawing; }

			// ���T�C�Y
			virtual bool resize() = 0;

			// �w�i��h��Ԃ�
			virtual void fill_background(
				const ColorF& col_f = 0
			) = 0;

			// ����`��
			virtual void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ����`��(����)
			virtual void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// �x�W�F�Ȑ���`��
			virtual void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// �x�W�F�Ȑ���`��(����)
			virtual void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ��`��`��(��)
			virtual void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ��`��`��(�h��)
			virtual void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& col_f = 0
			) = 0;

			// �ȉ~��`��(��)(���S�_�w��)
			virtual void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// �ȉ~��`��(��)(��`�w��)
			virtual void draw_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// �ȉ~��`��(�h��)(���S�_�w��)
			virtual void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& col_f = 0
			) = 0;

			// �ȉ~��`��(�h��)(��`�w��)
			virtual void fill_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& col_f = 0
			) = 0;

			// �p�X��`��(��)
			virtual void draw_path(
				const Path* p_path,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// �p�X��`��(�h��)
			virtual void fill_path(
				const Path* p_path,
				const ColorF& col_f = 0
			) = 0;

			// ��̃r�b�g�}�b�v���쐬
			virtual bool initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				const ColorF& col_f = 0
			) = 0;

			// �t�@�C������r�b�g�}�b�v���쐬
			virtual bool load_bitmap_from_filename(
				Bitmap* p_bitmap,
				const std::filesystem::path& path
			) = 0;

			// ���\�[�X����r�b�g�}�b�v���쐬
			virtual bool load_bitmap_from_resource(
				Bitmap* p_bitmap,
				HINSTANCE hinst,
				const char* res_name,
				const char* res_type = RT_BITMAP
			) = 0;

			// �r�b�g�}�b�v��`��(�A���J�[�|�C���g�w��)
			virtual void draw_bitmap(
				const Bitmap* p_bitmap,
				const Point<float>& point,
				AnchorPosition anchor_pos,
				float opacity = 1.f
			) = 0;

			// �r�b�g�}�b�v��`��(��`�w��)
			virtual void draw_bitmap(
				const Bitmap* p_bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) = 0;

			// �e�L�X�g��`��(�A���J�[�|�C���g�w��)
			virtual void draw_text(
				const std::string& text,
				const Point<float>& point,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				const ColorF& col_f = 0
			) = 0;

			// �e�L�X�g��`��(��`�w��)
			virtual void draw_text(
				const std::string& text,
				const Rectangle<float>& rect,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				bool fit_size = true,
				const ColorF& col_f = 0
			) = 0;
		};
	}
}