//-------------------------------------------------------------------------------- -
//		MKAul (Button)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "control.hpp"



namespace mkaul {
	namespace window {
		// ボタン
		class Button : public Control {
		public:
			Button() :
				hwnd_tooltip(NULL),
				tool_info({ 0 }),
				tooltip_label(),
				hover_highlight(0.f),
				hovered(false),
				clicked(false)
			{}

			virtual bool create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const Color_F* p_col_bg_,
				const Color_F* p_col_control_,
				const Window_Rectangle& rect,
				const std::string& tooltip_label_ = NULL,
				Round_Edge_Flag round_edge_flag_ = Round_Edge_Flag::None,
				float round_radius_ = 0.f,
				float hover_highlight_ = 0.04f
			);

		protected:
			static constexpr int HOVER_HIGHLIGHT = 10;

			HWND                    hwnd_tooltip;
			TOOLINFO	    		tool_info;
			std::string				tooltip_label;
			float					hover_highlight;
			bool			    	hovered, clicked;

			virtual LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		};


		// ボタン(ラベル)
		class Button_Label : public Button {
		public:

			Button_Label() :
				label(),
				p_col_label(nullptr)
			{}

			// ボタンを作成
			virtual bool create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const std::string& label_,
				const Color_F* p_col_bg_,
				const Color_F* p_col_control_,
				const Color_F* p_col_label,
				const Window_Rectangle& rect,
				const std::string& tooltip_label_ = NULL,
				Round_Edge_Flag round_edge_flag_ = Round_Edge_Flag::None,
				float round_radius_ = 0.f,
				float hover_highlight_ = 0.04f
			);

		protected:
			std::string				label;
			Color_F*				p_col_label;

			virtual LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		};


		// ボタン(アイコン)
		class Button_Icon : public Button {
		public:
			enum class Source_Type {
				File,
				Resource
			};

			Button_Icon() :
				icon(nullptr),
				icon_src(),
				src_type(Source_Type::File),
				icon_resource_num(0)
			{}

			// ボタンを作成(アイコン)
			virtual bool create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const char* icon_src_,
				Source_Type src_type_,
				const Color_F* p_col_bg_,
				const Color_F* p_col_control_,
				const Window_Rectangle& rect,
				const std::string& tooltip_label_ = NULL,
				Round_Edge_Flag round_edge_flag_ = Round_Edge_Flag::None,
				float round_radius_ = 0.f,
				float hover_highlight_ = 0.04f
			);

		protected:
			graphics::Bitmap* icon;
			Source_Type src_type;
			std::string icon_src;
			uint16_t icon_resource_num;

			virtual LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		};
	}
}