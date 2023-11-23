#pragma once

#include "control.hpp"



namespace mkaul {
	namespace window {
		// ボタン
		class Button : public Control {
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
				HWND hwnd_parent_,
				int id_,
				const ColorF* p_col_bg_,
				const ColorF* p_col_control_,
				const WindowRectangle& rect,
				const std::string& tooltip_label = "",
				RoundEdgeFlag round_edge_flag_ = RoundEdgeFlag::None,
				float round_radius_ = 0.f,
				float hover_highlight = 0.04f
			);

		protected:
			HWND                    hwnd_tooltip_;
			TOOLINFO	    		tool_info_;
			std::string				tooltip_label_;
			float					hover_highlight_;
			bool			    	hovered_, clicked_;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;
		};


		// ボタン(ラベル)
		class LabelButton : public Button {
		protected:
			std::string label_;
			graphics::Font font_;
			ColorF* p_col_label_;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;

		public:

			LabelButton() :
				label_(),
				font_(),
				p_col_label_(nullptr)
			{}

			// ボタンを作成
			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const std::string& label,
				const graphics::Font font,
				const ColorF* p_col_bg_,
				const ColorF* p_col_control_,
				const ColorF* p_col_label,
				const WindowRectangle& rect,
				const std::string& tooltip_label = "",
				RoundEdgeFlag round_edge_flag_ = RoundEdgeFlag::None,
				float round_radius_ = 0.f,
				float hover_highlight = 0.04f
			);
		};


		// ボタン(アイコン)
		class IconButton : public Button {
		public:
			enum class SourceType {
				File,
				Resource
			};

			IconButton() :
				icon(nullptr),
				icon_src(),
				src_type(SourceType::File),
				icon_resource_num(0)
			{}

			// ボタンを作成(アイコン)
			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const char* icon_src_,
				SourceType src_type_,
				const ColorF* p_col_bg_,
				const ColorF* p_col_control_,
				const WindowRectangle& rect,
				const std::string& tooltip_label = "",
				RoundEdgeFlag round_edge_flag_ = RoundEdgeFlag::None,
				float round_radius_ = 0.f,
				float hover_highlight = 0.04f
			);

		protected:
			graphics::Bitmap* icon;
			SourceType src_type;
			std::string icon_src;
			uint16_t icon_resource_num;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;
		};
	}
}