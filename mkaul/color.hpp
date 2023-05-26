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

		bool operator == (const Color& col) const noexcept
		{
			return this->r == col.r && this->g == col.g && this->b == col.b && this->a == col.a;
		}

		Color operator + (const Color& col) const noexcept
		{
			return {
				this->r + col.r,
				this->g + col.g,
				this->b + col.b,
				this->a + col.a
			};
		}

		Color operator - (const Color& col) const noexcept
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

		void change_color(T r_, T g_, T b_, T a_) noexcept;
		void change_brightness(T val) noexcept;
		void swap_br() noexcept;
	};

	template <typename T>
	inline void Color<T>::change_color(T r_, T g_, T b_, T a_) noexcept
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	template <typename T>
	inline void Color<T>::change_brightness(T val) noexcept
	{
		r += val;
		g += val;
		b += val;
	}

	template <typename T>
	inline void Color<T>::swap_br() noexcept
	{
		T tmp = r;
		r = b;
		b = tmp;
	}


	struct Color_I8;

	// Color (float)
	struct Color_F : public Color<float> {
		void operator = (COLORREF cr) noexcept
		{
			this->r = GetRValue(cr) / (float)MAX;
			this->g = GetGValue(cr) / (float)MAX;
			this->b = GetBValue(cr) / (float)MAX;
			this->a = 1.f;
		}

		Color_F(COLORREF cr = (COLORREF)0l) noexcept;
		Color_F(const Color_I8& col_i8) noexcept;
		Color_F(int r, int g, int b, int a = 1) noexcept;

		void clamp() noexcept;
		void invert() noexcept;
		void change_contrast(float val) noexcept;
		COLORREF colorref() const noexcept;
		D2D1_COLOR_F d2d1_colorf() const noexcept;
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

		Color_I8(COLORREF cr = (COLORREF)0l) noexcept;
		Color_I8(const Color_F& col_f) noexcept;

		void clamp() noexcept;
		void invert() noexcept;
		void change_contrast(float val) noexcept;
		COLORREF colorref() const noexcept;
		D2D1_COLOR_F d2d1_colorf() const noexcept;
	};


	// コンストラクタ(1)
	inline Color_F::Color_F(COLORREF cr) noexcept
	{
		r = GetRValue(cr) / (float)MAX;
		g = GetGValue(cr) / (float)MAX;
		b = GetBValue(cr) / (float)MAX;
		a = 1.f;
	}


	// コンストラクタ(2)
	inline Color_F::Color_F(const Color_I8& col_i8) noexcept
	{
		r = col_i8.r / (float)MAX;
		g = col_i8.g / (float)MAX;
		b = col_i8.b / (float)MAX;
		a = col_i8.a / (float)MAX;
	}


	// コンストラクタ(3)
	inline Color_F::Color_F(int r_, int g_, int b_, int a_) noexcept
	{
		r = r_ / (float)MAX;
		g = g_ / (float)MAX;
		b = b_ / (float)MAX;
		a = a_ / (float)MAX;
	}


	inline void Color_F::clamp() noexcept
	{
		r = std::clamp(r, 0.f, 1.f);
		g = std::clamp(g, 0.f, 1.f);
		b = std::clamp(b, 0.f, 1.f);
		a = std::clamp(a, 0.f, 1.f);
	}


	inline void Color_F::invert() noexcept
	{
		r = 1.f - r;
		g = 1.f - g;
		b = 1.f - b;
	}


	inline void Color_F::change_contrast(float val) noexcept
	{
		r = 0.5f + (r - 0.5f) * val;
		g = 0.5f + (g - 0.5f) * val;
		b = 0.5f + (b - 0.5f) * val;
	}


	inline COLORREF Color_F::colorref() const noexcept
	{
		return RGB(
			std::clamp((int)(r * MAX), 0, MAX),
			std::clamp((int)(g * MAX), 0, MAX),
			std::clamp((int)(b * MAX), 0, MAX)
		);
	}


	inline D2D1_COLOR_F Color_F::d2d1_colorf() const noexcept
	{
		return D2D1::ColorF(
			std::clamp(b, 0.f, 1.f),
			std::clamp(g, 0.f, 1.f),
			std::clamp(r, 0.f, 1.f),
			std::clamp(a, 0.f, 1.f)
		);
	}



	// コンストラクタ(1)
	inline Color_I8::Color_I8(COLORREF cr) noexcept
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = MAX;
	}


	// コンストラクタ(2)
	inline Color_I8::Color_I8(const Color_F& col_f) noexcept
	{
		r = (int)(col_f.r * MAX);
		g = (int)(col_f.g * MAX);
		b = (int)(col_f.b * MAX);
		a = (int)(col_f.a * MAX);
	}


	inline void Color_I8::clamp() noexcept
	{
		r = std::clamp(r, 0, MAX);
		g = std::clamp(g, 0, MAX);
		b = std::clamp(b, 0, MAX);
		a = std::clamp(a, 0, MAX);
	}


	inline void Color_I8::invert() noexcept
	{
		r = MAX - r;
		g = MAX - g;
		b = MAX - b;
	}


	inline void Color_I8::change_contrast(float val) noexcept
	{
		r = MAX / 2 + (int)((r - MAX / 2) * val);
		g = MAX / 2 + (int)((g - MAX / 2) * val);
		b = MAX / 2 + (int)((b - MAX / 2) * val);
	}


	inline COLORREF Color_I8::colorref() const noexcept
	{
		return RGB(
			std::clamp(r, 0, MAX),
			std::clamp(g, 0, MAX),
			std::clamp(b, 0, MAX)
		);
	}


	inline D2D1_COLOR_F Color_I8::d2d1_colorf() const noexcept
	{
		return D2D1::ColorF(
			std::clamp(b / (float)MAX, 0.f, 1.f),
			std::clamp(g / (float)MAX, 0.f, 1.f),
			std::clamp(r / (float)MAX, 0.f, 1.f),
			std::clamp(a / (float)MAX, 0.f, 1.f)
		);
	}
}