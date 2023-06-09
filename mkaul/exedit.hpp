//----------------------------------------------------------------------------------
//		MKAul (ExEdit)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include <aviutl.hpp>
#include <exedit.hpp>



namespace mkaul {
	namespace exedit {
		class Internal {
		private:
			AviUtl::FilterPlugin* fp_;

			HWND* p_hwnd_timeline_;
			HWND* p_hwnd_objdialog_;
			BOOL(*wndproc_timeline_)(HWND, UINT, WPARAM, LPARAM, AviUtl::EditHandle*, AviUtl::FilterPlugin*);
			WNDPROC wndproc_objdialog_orig_;
			WNDPROC wndproc_objdialog_;
			ExEdit::Object** p_obj_table_;
			ExEdit::Filter** p_filter_table_;
			int32_t* p_current_scene_idx_;
			int32_t* p_current_obj_idx_;
			int32_t* p_current_filter_idx_;
			int32_t* p_obj_num_;
			int32_t* p_current_scene_obj_num_;
			ExEdit::Object** p_obj_;
			uint8_t** p_obj_exdata_;
			int32_t* array_next_obj_idcs_;
			int32_t* array_selected_obj_idcs_;
			int32_t* p_selected_obj_num_;
			ExEdit::SceneSetting* p_scene_setting_;
			AviUtl::EditHandle* editp_;
			int32_t* p_layer_width_;
			int32_t* p_layer_height_;
			int32_t* p_visible_layer_num_;
			ExEdit::LayerSetting** array_layer_setting_;
			int32_t* p_aviutl_final_frame_;
			int32_t* p_exedit_final_frame_;
			int32_t* p_exedit_current_frame_;

		public:
			Internal() :
				fp_(nullptr),
				p_hwnd_timeline_(nullptr),
				p_hwnd_objdialog_(nullptr),
				wndproc_objdialog_orig_(nullptr),
				wndproc_objdialog_(nullptr),
				p_obj_table_(nullptr),
				p_filter_table_(nullptr),
				p_current_scene_idx_(nullptr),
				p_current_obj_idx_(nullptr),
				p_current_filter_idx_(nullptr),
				p_obj_num_(nullptr),
				p_current_scene_obj_num_(nullptr),
				p_obj_(nullptr),
				p_obj_exdata_(nullptr),
				array_next_obj_idcs_(nullptr),
				array_selected_obj_idcs_(nullptr),
				p_selected_obj_num_(nullptr),
				p_scene_setting_(nullptr),
				editp_(nullptr),
				p_layer_width_(nullptr),
				p_layer_height_(nullptr),
				p_visible_layer_num_(nullptr),
				array_layer_setting_(nullptr),
				p_aviutl_final_frame_(nullptr),
				p_exedit_final_frame_(nullptr),
				p_exedit_current_frame_(nullptr)
			{}

			bool init(AviUtl::FilterPlugin* fp);

			auto base_address() const noexcept
			{
				if (fp_) return (uint32_t)fp_->dll_hinst;
				else return 0x0u;
			}
			auto fp() const noexcept { return fp_; }
			auto p_hwnd_timeline() const noexcept { return p_hwnd_timeline_; }
			auto p_hwnd_objdialog() const noexcept { return p_hwnd_objdialog_; }
			auto get_wndproc_timeline() const noexcept { return wndproc_timeline_; }
			auto get_wndproc_objdialog_orig() const noexcept { return wndproc_objdialog_orig_; }
			auto get_wndproc_objdialog() const noexcept { return wndproc_objdialog_; }
			auto p_obj_table() const noexcept { return p_obj_table_; }
			auto p_filter_table() const noexcept { return p_filter_table_; }
			auto p_current_scene_idx() const noexcept { return p_current_scene_idx_; }
			auto p_current_obj_idx() const noexcept { return p_current_obj_idx_; }
			auto p_current_filter_idx() const noexcept { return p_current_filter_idx_; }
			auto p_obj_num() const noexcept { return p_obj_num_; }
			auto p_current_scene_obj_num() const noexcept { return p_current_scene_obj_num_; }
			auto p_obj() const noexcept { return p_obj_; }
			auto p_obj_exdata() const noexcept { return p_obj_exdata_; }
			auto array_next_obj_idcs() const noexcept { return array_next_obj_idcs_; }
			auto array_selected_obj_idcs() const noexcept { return array_selected_obj_idcs_; }
			auto p_selected_obj_num() const noexcept { return p_selected_obj_num_; }
			auto p_scene_setting() const noexcept { return p_scene_setting_; }
			auto editp() const noexcept { return editp_; }
			auto p_layer_width() const noexcept { return p_layer_width_; }
			auto p_layer_height() const noexcept { return p_layer_height_; }
			auto p_visible_layer_num() const noexcept { return p_visible_layer_num_; }
			auto array_layer_setting() const noexcept { return array_layer_setting_; }
			auto p_aviutl_final_frame() const noexcept { return p_aviutl_final_frame_; }
			auto p_exedit_final_frame() const noexcept { return p_exedit_final_frame_; }
			auto p_exedit_current_frame() const noexcept { return p_exedit_current_frame_; }
		};
	}
}