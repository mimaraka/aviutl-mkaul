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
		// �g���ҏW(ver. 0.92)�̃t�B���^�I�u�W�F�N�g�̃|�C���^���擾
		inline AviUtl::FilterPlugin* get_exedit(AviUtl::FilterPlugin* fp)
		{
			AviUtl::SysInfo si;
			fp->exfunc->get_sys_info(NULL, &si);

			for (int i = 0; i < si.filter_n; i++) {
				auto tfp = (AviUtl::FilterPlugin*)fp->exfunc->get_filterp(i);
				if (
					tfp->dll_hinst == ::GetModuleHandle("exedit.auf")
					&& !::strcmp(tfp->name, "�g���ҏW")
					&& !::strcmp(tfp->information, "�g���ҏW(exedit) version 0.92 by �j�d�m����")
				)
					return tfp;
			}
			return nullptr;
		}


		// �g���ҏW(�^�C�����C��)�̃E�B���h�E�n���h�����擾
		inline HWND get_exedit_wnd_timeline(AviUtl::FilterPlugin* fp)
		{
			auto fp_exedit = get_exedit(fp);
			return fp_exedit ? fp_exedit->hwnd : NULL;
		}


		// �g���ҏW(�I�u�W�F�N�g�ݒ�_�C�A���O)�̃E�B���h�E�n���h�����擾
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