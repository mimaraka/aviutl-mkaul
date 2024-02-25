#pragma once

#include "rectangle.hpp"



namespace mkaul {
	namespace ui {
		// コンポーネント (抽象クラス)
		class Component {
		public:
			virtual bool move(const WindowRectangle& rect) const noexcept = 0;
			virtual bool redraw() const noexcept = 0;
			virtual bool show() const noexcept = 0;
			virtual bool hide() const noexcept = 0;
		};
	}
}