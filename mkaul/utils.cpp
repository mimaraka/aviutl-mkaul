//----------------------------------------------------------------------------------
//		MKAul (Utils)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "utils.hpp"



namespace mkaul {
	// �֐��̒u������
	void* rewrite_function(LPCSTR mod_name, LPCSTR func_name, void* func_ptr)
	{
		// �x�[�X�A�h���X
		auto base_adr = (DWORD)::GetModuleHandle(mod_name);

		if (base_adr) {
			ULONG size;
			auto p_img_desc = (PIMAGE_IMPORT_DESCRIPTOR)::ImageDirectoryEntryToData(
				(HMODULE)(intptr_t)base_adr,
				TRUE,
				IMAGE_DIRECTORY_ENTRY_IMPORT,
				&size
			);

			for (; p_img_desc->Name; p_img_desc++) {
				auto p_first_thunk = (PIMAGE_THUNK_DATA)(intptr_t)(base_adr + p_img_desc->FirstThunk);
				auto p_orig_first_thunk = (PIMAGE_THUNK_DATA)(intptr_t)(base_adr + p_img_desc->OriginalFirstThunk);

				// �֐���
				for (; p_first_thunk->u1.Function; p_first_thunk++, p_orig_first_thunk++) {
					if (IMAGE_SNAP_BY_ORDINAL(p_orig_first_thunk->u1.Ordinal)) continue;

					auto p_import_name = (PIMAGE_IMPORT_BY_NAME)(intptr_t)(base_adr + (DWORD)p_orig_first_thunk->u1.AddressOfData);

					// ������������
					if (::_stricmp((const char*)p_import_name->Name, func_name) != 0) continue;

					// �ی��Ԃ�ύX
					DWORD old_protect;

					if (::VirtualProtect(
						&p_first_thunk->u1.Function,
						sizeof(p_first_thunk->u1.Function),
						PAGE_READWRITE,
						&old_protect
					)) {
						// ��������
						auto p_orig_func = (void*)(intptr_t)p_first_thunk->u1.Function;
						::WriteProcessMemory(
							::GetCurrentProcess(),
							&p_first_thunk->u1.Function,
							&func_ptr,
							sizeof(p_first_thunk->u1.Function),
							NULL
						);
						p_first_thunk->u1.Function = (DWORD)(intptr_t)func_ptr;

						// �ی��Ԃ�߂�
						::VirtualProtect(
							&p_first_thunk->u1.Function,
							sizeof(p_first_thunk->u1.Function),
							old_protect,
							&old_protect
						);
						return p_orig_func;
					}
				}
			}
		}
		return nullptr;
	}
}