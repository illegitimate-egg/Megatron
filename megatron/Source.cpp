#include <Windows.h>
#include "resource.h"

// #pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

INT_PTR connectDialog(HWND, UINT, WPARAM, LPARAM);

int wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	INT_PTR connectionDialog = DialogBox(NULL, MAKEINTRESOURCE(IDD_CONNECT), NULL, connectDialog);

	return 0;
}

INT_PTR connectDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG: {  
			HICON icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_DLG));
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
			SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);

			SetDlgItemText(hWnd, IDC_PORT, L"6675");
			SetDlgItemText(hWnd, IDC_HOST, L"127.0.0.1");

			SendDlgItemMessage(hWnd, IDC_LIST1, LB_ADDSTRING, 5, (LPARAM) TEXT("Direct Connect"));
			SendDlgItemMessage(hWnd, IDC_LIST1, LB_ADDSTRING, 5, (LPARAM) TEXT("illegitimate-egg Master Server"));

			return true;
		}
		case WM_COMMAND: {
			switch (HIWORD(wParam)) {
				case BN_CLICKED:
					switch (LOWORD(wParam)) {
						case IDC_CLOSE:
							PostQuitMessage(0);
							return true;
						case IDC_HELP:
							MessageBox(hWnd, L"There is no help.", L"Help", MB_OK);
							return true;
					}
					break;
				case EN_CHANGE:
					WCHAR buff[256];

					EnableWindow(GetDlgItem(hWnd, IDC_LOGIN), 
						GetDlgItemText(hWnd, IDC_HOST, buff, 256) > 0 &&
						GetDlgItemText(hWnd, IDC_PORT, buff, 256) > 0 &&
						GetDlgItemText(hWnd, IDC_NICK, buff, 256) > 0);
					break;
			}
			break;
		}
		case WM_CLOSE:
			PostQuitMessage(0);

			return true;
	}

	return false;
}