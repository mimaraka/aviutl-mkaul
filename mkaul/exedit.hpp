#pragma once

#include <aviutl.hpp>
#include <bit>
#include <exedit.hpp>



namespace mkaul {
	namespace exedit {
		class Internal {
			AviUtl::FilterPlugin* fp_ = nullptr;

			HWND* p_hwnd_aviutl_ = nullptr;
			HWND* p_hwnd_timeline_ = nullptr;
			HWND* p_hwnd_objdialog_ = nullptr;
			BOOL(*wndproc_timeline_)(HWND, UINT, WPARAM, LPARAM, AviUtl::EditHandle*, AviUtl::FilterPlugin*) = nullptr;
			WNDPROC wndproc_objdialog_orig_ = nullptr;
			WNDPROC wndproc_objdialog_ = nullptr;
			WNDPROC* p_wndproc_objdialog_ = nullptr;
			ExEdit::Object** p_obj_table_ = nullptr;
			ExEdit::Filter** p_filter_table_ = nullptr;
			int32_t* p_current_scene_idx_ = nullptr;
			int32_t* p_current_obj_idx_ = nullptr;
			int32_t* p_current_filter_idx_ = nullptr;
			int32_t* p_obj_num_ = nullptr;
			int32_t* p_current_scene_obj_num_ = nullptr;
			ExEdit::Object** p_array_obj_ = nullptr;
			uint8_t** p_obj_exdata_ = nullptr;
			int32_t* array_next_obj_idcs_ = nullptr;
			int32_t* array_selected_obj_idcs_ = nullptr;
			int32_t* p_selected_obj_num_ = nullptr;
			ExEdit::SceneSetting* p_scene_setting_ = nullptr;
			AviUtl::EditHandle* editp_ = nullptr;
			int32_t* p_layer_width_ = nullptr;
			int32_t* p_layer_height_ = nullptr;
			int32_t* p_visible_layer_num_ = nullptr;
			ExEdit::LayerSetting** array_layer_setting_ = nullptr;
			int32_t* p_aviutl_final_frame_ = nullptr;
			int32_t* p_exedit_final_frame_ = nullptr;
			int32_t* p_exedit_current_frame_ = nullptr;

		public:
			Internal() {}

			bool init(AviUtl::FilterPlugin* fp) noexcept;

			auto base_address() const noexcept {
				if (fp_) {
					return (uint32_t)fp_->dll_hinst;
				}
				else return 0u;
			}

			template <typename T>
			T* get(uint32_t offset_address) const noexcept {
				if (fp_) {
					return std::bit_cast<T*>(base_address() + offset_address);
				}
				else return nullptr;
			}

			template <typename Ret, typename...Args>
			Ret call_cdecl(uint32_t offset_address, Args...args) const noexcept {
				if (fp_) {
					auto func = std::bit_cast<Ret(__cdecl*)(Args...)>(base_address() + offset_address);
					return func(args...);
				}
				else return Ret{};
			}

			template <typename Ret, typename...Args>
			Ret call_stdcall(uint32_t offset_address, Args...args) const noexcept {
				if (fp_) {
					auto func = std::bit_cast<Ret(__stdcall*)(Args...)>(base_address() + offset_address);
					return func(args...);
				}
				else return Ret{};
			}

			auto fp() const noexcept { return fp_; }
			auto p_hwnd_aviutl() const noexcept { return p_hwnd_aviutl_; }
			auto p_hwnd_timeline() const noexcept { return p_hwnd_timeline_; }
			auto p_hwnd_objdialog() const noexcept { return p_hwnd_objdialog_; }
			auto get_wndproc_timeline() const noexcept { return wndproc_timeline_; }
			auto get_wndproc_objdialog_orig() const noexcept { return wndproc_objdialog_orig_; }
			auto get_wndproc_objdialog() const noexcept { return wndproc_objdialog_; }
			auto p_wndproc_objdialog() const noexcept { return p_wndproc_objdialog_; }
			auto p_obj_table() const noexcept { return p_obj_table_; }
			auto p_filter_table() const noexcept { return p_filter_table_; }
			auto p_current_scene_idx() const noexcept { return p_current_scene_idx_; }
			auto p_current_obj_idx() const noexcept { return p_current_obj_idx_; }
			auto p_current_filter_idx() const noexcept { return p_current_filter_idx_; }
			auto p_obj_num() const noexcept { return p_obj_num_; }
			auto p_current_scene_obj_num() const noexcept { return p_current_scene_obj_num_; }
			auto p_array_obj() const noexcept { return p_array_obj_; }
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