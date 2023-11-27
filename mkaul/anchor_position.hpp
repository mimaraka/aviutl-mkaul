#pragma once



namespace mkaul {
	namespace graphics {
		struct AnchorPosition {
			enum class Horizontal {
				Center,
				Left,
				Right
			} horizontal;
			enum class Vertical {
				Center,
				Top,
				Bottom
			} vertical;

			AnchorPosition(
				Horizontal horizontal_ = Horizontal::Center,
				Vertical vertical_ = Vertical::Center
			) :
				horizontal(horizontal_),
				vertical(vertical_)
			{}
		};
	}
}