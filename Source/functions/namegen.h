#include "_global.h"

BOOL CALLBACK DlgNameGen(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        char program[50]="";
        char type[50]="";
        char company[50]="";
        SendMessageA(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1)));
        GetDlgItemTextA(hwndMainDlg, IDC_EDT_COMPANY, company, 50);
        GetDlgItemTextA(hwndMainDlg, IDC_EDT_RLZTYPE, type, 50);
        GetDlgItemTextA(hwndMainDlg, IDC_EDT_PROGRAM, program, 50);
        SetDlgItemTextA(hwndDlg, IDC_EDT_NAME_COMPANY, company);
        SetDlgItemTextA(hwndDlg, IDC_EDT_NAME_RELEASETYPE, type);
        SetDlgItemTextA(hwndDlg, IDC_EDT_NAME_PROGRAM, program);
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_GENERATE:
        {
            char program[50]="";
            char type[50]="";
            char company[50]="";
            char final_text[150]="";
            int a=GetDlgItemTextA(hwndDlg, IDC_EDT_NAME_COMPANY, company, 50);
            int b=GetDlgItemTextA(hwndDlg, IDC_EDT_NAME_RELEASETYPE, type, 50);
            int c=GetDlgItemTextA(hwndDlg, IDC_EDT_NAME_PROGRAM, program, 50);
            if(a and b and c)
            {
                sprintf(final_text, "%s.%s.%s-TPoDT", RemoveCrap(company), RemoveCrap(program), RemoveCrap(type));
                SetDlgItemTextA(hwndDlg, IDC_EDT_NAME_RELEASENAME, final_text);
                SendDlgItemMessageA(hwndDlg, IDC_EDT_NAME_RELEASENAME, EM_SETSEL, 0, -1);
                SetFocus(GetDlgItem(hwndDlg, IDC_EDT_NAME_RELEASENAME));
            }
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}
