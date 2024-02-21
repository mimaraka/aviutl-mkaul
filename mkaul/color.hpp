#pragma once

#include <algorithm>
#include <Windows.h>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")



namespace mkaul {
	template <typename T>
	class Color {
	protected:
		static constexpr int MAX = 0xff;
		T r_, g_, b_, a_;

	public:
		constexpr bool operator == (const Color& col) const noexcept {
			return this->r_ == col.get_r() and this->g_ == col.get_g() and this->b_ == col.get_b() and this->a_ == col.get_a();
		}

		constexpr Color operator + (const Color& col) const noexcept {
			return Color(
				this->r_ + col.get_r(),
				this->g_ + col.get_g(),
				this->b_ + col.get_b(),
				this->a_ + col.get_a()
			);
		}

		constexpr Color operator - (const Color& col) const noexcept {
			return Color(
				this->r_ - col.get_r(),
				this->g_ - col.get_g(),
				this->b_ - col.get_b(),
				this->a_ - col.get_a()
			);
		}

		constexpr Color(
			T r = (T)0,
			T g = (T)0,
			T b = (T)0,
			T a = (T)0
		) :
			r_(r), g_(g), b_(b), a_(a)
		{}

		constexpr auto get_r() const noexcept { return r_; }
		constexpr auto get_g() const noexcept { return g_; }
		constexpr auto get_b() const noexcept { return b_; }
		constexpr auto get_a() const noexcept { return a_; }

		void set_r(T r) noexcept { r_ = r; }
		void set_g(T g) noexcept { g_ = g; }
		void set_b(T b) noexcept { b_ = b; }
		void set_a(T a) noexcept { a_ = a; }

		void change_color(T r_, T g_, T b_, T a_) noexcept;
		void change_brightness(T val) noexcept;
		void swap_br() noexcept;
	};

	template <typename T>
	inline void Color<T>::change_color(T r, T g, T b, T a) noexcept {
		r_ = r;
		g_ = g;
		b_ = b;
		a_ = a;
	}

	template <typename T>
	inline void Color<T>::change_brightness(T val) noexcept {
		r_ += val;
		g_ += val;
		b_ += val;
	}

	template <typename T>
	inline void Color<T>::swap_br() noexcept {
		T tmp = r_;
		r_ = b_;
		b_ = tmp;
	}


	class ColorI8;

	// Color (float)
	class ColorF : public Color<float> {
	public:
		void operator = (COLORREF cr) noexcept
		{
			this->r_ = GetRValue(cr) / (float)MAX;
			this->g_ = GetGValue(cr) / (float)MAX;
			this->b_ = GetBValue(cr) / (float)MAX;
			this->a_ = 1.f;
		}

		constexpr ColorF(COLORREF cr = (COLORREF)0l) noexcept;
		constexpr ColorF(const ColorI8& col_i8) noexcept;
		constexpr ColorF(int r, int g, int b, int a = MAX) noexcept;

		void clamp() noexcept;
		void invert() noexcept;
		void change_contrast(float val) noexcept;
		constexpr COLORREF colorref() const noexcept;
		D2D1_COLOR_F d2d1_colorf() const noexcept;
	};


	// Color (int)
	class ColorI8 : public Color<int> {
	public:
		void operator = (COLORREF cr)
		{
			this->r_ = GetRValue(cr);
			this->g_ = GetGValue(cr);
			this->b_ = GetBValue(cr);
			this->a_ = MAX;
		}

		constexpr ColorI8(COLORREF cr = (COLORREF)0l) noexcept;
		constexpr ColorI8(const ColorF& color) noexcept;

		void clamp() noexcept;
		void invert() noexcept;
		void change_contrast(float val) noexcept;
		constexpr COLORREF colorref() const noexcept;
		D2D1_COLOR_F d2d1_colorf() const noexcept;
	};


	// コンストラクタ(1)
	inline constexpr ColorF::ColorF(COLORREF cr) noexcept
	{
		r_ = GetRValue(cr) / (float)MAX;
		g_ = GetGValue(cr) / (float)MAX;
		b_ = GetBValue(cr) / (float)MAX;
		a_ = 1.f;
	}


	// コンストラクタ(2)
	inline constexpr ColorF::ColorF(const ColorI8& col_i8) noexcept
	{
		r_ = col_i8.get_r() / (float)MAX;
		g_ = col_i8.get_g() / (float)MAX;
		b_ = col_i8.get_b() / (float)MAX;
		a_ = col_i8.get_a() / (float)MAX;
	}


	// コンストラクタ(3)
	inline constexpr ColorF::ColorF(int r, int g, int b, int a) noexcept
	{
		r_ = r / (float)MAX;
		g_ = g / (float)MAX;
		b_ = b / (float)MAX;
		a_ = a / (float)MAX;
	}


	inline void ColorF::clamp() noexcept
	{
		r_ = std::clamp(r_, 0.f, 1.f);
		g_ = std::clamp(g_, 0.f, 1.f);
		b_ = std::clamp(b_, 0.f, 1.f);
		a_ = std::clamp(a_, 0.f, 1.f);
	}


	inline void ColorF::invert() noexcept
	{
		r_ = 1.f - r_;
		g_ = 1.f - g_;
		b_ = 1.f - b_;
	}


	inline void ColorF::change_contrast(float val) noexcept
	{
		r_ = 0.5f + (r_ - 0.5f) * val;
		g_ = 0.5f + (g_ - 0.5f) * val;
		b_ = 0.5f + (b_ - 0.5f) * val;
	}


	inline constexpr COLORREF ColorF::colorref() const noexcept
	{
		return RGB(
			std::clamp((int)(r_ * MAX), 0, MAX),
			std::clamp((int)(g_ * MAX), 0, MAX),
			std::clamp((int)(b_ * MAX), 0, MAX)
		);
	}


	inline D2D1_COLOR_F ColorF::d2d1_colorf() const noexcept
	{
		return D2D1::ColorF(
			std::clamp(r_, 0.f, 1.f),
			std::clamp(g_, 0.f, 1.f),
			std::clamp(b_, 0.f, 1.f),
			std::clamp(a_, 0.f, 1.f)
		);
	}



	// コンストラクタ(1)
	inline constexpr ColorI8::ColorI8(COLORREF cr) noexcept
	{
		r_ = GetRValue(cr);
		g_ = GetGValue(cr);
		b_ = GetBValue(cr);
		a_ = MAX;
	}


	// コンストラクタ(2)
	inline constexpr ColorI8::ColorI8(const ColorF& color) noexcept
	{
		r_ = (int)(color.get_r() * MAX);
		g_ = (int)(color.get_g() * MAX);
		b_ = (int)(color.get_b() * MAX);
		a_ = (int)(color.get_a() * MAX);
	}


	inline void ColorI8::clamp() noexcept
	{
		r_ = std::clamp(r_, 0, MAX);
		g_ = std::clamp(g_, 0, MAX);
		b_ = std::clamp(b_, 0, MAX);
		a_ = std::clamp(a_, 0, MAX);
	}


	inline void ColorI8::invert() noexcept
	{
		r_ = MAX - r_;
		g_ = MAX - g_;
		b_ = MAX - b_;
	}


	inline void ColorI8::change_contrast(float val) noexcept
	{
		r_ = MAX / 2 + (int)((r_ - MAX / 2) * val);
		g_ = MAX / 2 + (int)((g_ - MAX / 2) * val);
		b_ = MAX / 2 + (int)((b_ - MAX / 2) * val);
	}


	inline constexpr COLORREF ColorI8::colorref() const noexcept
	{
		return RGB(
			std::clamp(r_, 0, MAX),
			std::clamp(g_, 0, MAX),
			std::clamp(b_, 0, MAX)
		);
	}


	inline D2D1_COLOR_F ColorI8::d2d1_colorf() const noexcept
	{
		return D2D1::ColorF(
			std::clamp(b_ / (float)MAX, 0.f, 1.f),
			std::clamp(g_ / (float)MAX, 0.f, 1.f),
			std::clamp(r_ / (float)MAX, 0.f, 1.f),
			std::clamp(a_ / (float)MAX, 0.f, 1.f)
		);
	}
}