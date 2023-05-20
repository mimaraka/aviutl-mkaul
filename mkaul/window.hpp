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
		// �q�E�B���h�E���ċA�I�Ɍ���
		void get_all_children(HWND hwnd, std::vector<HWND>* hwnd_list);


		// �E�B���h�E�N���X
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


		// �E�B���h�E���ړ�
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

		// �ĕ`��
		inline bool Window::redraw() const
		{
			if (hwnd) {
				std::vector<HWND> hwnd_list;
				get_all_children(hwnd, &hwnd_list);
				// ���g���ĕ`��
				::InvalidateRect(hwnd, NULL, FALSE);

				// ���w�̃E�B���h�E��S�čĕ`��
				for (auto hw : hwnd_list)
					::InvalidateRect(hw, NULL, FALSE);
				
				return true;
			}
			else return false;
		}


		// �E�B���h�E��\��
		inline bool Window::show() const
		{
			return ::ShowWindow(hwnd, SW_SHOW);
		}

		// �E�B���h�E���\��
		inline bool Window::hide() const
		{
			return ::ShowWindow(hwnd, SW_HIDE);
		}

		// �E�B���h�E�����
		inline bool Window::close() const
		{
			return ::DestroyWindow(hwnd);
		}
	}
}