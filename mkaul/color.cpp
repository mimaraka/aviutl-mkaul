//----------------------------------------------------------------------------------
//		MKAul (Color)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "color.hpp"



namespace mkaul {
	// コンストラクタ(1)
	Color_F::Color_F(COLORREF cr = (COLORREF)0l)
	{
		r = GetRValue(cr) / (float)MAX;
		g = GetGValue(cr) / (float)MAX;
		b = GetBValue(cr) / (float)MAX;
		a = 1.f;
	}


	// コンストラクタ(2)
	Color_F::Color_F(const Color_I8& col_i8)
	{
		r = col_i8.r / (float)MAX;
		g = col_i8.g / (float)MAX;
		b = col_i8.b / (float)MAX;
		a = col_i8.a / (float)MAX;
	}

	
	inline void Color_F::clamp()
	{
		r = std::clamp(r, 0.f, 1.f);
		g = std::clamp(g, 0.f, 1.f);
		b = std::clamp(b, 0.f, 1.f);
		a = std::clamp(a, 0.f, 1.f);
	}


	inline void Color_F::invert()
	{
		r = 1.f - r;
		g = 1.f - g;
		b = 1.f - b;
	}


	inline void Color_F::change_contrast(float val)
	{
		r = 0.5f + (r - 0.5f) * val;
		g = 0.5f + (g - 0.5f) * val;
		b = 0.5f + (b - 0.5f) * val;
	}


	inline COLORREF Color_F::colorref() const
	{
		return RGB(
			std::clamp((int)(r * MAX), 0, MAX),
			std::clamp((int)(g * MAX), 0, MAX),
			std::clamp((int)(b * MAX), 0, MAX)
		);
	}


	inline D2D1_COLOR_F Color_F::d2d1_colorf() const
	{
		return D2D1::ColorF(
			std::clamp(b, 0.f, 1.f),
			std::clamp(g, 0.f, 1.f),
			std::clamp(r, 0.f, 1.f),
			std::clamp(a, 0.f, 1.f)
		);
	}



	// コンストラクタ(1)
	Color_I8::Color_I8(COLORREF cr = (COLORREF)0l)
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = MAX;
	}


	// コンストラクタ(2)
	Color_I8::Color_I8(const Color_F& col_f)
	{
		r = (int)(col_f.r * MAX);
		g = (int)(col_f.g * MAX);
		b = (int)(col_f.b * MAX);
		a = (int)(col_f.a * MAX);
	}


	inline void Color_I8::clamp()
	{
		r = std::clamp(r, 0, MAX);
		g = std::clamp(g, 0, MAX);
		b = std::clamp(b, 0, MAX);
		a = std::clamp(a, 0, MAX);
	}


	inline void Color_I8::invert()
	{
		r = MAX - r;
		g = MAX - g;
		b = MAX - b;
	}


	inline void Color_I8::change_contrast(float val)
	{
		r = MAX / 2 + (int)((r - MAX / 2) * val);
		g = MAX / 2 + (int)((g - MAX / 2) * val);
		b = MAX / 2 + (int)((b - MAX / 2) * val);
	}


	inline COLORREF Color_I8::colorref() const
	{
		return RGB(
			std::clamp(r, 0, MAX),
			std::clamp(g, 0, MAX),
			std::clamp(b, 0, MAX)
		);
	}


	inline D2D1_COLOR_F Color_I8::d2d1_colorf() const
	{
		return D2D1::ColorF(
			std::clamp(b / (float)MAX, 0.f, 1.f),
			std::clamp(g / (float)MAX, 0.f, 1.f),
			std::clamp(r / (float)MAX, 0.f, 1.f),
			std::clamp(a / (float)MAX, 0.f, 1.f)
		);
	}
}