#include "path_base.hpp"


namespace mkaul {
	namespace graphics {
		// 角度angleにある楕円上の点
		// 角度は時計の12時の方向を0dとした時計回りの度数
		void Path::ellipse_pos(const Size<float>& size, float angle, Point<float>* p_pt) {
			float rad = deg2rad(-angle + 90.f);
			float x = size.width * size.height / std::sqrt(size.height * size.height + size.width * size.width * (float)std::pow(std::tan(rad), 2)) * sign(std::cos(rad));
			float y = -x * std::tan(rad);

			p_pt->x = x;
			p_pt->y = y;
		}
	}
}