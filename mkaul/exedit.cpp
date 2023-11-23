#include "exedit.hpp"



namespace mkaul {
	namespace exedit {
		bool Internal::init(AviUtl::FilterPlugin* fp)
		{
			AviUtl::SysInfo si;
			fp->exfunc->get_sys_info(nullptr, &si);

			for (int i = 0; i < si.filter_n; i++) {
				auto tfp = (AviUtl::FilterPlugin*)fp->exfunc->get_filterp(i);
				if (tfp->dll_hinst == ::GetModuleHandleA("exedit.auf")) {
					fp_ = tfp;
					auto exedit_base = (uint32_t)tfp->dll_hinst;

					p_hwnd_timeline_ = (HWND*)(exedit_base + 0x177a44);
					p_hwnd_objdialog_ = (HWND*)(exedit_base + 0x1539c8);
					wndproc_objdialog_orig_ = (WNDPROC)(exedit_base + 0x02cde0);
					p_wndproc_objdialog_ = (WNDPROC*)(exedit_base + 0x02e804);
					wndproc_objdialog_ = *p_wndproc_objdialog_;
					p_obj_table_ = (ExEdit::Object**)(exedit_base + 0x168fa8);
					p_filter_table_ = (ExEdit::Filter**)(exedit_base + 0x187c98);
					p_current_scene_idx_ = (int32_t*)(exedit_base + 0x1a5310);
					p_current_obj_idx_ = (int32_t*)(exedit_base + 0x177a10);
					p_current_filter_idx_ = (int32_t*)(exedit_base + 0x14965c);
					p_obj_num_ = (int32_t*)(exedit_base + 0x146250);
					p_current_scene_obj_num_ = (int32_t*)(exedit_base + 0x15918c);
					p_obj_ = (ExEdit::Object**)(exedit_base + 0x1e0fa4);
					p_obj_exdata_ = (uint8_t**)(exedit_base + 0x1e0fa8);
					array_next_obj_idcs_ = (int32_t*)(exedit_base + 0x1592d8);
					array_selected_obj_idcs_ = (int32_t*)(exedit_base + 0x179230);
					p_selected_obj_num_ = (int32_t*)(exedit_base + 0x167d88);
					p_scene_setting_ = (ExEdit::SceneSetting*)(exedit_base + 0x177a50);
					editp_ = (AviUtl::EditHandle*)(exedit_base + 0x1a532c);
					p_layer_width_ = (int32_t*)(exedit_base + 0x1a52fc);
					p_layer_height_ = (int32_t*)(exedit_base + 0x0a3e20);
					p_visible_layer_num_ = (int32_t*)(exedit_base + 0x0a3fbc);
					array_layer_setting_ = (ExEdit::LayerSetting**)(exedit_base + 0x0a4058);
					p_aviutl_final_frame_ = (int32_t*)(exedit_base + 0x14d3a0);
					p_exedit_final_frame_ = (int32_t*)(exedit_base + 0x1a5318);
					p_exedit_current_frame_ = (int32_t*)(exedit_base + 0x1a5304);

					return true;
				}
			}
			return false;
		}
	}
}