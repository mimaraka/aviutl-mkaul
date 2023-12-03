#pragma once

#include "graphics_gdiplus.hpp"
#include "graphics_directx.hpp"



namespace mkaul {
	namespace graphics {
		class Factory final {
		public:
			// 描画方式
			enum class GraphicMethod {
				Null,
				Gdiplus,
				Directx
			};

			Factory() = delete;
			~Factory() = delete;

			// 描画環境の用意
			static bool startup(GraphicMethod method = GraphicMethod::Gdiplus) noexcept;
			// 描画環境の破棄
			static bool shutdown() noexcept;

			static auto get_method() noexcept { return method_; }

			// グラフィックス作成
			static std::unique_ptr<Graphics> create_graphics() noexcept;

			// パス作成
			static std::unique_ptr<Path> create_path() noexcept;

		private:
			inline static GraphicMethod method_ = GraphicMethod::Null;
		};
	}
}