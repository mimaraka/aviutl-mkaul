#pragma once

#include "ui_component.hpp"
#include <vector>



namespace mkaul {
	namespace ui {
		// リスト
		class List : public Component {
		protected:
			std::vector<Component*> children_;

		public:
			template<class... Args>
			List(Args... args) {
				for (auto child : std::initializer_list<Component*>{ args... }) {
					children_.emplace_back(child);
				}
			}

			void add(const Component* child) { children_.emplace_back(const_cast<Component*>(child)); }
			bool insert(size_t index, const Component* child);
			bool replace(size_t index, const Component* child);
			bool redraw() const noexcept override;
			bool show() const noexcept override;
			bool hide() const noexcept override;
		};

		// 行
		class Row : public List {
		public:
			template<class... Args>
			Row(Args... args) : List(args...) {}
			bool move(const WindowRectangle& rect) const noexcept override;
		};

		// 列
		class Column : public List {
		public:
			template<class... Args>
			Column(Args... args) : List(args...) {}
			bool move(const WindowRectangle& rect) const noexcept override;
		};
	}
}