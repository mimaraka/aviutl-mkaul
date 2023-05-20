//---------------------------------------------------------------------------------------
//		MKAul (Window)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include <commctrl.h>
#include "rectangle.hpp"
#include "utils.hpp"

#pragma comment(lib, "comctl32.lib")



namespace mkaul {
	namespace window {
		// 子ウィンドウを再帰的に検索
		void get_all_children(HWND hwnd, std::vector<HWND>* hwnd_list);


		// ウィンドウクラス
		class Window {
		public:
			HWND hwnd;

			Window() :
				hwnd(NULL)
			{}

			virtual bool create(
				HINSTANCE				hinst,
				HWND					hwnd_parent,
				const std::string&		window_name,
				const std::string&		class_name,
				WNDPROC					wndproc_,
				LONG					window_style,
				LONG					class_style,
				const Rectangle<LONG>&	rect,
				HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW),
				LPVOID					lp_param = nullptr
			);

			bool move(const Rectangle<LONG>& rect) const;
			bool redraw() const;
			bool close() const;
			bool show() const;
			bool hide() const;
		};


		// ウィンドウを移動
		inline bool Window::move(const Rectangle<LONG>& rect) const
		{
			return ::MoveWindow(
				hwnd,
				(std::min)(rect.left, rect.right),
				(std::min)(rect.top, rect.bottom),
				std::abs(rect.right - rect.left),
				std::abs(rect.bottom - rect.top),
				TRUE
			);
		}

		// 再描画
		inline bool Window::redraw() const
		{
			if (hwnd) {
				std::vector<HWND> hwnd_list;
				get_all_children(hwnd, &hwnd_list);
				// 自身を再描画
				::InvalidateRect(hwnd, NULL, FALSE);

				// 下層のウィンドウを全て再描画
				for (auto hw : hwnd_list)
					::InvalidateRect(hw, NULL, FALSE);
				
				return true;
			}
			else return false;
		}


		// ウィンドウを表示
		inline bool Window::show() const
		{
			return ::ShowWindow(hwnd, SW_SHOW);
		}

		// ウィンドウを非表示
		inline bool Window::hide() const
		{
			return ::ShowWindow(hwnd, SW_HIDE);
		}

		// ウィンドウを閉じる
		inline bool Window::close() const
		{
			return ::DestroyWindow(hwnd);
		}
	}
}