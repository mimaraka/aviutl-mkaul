#include "ui_list.hpp"



namespace mkaul {
	namespace ui {
		bool List::insert(size_t index, const Component* child) {
			if (children_.size() < index) return false;
			children_.insert(children_.begin() + index, const_cast<Component*>(child));
			return true;
		}

		bool List::replace(size_t index, const Component* child) {
			if (children_.size() <= index) return false;
			children_[index] = const_cast<Component*>(child);
			return true;
		}

		bool List::redraw() const noexcept {
			for (auto child : children_) {
				if (!child->redraw()) {
					return false;
				}
			}
			return true;
		}

		bool List::show() const noexcept {
			bool result = true;
			for (auto child : children_) {
				result &= child->show();
			}
			return result;
		}

		bool List::hide() const noexcept {
			bool result = true;
			for (auto child : children_) {
				result &= child->hide();
			}
			return result;
		}

		bool Row::move(const WindowRectangle& rect) const noexcept {
			const float width = rect.get_width() / (float)children_.size();
			for (int i = 0; i < (int)children_.size(); i++) {
				int left = rect.left + (int)(i * width);
				int right = rect.left + (int)((i + 1) * width);
				if (!children_[i]->move(WindowRectangle{ left, rect.top, right, rect.bottom })) {
					return false;
				}
			}
			return true;
		}

		bool Column::move(const WindowRectangle& rect) const noexcept {
			const float height = rect.get_height() / (float)children_.size();
			for (int i = 0; i < (int)children_.size(); i++) {
				int top = rect.top + (int)(i * height);
				int bottom = rect.top + (int)((i + 1) * height);
				if (!children_[i]->move(WindowRectangle{ rect.left, top, rect.right, bottom })) {
					return false;
				}
			}
			return true;
		}
	}
}