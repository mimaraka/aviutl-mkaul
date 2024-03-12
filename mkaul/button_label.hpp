#pragma once

#include "button.hpp"



namespace mkaul {
	namespace ui {
		// ボタン(ラベル)
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

			// ボタンを作成
			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent_,
				int id_,
				const std::string& label,
				const graphics::Font font,
				const ColorF* p_color_bg,
				const ColorF* p_color_control,
				const ColorF* p_color_label,
				const std::string& tooltip_label = "",
				const WindowRectangle& rect = WindowRectangle{},
				const WindowRectangle& padding = WindowRectangle{},
				flag::RoundEdge round_edge = flag::RoundEdge::None,
				float round_radius = 0.f,
				float hover_highlight = 0.05f
			) noexcept;

			auto get_label() const noexcept { return label_; }
			void set_label(const std::string& label) noexcept;
		};
	}
}