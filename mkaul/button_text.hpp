#pragma once

#include "button.hpp"



namespace mkaul::ui {
	// ボタン(文字)
	class TextButton : public Button {
	protected:
		std::string text_;
		graphics::Font font_;
		ColorF* p_color_text_;

		virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;

	public:

		TextButton() :
			text_(),
			font_(),
			p_color_text_(nullptr)
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

		auto get_text() const noexcept { return text_; }
		void set_text(const std::string& label) noexcept;
	};
}