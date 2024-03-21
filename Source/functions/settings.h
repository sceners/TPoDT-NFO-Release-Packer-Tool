#include "_global.h"

BOOL CALLBACK DlgSettings(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        SetDlgItemTextA(hwndDlg, IDC_EDT_SETTINGS_CRACKER, default_cracker);
        SetDlgItemTextA(hwndDlg, IDC_EDT_SETTINGS_PLATFORM, default_platform);
        SendMessageA(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1)));
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_CTLCOLORSTATIC:
    {
        SetTextColor((HDC)wParam, RGB(255,128,0));
        SetBkMode((HDC)wParam, TRANSPARENT);
    }
    return (BOOL)GetStockObject(NULL_BRUSH);

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_OK:
        {
            SetFocus(0);
            GetDlgItemTextA(hwndDlg, IDC_EDT_SETTINGS_CRACKER, default_cracker, 50);
            GetDlgItemTextA(hwndDlg, IDC_EDT_SETTINGS_PLATFORM, default_platform, 50);
            SendMessageA(hwndDlg, WM_CLOSE, 0, 0);
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}
