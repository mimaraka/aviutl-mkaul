#pragma once

#include "window.hpp"
#include "graphics.hpp"
#include "flag.hpp"



namespace mkaul {
	namespace flag {
		// ステータス
		enum class Status : uint32_t {
			Null = 0u,
			Disabled = 1u << 0
		};

		// どの角を丸くするかのフラグ
		enum class RoundEdge : uint32_t {
			None = 0u,
			LeftTop = 1u << 0,
			LeftBottom = 1u << 1,
			RightTop = 1u << 2,
			RightBottom = 1u << 3,
			All = 0b1111
		};
	}

	namespace ui {
		//---------------------------------------------------------------------
		//		コントロール
		//---------------------------------------------------------------------
		class Control : public Window {
		protected:
			// 色
			ColorF* p_color_bg_ = nullptr;
			ColorF* p_color_control_ = nullptr;
			// ステータス
			flag::Status status_ = flag::Status::Null;
			TRACKMOUSEEVENT tme_ = { 0 };
			HWND hwnd_parent_ = NULL;
			std::unique_ptr<graphics::Graphics> p_graphics_;
			flag::RoundEdge round_edge_ = flag::RoundEdge::None;
			// コントロールID
			int id_ = 0;
			float round_radius_ = 0.f;

			static LRESULT CALLBACK	wndproc_static(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam);
			virtual LRESULT			wndproc(HWND hwnd_, UINT message, WPARAM wparam, LPARAM lparam) = 0;

		public:
			Control() {}
			~Control() {}

			// コントロールを作成
			virtual HWND create(
				HINSTANCE hinst,
				HWND hwnd_parent,
				int id,
				LPCTSTR class_name,
				LONG window_style,
				LONG class_style,
				const ColorF* p_color_bg,
				const ColorF* p_color_control,
				const WindowRectangle& rect = WindowRectangle{},
				const WindowRectangle& padding = WindowRectangle{},
				flag::RoundEdge round_edge = flag::RoundEdge::None,
				float round_radius = 0.f,
				HCURSOR cursor = ::LoadCursorA(NULL, IDC_ARROW)
			) noexcept;
			auto get_id() const noexcept { return id_; }
			auto get_status() const noexcept { return status_; }
			// ステータスを設定
			void set_status(flag::Status status) noexcept;
			// ステータスを追加
			void add_status(flag::Status status) noexcept;
			// ラウンドエッジを描画
			void draw_round_edge() noexcept;
		};
	}
}