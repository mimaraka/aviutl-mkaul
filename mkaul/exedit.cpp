#include "exedit.hpp"
#include "aviutl.hpp"



namespace mkaul {
	namespace exedit {
		bool Internal::init(AviUtl::FilterPlugin* fp) noexcept {
			auto exedit_fp = aviutl::get_fp_by_name(fp, "exedit.auf");
			if (exedit_fp) {
				fp_ = exedit_fp;
				auto exedit_base = (uint32_t)exedit_fp->dll_hinst;

				p_hwnd_aviutl_ = reinterpret_cast<HWND*>(exedit_base + 0x135c6c);
				p_hwnd_timeline_ = reinterpret_cast<HWND*>(exedit_base + 0x177a44);
				p_hwnd_objdialog_ = reinterpret_cast<HWND*>(exedit_base + 0x1539c8);
				wndproc_objdialog_orig_ = reinterpret_cast<WNDPROC>(exedit_base + 0x02cde0);
				p_wndproc_objdialog_ = reinterpret_cast<WNDPROC*>(exedit_base + 0x02e804);
				wndproc_objdialog_ = *p_wndproc_objdialog_;
				p_obj_table_ = reinterpret_cast<ExEdit::Object**>(exedit_base + 0x168fa8);
				p_filter_table_ = reinterpret_cast<ExEdit::Filter**>(exedit_base + 0x187c98);
				p_current_scene_idx_ = reinterpret_cast<int32_t*>(exedit_base + 0x1a5310);
				p_current_obj_idx_ = reinterpret_cast<int32_t*>(exedit_base + 0x177a10);
				p_current_filter_idx_ = reinterpret_cast<int32_t*>(exedit_base + 0x14965c);
				p_obj_num_ = reinterpret_cast<int32_t*>(exedit_base + 0x146250);
				p_current_scene_obj_num_ = reinterpret_cast<int32_t*>(exedit_base + 0x15918c);
				p_array_obj_ = reinterpret_cast<ExEdit::Object**>(exedit_base + 0x1e0fa4);
				p_obj_exdata_ = reinterpret_cast<uint8_t**>(exedit_base + 0x1e0fa8);
				array_next_obj_idcs_ = reinterpret_cast<int32_t*>(exedit_base + 0x1592d8);
				array_selected_obj_idcs_ = reinterpret_cast<int32_t*>(exedit_base + 0x179230);
				p_selected_obj_num_ = reinterpret_cast<int32_t*>(exedit_base + 0x167d88);
				p_scene_setting_ = reinterpret_cast<ExEdit::SceneSetting*>(exedit_base + 0x177a50);
				editp_ = reinterpret_cast<AviUtl::EditHandle*>(exedit_base + 0x1a532c);
				p_layer_width_ = reinterpret_cast<int32_t*>(exedit_base + 0x1a52fc);
				p_layer_height_ = reinterpret_cast<int32_t*>(exedit_base + 0x0a3e20);
				p_visible_layer_num_ = reinterpret_cast<int32_t*>(exedit_base + 0x0a3fbc);
				array_layer_setting_ = reinterpret_cast<ExEdit::LayerSetting**>(exedit_base + 0x0a4058);
				p_aviutl_final_frame_ = reinterpret_cast<int32_t*>(exedit_base + 0x14d3a0);
				p_exedit_final_frame_ = reinterpret_cast<int32_t*>(exedit_base + 0x1a5318);
				p_exedit_current_frame_ = reinterpret_cast<int32_t*>(exedit_base + 0x1a5304);

				return true;
			}

			return false;
		}
	}
}