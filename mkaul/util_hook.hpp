//----------------------------------------------------------------------------------
//		MKAul (Utils (Hook))
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	// ä÷êîÇÃíuÇ´ä∑Ç¶
	bool replace_func(uintptr_t base_address, uintptr_t func_address, void* replaced_func);
	bool replace_func(const char* mod_name, uintptr_t func_ptr, void* replaced_func);
	void* replace_func(const char* mod_name, const char* func_name, void* replaced_func);
}