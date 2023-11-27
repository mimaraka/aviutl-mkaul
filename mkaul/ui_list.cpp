#include "ui_list.hpp"



namespace mkaul {
	namespace ui {
		bool Row::move(const WindowRectangle& rect) const noexcept
		{
			const int width = rect.get_width() / (int)children_.size();
			for (int i = 0; i < (int)children_.size(); i++) {
				int left = rect.left + i * width;
				int right = (i == (int)children_.size() - 1) ? rect.right : left + width;
				if (!children_[i]->move(WindowRectangle{ left, rect.top, right, rect.bottom })) {
					return false;
				}
			}
			return true;
		}

		bool Column::move(const WindowRectangle& rect) const noexcept
		{
			const int height = rect.get_height() / (int)children_.size();
			for (int i = 0; i < (int)children_.size(); i++) {
				int top = rect.top + i * height;
				int bottom = (i == (int)children_.size() - 1) ? rect.bottom : top + height;
				if (!children_[i]->move(WindowRectangle{ rect.left, top, rect.right, bottom })) {
					return false;
				}
			}
			return true;
		}
	}
}