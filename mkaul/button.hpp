#pragma once

#include "control.hpp"
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")



namespace mkaul {
	namespace ui {
		// ボタン
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
				const std::string& tooltip_label = "",
				const WindowRectangle& rect = WindowRectangle{},
				const WindowRectangle& padding = WindowRectangle{},
				flag::RoundEdge round_edge = flag::RoundEdge::None,
				float round_radius = 0.f,
				float hover_highlight = 0.05f
			) noexcept;
		};
	}
}