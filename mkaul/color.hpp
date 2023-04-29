//----------------------------------------------------------------------------------
//		MKAul (Color)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <d2d1.h>
#include "common.hpp"

#pragma comment(lib, "d2d1.lib")



namespace mkaul {
	template <typename T>
	struct Color {
	protected:
		static constexpr int MAX = 255;

	public:
		T r, g, b, a;

		bool operator == (const Color& col) const
		{
			return this->r == col.r && this->g == col.g && this->b == col.b && this->a == col.a;
		}

		Color operator + (const Color& col) const
		{
			return {
				this->r + col.r,
				this->g + col.g,
				this->b + col.b,
				this->a + col.a
			};
		}

		Color operator - (const Color& col) const
		{
			return {
				this->r - col.r,
				this->g - col.g,
				this->b - col.b,
				this->a - col.a
			};
		}

		Color(
			T r_ = (T)0,
			T g_ = (T)0,
			T b_ = (T)0,
			T a_ = (T)0
		) :
			r(r_),
			g(g_),
			b(b_),
			a(a_)
		{}

		void change_color(T r_, T g_, T b_, T a_);
		void change_brightness(T val);
		void swap_br();
	};

	template <typename T>
	inline void Color<T>::change_color(T r_, T g_, T b_, T a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	template <typename T>
	inline void Color<T>::change_brightness(T val)
	{
		r += val;
		g += val;
		b += val;
	}

	template <typename T>
	inline void Color<T>::swap_br()
	{
		T tmp = r;
		r = b;
		b = tmp;
	}


	struct Color_I8;

	// Color (float)
	struct Color_F : public Color<float> {
		void operator = (COLORREF cr)
		{
			this->r = GetRValue(cr) / (float)MAX;
			this->g = GetGValue(cr) / (float)MAX;
			this->b = GetBValue(cr) / (float)MAX;
			this->a = 1.f;
		}

		Color_F(COLORREF cr);
		Color_F(const Color_I8& col_i8);

		void clamp();
		void invert();
		void change_contrast(float val);
		COLORREF colorref() const;
		D2D1_COLOR_F d2d1_colorf() const;
	};


	// Color (int)
	struct Color_I8 : public Color<int> {
		void operator = (COLORREF cr)
		{
			this->r = GetRValue(cr);
			this->g = GetGValue(cr);
			this->b = GetBValue(cr);
			this->a = MAX;
		}

		Color_I8(COLORREF cr);
		Color_I8(const Color_F& col_f);

		void clamp();
		void invert();
		void change_contrast(float val);
		COLORREF colorref() const;
		D2D1_COLOR_F d2d1_colorf() const;
	};
}