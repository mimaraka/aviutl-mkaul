#pragma once

#include "control.hpp"



namespace mkaul {
	namespace window {
		// �{�^��
		class Button : public Control {
		protected:
			HWND                    hwnd_tooltip_;
			TOOLINFO	    		tool_info_;
			std::string				tooltip_label_;
			float					hover_highlight_;
			bool			    	hovered_, clicked_;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;

		public:
			Button() :
				hwnd_tooltip_(NULL),
				tool_info_({ 0 }),
				tooltip_label_(),
				hover_highlight_(0.f),
				hovered_(false),
				clicked_(false)
			{}

			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent,
				int id,
				const ColorF* p_color_bg,
				const ColorF* p_color_control,
				const WindowRectangle& rect,
				const std::string& tooltip_label = "",
				flag::RoundEdge round_edge = flag::RoundEdge::None,
				float round_radius = 0.f,
				float hover_highlight = 0.04f
			);
		};


		// �{�^��(���x��)
		class LabelButton : public Button {
		protected:
			std::string label_;
			graphics::Font font_;
			ColorF* p_color_label_;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;

		public:

			LabelButton() :
				label_(),
				font_(),
				p_color_label_(nullptr)
			{}

			// �{�^�����쐬
			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const std::string& label,
				const graphics::Font font,
				const ColorF* p_color_bg,
				const ColorF* p_color_control,
				const ColorF* p_color_label,
				const WindowRectangle& rect,
				const std::string& tooltip_label = "",
				flag::RoundEdge round_edge = flag::RoundEdge::None,
				float round_radius = 0.f,
				float hover_highlight = 0.04f
			);
		};


		// �{�^��(�A�C�R��)
		class IconButton : public Button {
		public:
			enum class SourceType {
				File,
				Resource
			};

		protected:
			graphics::Bitmap* icon_;
			SourceType source_type_;
			std::string icon_source_;
			uint16_t icon_resource_num_;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;

		public:
			IconButton() :
				icon_(nullptr),
				icon_source_(),
				source_type_(SourceType::File),
				icon_resource_num_(0)
			{}

			// �{�^�����쐬(�A�C�R��)
			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent,
				int id,
				const char* icon_source,
				SourceType source_type,
				const ColorF* p_color_bg,
				const ColorF* p_color_control,
				const WindowRectangle& rect,
				const std::string& tooltip_label = "",
				flag::RoundEdge round_edge = flag::RoundEdge::None,
				float round_radius = 0.f,
				float hover_highlight = 0.04f
			);
		};
	}
}