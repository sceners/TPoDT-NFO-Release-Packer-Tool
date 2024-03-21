#include "_global.h"

BOOL CALLBACK ScrollSubClass(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC oldproc=(WNDPROC)GetWindowLongA(hwndDlg, GWL_USERDATA);
    SCROLLINFO sbar= {0};
    bool bottom=false;
    switch(uMsg)
    {
    case WM_MOUSEWHEEL:
    case WM_VSCROLL:
    {
        sbar.cbSize=sizeof(SCROLLINFO);
        sbar.fMask=SIF_ALL;
        GetScrollInfo(hwndDlg, SB_VERT, &sbar);
        if(sbar.nPage+sbar.nPos>(unsigned int)sbar.nMax)
            bottom=true;
        EnableWindow(GetDlgItem(GetParent(hwndDlg), IDC_BUTTON1), bottom);
    }
    break;
    }
    return CallWindowProc(oldproc, hwndDlg, uMsg, wParam, lParam);
}

BOOL CALLBACK DlgNfoViewer(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hwndControl=GetDlgItem(hwndDlg, IDC_EDIT1);

        if(nfo_viewer_mustscroll and !GetWindowLongA(hwndControl, GWL_USERDATA))
        {
            EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON1), 0);
            SetWindowLongA(hwndControl, GWL_USERDATA, (LONG)SetWindowLong(hwndControl, GWLP_WNDPROC, (LONG)ScrollSubClass));
        }
        SendMessageA(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCEA(IDI_ICON1)));
        if(nfo_viewer_text)
            SetDlgItemTextA(hwndDlg, IDC_EDIT1, nfo_viewer_text);
    }
    return TRUE;

    case WM_CTLCOLORSTATIC:
    {
        HBRUSH brush;
        if(GetDlgCtrlID((HWND)lParam)==IDC_EDIT1)
            brush=GetSysColorBrush(COLOR_WINDOW);
        else
            brush=(HBRUSH)GetStockObject(NULL_BRUSH);
        return (BOOL)brush;
    }

    case WM_CLOSE:
    {
        if(IsWindowEnabled(GetDlgItem(hwndDlg, IDC_BUTTON1)) or !nfo_viewer_mustscroll)
            EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BUTTON1:
        {
            SetFocus(0);
            SendMessageA(hwndDlg, WM_CLOSE, 0, 0);
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}
