#pragma once

#include "button.hpp"



namespace mkaul {
	namespace ui {
		// ボタン(アイコン)
		class IconButton : public Button {
		protected:
			std::unique_ptr<graphics::Bitmap> icon_;

			virtual LRESULT wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) override;

		public:
			IconButton() :
				icon_(nullptr)
			{}

			void set_icon_from_filename(const std::filesystem::path& path);
			void set_icon_from_resource(HINSTANCE hinst, const char* res_name, const char* res_type);
		};
	}
}