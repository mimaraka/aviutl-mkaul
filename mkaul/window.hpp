//---------------------------------------------------------------------------------------
//		MKAul (Window)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <commctrl.h>
#include "common.hpp"

#pragma comment(lib, "comctl32.lib")



namespace mkaul {
	namespace window {
		class Window {
		public:
			static constexpr int WM_REDRAW = WM_APP;
			HWND hwnd;

			Window() :
				hwnd(NULL)
			{}

			virtual BOOL	    create(
				HWND		hwnd_parent,
				LPCTSTR     window_name,
				LPCTSTR		class_name,
				WNDPROC		wndproc_,
				LONG		window_style,
				LONG		class_style,
				HCURSOR     cursor,
				const RECT& rect,
				LPVOID		lp_param
			);

			bool        		move(const RECT& rect) const;
			bool				redraw() const;
			bool				close() const;
			bool				show() const;
			bool				hide() const;
		};
	}
}