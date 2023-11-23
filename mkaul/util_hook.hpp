//----------------------------------------------------------------------------------
//		MKAul (Utils (Hook))
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <stdint.h>



namespace mkaul {
	// 指定アドレス、サイズ分の機械語を書き換え
	bool replace_x86(uint32_t address, uint8_t code[], size_t n);

	// 同じ型の変数を置き換え
	template <typename T>
	bool replace_var(uint32_t address, T new_var)
	{
		auto code = reinterpret_cast<uint8_t*>(&new_var);
		return replace_x86(address, code, sizeof(T));
	}

	template <typename T>
	bool replace_var(T* p_old_var, T new_var)
	{
		return replace_var(reinterpret_cast<uint32_t>(p_old_var), new_var);
	}

	// 関数の置き換え(関数名から)
	void* replace_func(uint32_t base_address, const char* func_name, void* replaced_func);
}