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
				hovered(false),
				clicked(false)
			{}

			virtual bool create(
				HWND hwnd_parent_,
				int id_,
				const Color_F* p_col_bg_,
				const Color_F* p_col_control_,
				const Rectangle<LONG>& rect,
				const std::string& tooltip_label_ = NULL,
				int round_edge_flag_ = ROUND_EDGE_ALL,
				float round_radius_ = 0.f
			);

		protected:
			static constexpr int HOVER_HIGHLIGHT = 10;

			HWND                    hwnd_tooltip;
			TOOLINFO	    		tool_info;
			std::string				tooltip_label;
			bool			    	hovered, clicked;

			virtual LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		};


		// ボタン(ラベル)
		class Button_Label : public Button {
		public:

			Button_Label()
			{}

			// ボタンを作成
			virtual bool create(
				HWND hwnd_parent_,
				int id_,
				const std::string& label_,
				const Color_F* p_col_bg_,
				const Color_F* p_col_control_,
				const Color_F* p_col_label,
				const Rectangle<LONG>& rect,
				const std::string& tooltip_label_ = NULL,
				int round_edge_flag_ = ROUND_EDGE_ALL,
				float round_radius_ = 0.f
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
				Filename,
				Resource
			};

			Button_Icon() :
				icon(nullptr),
				icon_source(),
				icon_resource_num(0)
			{}

			// ボタンを作成(アイコン)
			virtual bool create(
				HWND hwnd_parent_,
				int id_,
				const char* icon_source_,
				Source_Type source_type_,
				const Color_F* p_col_bg_,
				const Color_F* p_col_control_,
				const Rectangle<LONG>& rect,
				const std::string& tooltip_label_ = NULL,
				BYTE round_edge_flag_ = ROUND_EDGE_ALL,
				float round_radius_ = 0.f
			);

		protected:
			graphics::Bitmap* icon;
			Source_Type source_type;
			std::string icon_source;
			uint16_t icon_resource_num;

			virtual LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		};
	}
}