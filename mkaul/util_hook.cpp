#include "util_hook.hpp"
#include <imagehlp.h>

#pragma comment(lib, "imagehlp.lib")



namespace mkaul {
	// �w��A�h���X�A�T�C�Y���̋@�B�����������
	bool replace_x86(uint32_t address, uint8_t code[], size_t n)
	{
		DWORD old_protect;

		if (!::VirtualProtect(reinterpret_cast<void*>(address), n, PAGE_EXECUTE_READWRITE, &old_protect)) {
			return false;
		}
		for (size_t i = 0; i < n; i++) {
			*((uint8_t*)address + i) = code[i];
		}
		return ::VirtualProtect(reinterpret_cast<void*>(address), n, old_protect, &old_protect);
	}


	// �֐��̒u������(�֐����w��)
	void* replace_func(uint32_t base_address, const char* func_name, void* replaced_func)
	{
		if (base_address) {
			ULONG size;
			auto p_img_desc = (PIMAGE_IMPORT_DESCRIPTOR)::ImageDirectoryEntryToData(
				(HMODULE)(intptr_t)base_address,
				TRUE,
				IMAGE_DIRECTORY_ENTRY_IMPORT,
				&size
			);

			for (; p_img_desc->Name; p_img_desc++) {
				auto p_first_thunk = (PIMAGE_THUNK_DATA)(intptr_t)(base_address + p_img_desc->FirstThunk);
				auto p_orig_first_thunk = (PIMAGE_THUNK_DATA)(intptr_t)(base_address + p_img_desc->OriginalFirstThunk);

				// �֐���
				for (; p_first_thunk->u1.Function; p_first_thunk++, p_orig_first_thunk++) {
					if (IMAGE_SNAP_BY_ORDINAL(p_orig_first_thunk->u1.Ordinal))
						continue;

					auto p_import_name = (PIMAGE_IMPORT_BY_NAME)(intptr_t)(base_address + (DWORD)p_orig_first_thunk->u1.AddressOfData);

					// ������������
					if (::_stricmp((const char*)p_import_name->Name, func_name) != 0)
						continue;

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
							&replaced_func,
							sizeof(p_first_thunk->u1.Function),
							NULL
						);
						p_first_thunk->u1.Function = (DWORD)(intptr_t)replaced_func;

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