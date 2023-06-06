//----------------------------------------------------------------------------------
//		MKAul (ExEdit)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include <aviutl.hpp>



namespace mkaul {
	namespace exedit {
		// 拡張編集(ver. 0.92)のフィルタオブジェクトのポインタを取得
		inline AviUtl::FilterPlugin* get_exedit(AviUtl::FilterPlugin* fp)
		{
			AviUtl::SysInfo si;
			fp->exfunc->get_sys_info(NULL, &si);

			for (int i = 0; i < si.filter_n; i++) {
				auto tfp = (AviUtl::FilterPlugin*)fp->exfunc->get_filterp(i);
				if (
					tfp->dll_hinst == ::GetModuleHandle("exedit.auf")
					&& !::strcmp(tfp->name, "拡張編集")
					&& !::strcmp(tfp->information, "拡張編集(exedit) version 0.92 by ＫＥＮくん")
				)
					return tfp;
			}
			return nullptr;
		}


		// 拡張編集(タイムライン)のウィンドウハンドルを取得
		inline HWND get_exedit_wnd_timeline(AviUtl::FilterPlugin* fp)
		{
			auto fp_exedit = get_exedit(fp);
			return fp_exedit ? fp_exedit->hwnd : NULL;
		}


		// 拡張編集(オブジェクト設定ダイアログ)のウィンドウハンドルを取得
		inline HWND get_exedit_wnd_objdialog(AviUtl::FilterPlugin* fp)
		{
			struct local {
				const HWND timeline;
				HWND objdlg;

				static BOOL CALLBACK enumproc(HWND hwnd, LPARAM lp)
				{
					auto p = (local*)lp;
					HWND owner = ::GetWindow(hwnd, GW_OWNER);
					if (owner == p->timeline) {
						p->objdlg = hwnd;
						return FALSE;
					}
					else return TRUE;
				}
			} l = { get_exedit_wnd_timeline(fp), NULL};

			DWORD id = ::GetCurrentThreadId();
			::EnumThreadWindows(id, local::enumproc, (LPARAM)&l);

			return l.objdlg;
		}
	}
}