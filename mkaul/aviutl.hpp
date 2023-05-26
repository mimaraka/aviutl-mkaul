//----------------------------------------------------------------------------------
//		MKAul (AviUtl)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include <aviutl_plugin_sdk/filter.h>



namespace mkaul {
	namespace aviutl {
		// �g���ҏW�̃t�B���^�I�u�W�F�N�g�̃|�C���^���擾
		inline FILTER* get_exedit(FILTER* fp)
		{
            SYS_INFO si;
            fp->exfunc->get_sys_info(NULL, &si);

            for (int i = 0; i < si.filter_n; i++) {
                auto tfp = (FILTER*)fp->exfunc->get_filterp(i);
				if (tfp->dll_hinst == ::GetModuleHandle("exedit.auf"))
					return tfp;
            }
            return nullptr;
		}


        // �g���ҏW(�^�C�����C��)�̃E�B���h�E�n���h�����擾
        inline HWND get_exedit_timeline_wnd(FILTER* fp)
        {
            auto fp_exedit = get_exedit(fp);
            return fp_exedit ? fp_exedit->hwnd : NULL;
        }


        // �g���ҏW(�I�u�W�F�N�g�ݒ�_�C�A���O)�̃E�B���h�E�n���h�����擾
        inline HWND get_exedit_objdialog_wnd(FILTER* fp)
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
			} l = { get_exedit_timeline_wnd(fp), NULL};

			DWORD id = ::GetCurrentThreadId();
			::EnumThreadWindows(id, local::enumproc, (LPARAM)&l);

			return l.objdlg;
        }
	}
}