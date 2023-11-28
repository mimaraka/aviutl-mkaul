#pragma once

#include "graphics_gdiplus.hpp"
#include "graphics_directx.hpp"



namespace mkaul {
	namespace graphics {
		class Manager final {
		public:
			// 描画方式
			enum class GraphicMethod {
				Null,
				Gdiplus,
				Directx
			};

			Manager() = delete;
			~Manager() = delete;

			// 描画環境の用意
			static bool startup(GraphicMethod method_ = GraphicMethod::Gdiplus);
			// 描画環境の破棄
			static bool shutdown();

			static auto get_method() { return method; }

			// グラフィックス作成
			static bool create_graphics(Graphics** pp_graphics);

			// ジオメトリ作成
			static bool create_path(Path** pp_path);

		private:
			inline static GraphicMethod method = GraphicMethod::Null;
		};
	}
}