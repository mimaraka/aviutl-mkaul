//----------------------------------------------------------------------------------
//		MKAul (Window)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "window.hpp"



namespace mkaul {
	namespace window {
		// �q�E�B���h�E���ċA�I�Ɍ���
		void get_all_children(HWND hwnd, std::vector<HWND>* p_vec_hwnd)
		{
			HWND hwnd_child = ::GetWindow(hwnd, GW_CHILD);
			// ���񃋁[�v�͎q�E�B���h�E�����݂��邩�𔻒�(�q�E�B���h�E�����݂��Ȃ������炻�̂܂܏I��)
			// 2��ڈȍ~�̃��[�v�͌Z��E�B���h�E�����݂�����葱��
			while (hwnd_child) {
				// �ċA�Ăяo��
				get_all_children(hwnd_child, p_vec_hwnd);
				// �ċA�Ăяo�����I�������(=����ȏ�q�E�B���h�E��������Ȃ��Ȃ�����)
				// �E�B���h�E�n���h�������X�g�ɒǉ�
				p_vec_hwnd->emplace_back(hwnd_child);
				// �Z��E�B���h�E������
				hwnd_child = ::GetWindow(hwnd_child, GW_HWNDNEXT);
			}
		}

		// �E�B���h�E���쐬
		bool Window::create(
			HINSTANCE				hinst,
			HWND					hwnd_parent,
			LPCTSTR					window_name,
			LPCTSTR					class_name,
			WNDPROC					wndproc_,
			LONG					window_style,
			LONG					class_style,
			const Window_Rectangle&	rect,
			HCURSOR					cursor,
			LPVOID					lp_param
		)
		{
			WNDCLASSEX ws;
			ws.cbSize = sizeof(ws);
			ws.style = CS_HREDRAW | CS_VREDRAW | class_style;
			ws.lpfnWndProc = wndproc_;
			ws.cbClsExtra = 0;
			ws.cbWndExtra = 0;
			ws.hInstance = hinst;
			ws.hIcon = NULL;
			ws.hCursor = cursor;
			ws.hbrBackground = NULL;
			ws.lpszMenuName = NULL;
			ws.lpszClassName = class_name;
			ws.hIconSm = NULL;

			if (::RegisterClassEx(&ws)) {
				hwnd = ::CreateWindowEx(
					NULL,
					class_name,
					window_name,
					WS_VISIBLE | WS_CLIPCHILDREN | window_style,
					rect.left,
					rect.top,
					rect.right - rect.left,
					rect.bottom - rect.top,
					hwnd_parent,
					NULL,
					hinst,
					lp_param
				);
				if (hwnd) return true;
			}
			return false;
		}
	}
}