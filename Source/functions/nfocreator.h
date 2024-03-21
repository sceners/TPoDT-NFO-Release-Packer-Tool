#include "_global.h"

void SetASCIIFont(HWND hwndDlg, int id)
{
    HWND stc=GetDlgItem(hwndDlg, id);
    HFONT hFont=(HFONT)SendMessageA(stc, WM_GETFONT, 0, 0);
    LOGFONT logFont= {0};
    GetObjectA(hFont, sizeof(LOGFONT), &logFont);
    logFont.lfHeight=-12;
    logFont.lfWidth=-7;
    logFont.lfWeight=FW_REGULAR;
    logFont.lfCharSet=OEM_CHARSET;
    logFont.lfPitchAndFamily=FIXED_PITCH|FF_MODERN;
    strcpy(logFont.lfFaceName, "Terminal");
    SendMessageA(stc, WM_SETFONT, (WPARAM)CreateFontIndirectA(&logFont), MAKELPARAM(true, 0));
}

void SetFontShort(HWND hwndDlg, int id, const char* font_name)
{
    SendMessageA(GetDlgItem(hwndDlg, id), WM_SETFONT, (WPARAM)CreateFont(-11, 0, 0, 0, 0, FALSE, FALSE, FALSE, 1, 0, 0, 0, 0, font_name), true);
}

const char* GetClipBoardText()
{
    OpenClipboard(0);
    const char* cliptext=(const char*)GetClipboardData(CF_OEMTEXT);
    CloseClipboard();
    return cliptext;
}

bool gtl(HWND hwndDlg, int id)
{
    if(SendDlgItemMessageA(hwndDlg, id, WM_GETTEXTLENGTH, 0, 0))
        return true;
    return false;
}

bool bool_true(bool* a, int total)
{
    int sum=0;
    for(int i=0; i<total; i++)
        sum+=a[i];
    if(sum==total)
        return true;
    else
        return false;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        hwndMainDlg=hwndDlg;
        SendMessageA(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1)));
        SetASCIIFont(hwndDlg, IDC_STATIC1);
        SetASCIIFont(hwndDlg, IDC_STATIC2);
        SetASCIIFont(hwndDlg, IDC_STATIC3);
        SetASCIIFont(hwndDlg, IDC_STATIC4);
        SetASCIIFont(hwndDlg, IDC_STATIC5);
        SetASCIIFont(hwndDlg, IDC_STATIC6);
        SetFontShort(hwndDlg, IDC_EDT_RELEASENOTES, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_INSTALLNOTES, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_PROGRAM, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_COMPANY, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_CRACKER, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_PROTECTION, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_RLZTYPE, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_RLZDATE, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_RLZSIZE, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_PLATFORM, "Courier New");
        SetFontShort(hwndDlg, IDC_EDT_WEBSITE, "Courier New");
        SendDlgItemMessageA(hwndDlg, IDC_EDT_WEBSITE, EM_SETLIMITTEXT, website_len, 0);
        ClearAllNfoInfo(hwndDlg);
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_CONTEXTMENU:
    {
        int idControl=GetDlgCtrlID((HWND)wParam);
        switch(idControl)
        {
        case IDC_EDT_INSTALLNOTES:
        case IDC_EDT_RELEASENOTES:
        {
            HMENU myMenu=NULL;
            myMenu=CreatePopupMenu();
            AppendMenu(myMenu, MF_STRING, 1, "Paste");
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            SetForegroundWindow(hwndDlg);
            UINT MenuItemClicked=TrackPopupMenu(myMenu, TPM_RETURNCMD|TPM_NONOTIFY, cursorPos.x, cursorPos.y, 0, hwndDlg, NULL);
            SendMessage(hwndDlg, WM_NULL, 0, 0);
            if(MenuItemClicked)
                SendMessageA((HWND)wParam, EM_REPLACESEL, true, (LPARAM)GetClipBoardText());
        }
        break;
        }
    }
    return TRUE;

    case WM_CTLCOLORSTATIC:
    {
        unsigned int color=0;
        switch(GetDlgCtrlID((HWND)lParam))
        {
        case IDC_STATIC1:
            color=0xFF0000;
            break;
        case IDC_STATIC2:
            color=0x1FE000;
            break;
        case IDC_STATIC3:
            color=0x03FC00;
            break;
        case IDC_STATIC4:
            color=0x007F80;
            break;
        case IDC_STATIC5:
            color=0x000FF0;
            break;
        case IDC_STATIC6:
            color=RGB(0,100,0);
            break;
        case IDC_STATIC7:
            color=0x00003F;
            break;
        case IDC_STATIC8:
            color=RGB(255,128,0);
            break;
        case IDC_STC_CLICKME1:
        case IDC_STC_CLICKME2:
            color=RGB(255,0,0);
            break;
        }
        SetTextColor((HDC)wParam, color);
        SetBkMode((HDC)wParam, TRANSPARENT);
    }
    return (BOOL)GetStockObject(NULL_BRUSH);

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_PACKRELEASE:
        {
            SetFocus(0);
            bool a[10]=
            {
                gtl(hwndDlg, IDC_EDT_PROGRAM),
                gtl(hwndDlg, IDC_EDT_COMPANY),
                gtl(hwndDlg, IDC_EDT_CRACKER),
                gtl(hwndDlg, IDC_EDT_PROTECTION),
                gtl(hwndDlg, IDC_EDT_RLZTYPE),
                gtl(hwndDlg, IDC_EDT_RLZDATE),
                gtl(hwndDlg, IDC_EDT_PLATFORM),
                gtl(hwndDlg, IDC_EDT_WEBSITE),
                gtl(hwndDlg, IDC_EDT_INSTALLNOTES),
                gtl(hwndDlg, IDC_EDT_RELEASENOTES)
            };
            if(bool_true(a, sizeof(a)))
            {
                DialogBox(hInst, MAKEINTRESOURCE(DLG_PACKRELEASE), hwndDlg, (DLGPROC)DlgPackRelease);
            }
            else
                MessageBoxA(hwndDlg, "Please enter more information...", "Warning", MB_ICONWARNING);
        }
        return TRUE;

        case IDM_FILE_SAVE:
        {
            bool a[10]=
            {
                gtl(hwndDlg, IDC_EDT_PROGRAM),
                gtl(hwndDlg, IDC_EDT_COMPANY),
                gtl(hwndDlg, IDC_EDT_CRACKER),
                gtl(hwndDlg, IDC_EDT_PROTECTION),
                gtl(hwndDlg, IDC_EDT_RLZTYPE),
                gtl(hwndDlg, IDC_EDT_RLZDATE),
                gtl(hwndDlg, IDC_EDT_PLATFORM),
                gtl(hwndDlg, IDC_EDT_WEBSITE),
                gtl(hwndDlg, IDC_EDT_INSTALLNOTES),
                gtl(hwndDlg, IDC_EDT_RELEASENOTES)
            };
            if(bool_true(a, sizeof(a)))
            {
                char nfo_file[256]="";
                char nfo_name[256]="";
                char diz_name[256]="";
                OPENFILENAME ofstruct;
                memset(&ofstruct, 0, sizeof(ofstruct));
                ofstruct.lStructSize=sizeof(ofstruct);
                ofstruct.hwndOwner=hwndDlg;
                ofstruct.hInstance=hInst;
                ofstruct.lpstrFilter="NFO Files (.nfo)\0*.nfo\0\0";
                ofstruct.lpstrFile=nfo_file;
                ofstruct.nMaxFile=256;
                ofstruct.lpstrDefExt="nfo";
                ofstruct.Flags=OFN_EXTENSIONDIFFERENT|OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_OVERWRITEPROMPT;
                GetSaveFileName(&ofstruct);
                if(!nfo_file[0])
                    return TRUE;
                int len=strlen(nfo_file);
                int i=0;
                while(nfo_file[len-i]!='\\')
                {
                    nfo_file[len-i]=0;
                    i++;
                }
                sprintf(nfo_name, "%sTPoDT.nfo", nfo_file);
                sprintf(diz_name, "%sfile_id.diz", nfo_file);
                if(WriteNfo(nfo_name, &nfo_struct) and WriteDiz(diz_name, &nfo_struct))
                    MessageBoxA(hwndDlg, "NFO & DIZ Written!", "Done!", MB_ICONINFORMATION);
                else
                    MessageBoxA(hwndDlg, "Something went wrong, try again later...", nfo_file, MB_ICONERROR);
            }
            else
                MessageBoxA(hwndDlg, "Please enter more information...", "Warning", MB_ICONWARNING);
        }
        return TRUE;

        case IDM_TOOLS_PREVIEW:
        {
            SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_PREVIEW, 0);
        }
        return TRUE;

        case IDC_BTN_PREVIEW:
        {
            SetFocus(0);
            char date[50]="";
            char date_question[256]="";
            GetDlgItemTextA(hwndDlg, IDC_EDT_RLZDATE, date, 50);
            bool a[10]=
            {
                gtl(hwndDlg, IDC_EDT_PROGRAM),
                gtl(hwndDlg, IDC_EDT_COMPANY),
                gtl(hwndDlg, IDC_EDT_CRACKER),
                gtl(hwndDlg, IDC_EDT_PROTECTION),
                gtl(hwndDlg, IDC_EDT_RLZTYPE),
                gtl(hwndDlg, IDC_EDT_RLZDATE),
                gtl(hwndDlg, IDC_EDT_PLATFORM),
                gtl(hwndDlg, IDC_EDT_WEBSITE),
                gtl(hwndDlg, IDC_EDT_INSTALLNOTES),
                gtl(hwndDlg, IDC_EDT_RELEASENOTES)
            };
            if(bool_true(a, sizeof(a)))
            {
                sprintf(date_question, "The current date is %s, is this correct?\n\n(Crackers often change their dates to expire trials)", date);
                if(MessageBoxA(hwndDlg, date_question, date, MB_ICONQUESTION|MB_YESNO)==IDNO)
                    return TRUE;
                char temp_name[256]="";
                GetTempPath(256, temp_name);
                sprintf(temp_name, "%s%u.nfo", temp_name, (unsigned int)GetTickCount());
                if(WriteNfo(temp_name, &nfo_struct))
                {
                    HANDLE hFile=CreateFileA(temp_name, GENERIC_ALL, 0, 0, OPEN_EXISTING, 0, 0);
                    if(hFile!=INVALID_HANDLE_VALUE)
                    {
                        int filesize=GetFileSize(hFile, 0);
                        DWORD read=0;
                        nfo_viewer_text=(char*)malloc(filesize+3);
                        memset(nfo_viewer_text, 0, filesize+3);
                        ReadFile(hFile, nfo_viewer_text, filesize+3, &read, 0);
                        strcat(nfo_viewer_text, "\r\n");
                        CloseHandle(hFile);
                        nfo_viewer_mustscroll=true;
                        if(nfo_viewer_text[0])
                            DialogBox(hInst, MAKEINTRESOURCE(DLG_NFOVIEWER), hwndDlg, (DLGPROC)DlgNfoViewer);
                        free(nfo_viewer_text);
                        EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_PACKRELEASE), 1);
                    }
                    else
                        MessageBoxA(hwndDlg, "Could not open the temporary NFO file, try again later...", temp_name, MB_ICONERROR);
                }
                else
                    MessageBoxA(hwndDlg, "Could not create the file, try again later...", temp_name, MB_ICONERROR);
            }
            else
                MessageBoxA(hwndDlg, "Please enter more information...", "Warning", MB_ICONWARNING);
        }
        return TRUE;

        case IDM_HELP_ABOUT:
        {
            MessageBoxA(hwndDlg, "TPoDT NFO Creator\n\nCreated by:\nMr. eXoDia // T.P.o.D.T 2012\nmr.exodia.tpodt@gmail.com\nhttp://www.tpodt.net16.net\n\nJust fill in all possible info and hit\nFile->Save to make the NFO and\nDIZ files...", ClientVersion, MB_ICONINFORMATION);
        }
        return TRUE;

        case IDM_FILE_EXIT:
        {
            SendMessageA(hwndDlg, WM_CLOSE, 0, 0);
        }
        return TRUE;

        case IDM_TOOLS_SETTINGS:
        {
            DialogBox(hInst, MAKEINTRESOURCE(DLG_SETTINGS), hwndDlg, (DLGPROC)DlgSettings);
            SetDlgItemTextA(hwndDlg, IDC_EDT_CRACKER, default_cracker);
            SetDlgItemTextA(hwndDlg, IDC_EDT_PLATFORM, default_platform);
            WritePrivateProfileStringA("Defaults", "cracker", default_cracker, ini_file);
            WritePrivateProfileStringA("Defaults", "platform", default_platform, ini_file);
        }
        return TRUE;

        case IDM_TOOLS_NAMEGEN:
        {
            DialogBox(hInst, MAKEINTRESOURCE(DLG_NAMEGEN), hwndDlg, (DLGPROC)DlgNameGen);
        }
        return TRUE;

        case IDC_BTN_FORMATRELEASE:
        {
            SetFocus(0);
            char* raw_text=(char*)malloc(max_len);
            memset(raw_text, 0, max_len);
            GetDlgItemTextA(hwndDlg, IDC_EDT_RELEASENOTES, raw_text, max_len);
            HMENU myMenu=NULL;
            myMenu=CreatePopupMenu();
            AppendMenuA(myMenu, MF_STRING, 1, "&Multi-line seperator");
            AppendMenuA(myMenu, MF_STRING, 2, "&Single-line seperator");
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            SetForegroundWindow(hwndDlg);
            UINT MenuItemClicked=TrackPopupMenu(myMenu, TPM_RETURNCMD | TPM_NONOTIFY, cursorPos.x, cursorPos.y, 0, hwndDlg, NULL);
            SendMessage(hwndDlg, WM_NULL, 0, 0);
            bool single_line=true;
            switch(MenuItemClicked)
            {
            case 1:
                single_line=false;
            case 2:
            {
                FormatText(raw_text, single_line);
                SetDlgItemTextA(hwndDlg, IDC_EDT_RELEASENOTES, raw_text);
            }
            break;
            }
            free(raw_text);
        }
        return TRUE;

        case IDC_BTN_FORMATINSTALL:
        {
            SetFocus(0);
            char* raw_text=(char*)malloc(max_len);
            memset(raw_text, 0, max_len);
            GetDlgItemTextA(hwndDlg, IDC_EDT_INSTALLNOTES, raw_text, max_len);
            HMENU myMenu=NULL;
            myMenu=CreatePopupMenu();
            AppendMenuA(myMenu, MF_STRING, 2, "&Single-line seperator");
            AppendMenuA(myMenu, MF_STRING, 1, "&Multi-line seperator");
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            SetForegroundWindow(hwndDlg);
            UINT MenuItemClicked=TrackPopupMenu(myMenu, TPM_RETURNCMD | TPM_NONOTIFY, cursorPos.x, cursorPos.y, 0, hwndDlg, NULL);
            SendMessage(hwndDlg, WM_NULL, 0, 0);
            bool single_line=true;
            switch(MenuItemClicked)
            {
            case 1:
                single_line=false;
            case 2:
            {
                FormatText(raw_text, single_line);
                SetDlgItemTextA(hwndDlg, IDC_EDT_INSTALLNOTES, raw_text);
            }
            break;
            }
            free(raw_text);
        }
        return TRUE;

        case IDC_EDT_PROGRAM:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.program, 50);
            MatchLength(nfo_struct.program, col1_len);
        }
        return TRUE;

        case IDC_EDT_COMPANY:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.company, 50);
            MatchLength(nfo_struct.company, col1_len);
        }
        return TRUE;

        case IDC_EDT_CRACKER:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.cracker, 50);
            MatchLength(nfo_struct.cracker, col1_len);
        }
        return TRUE;

        case IDC_EDT_PROTECTION:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.protection, 50);
            MatchLength(nfo_struct.protection, col1_len);
        }
        return TRUE;

        case IDC_EDT_RLZTYPE:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.rlz_type, 50);
            MatchLength(nfo_struct.rlz_type, col2_len);
        }
        return TRUE;

        case IDC_EDT_RLZDATE:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.rlz_date, 50);
            strcpy(nfo_struct.rlz_date_diz, nfo_struct.rlz_date);
            MatchLength(nfo_struct.rlz_date, col2_len);
        }
        return TRUE;

        case IDC_EDT_RLZSIZE:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.rlz_size, 50);
            MatchLength(nfo_struct.rlz_size, col2_len);
        }
        return TRUE;

        case IDC_EDT_PLATFORM:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.platform, 50);
            MatchLength(nfo_struct.platform, col2_len);
        }
        return TRUE;

        case IDC_EDT_WEBSITE:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.website, 100);
            MatchLength(nfo_struct.website, website_len);
        }
        return TRUE;

        case IDC_EDT_RELEASENOTES:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.release_notes, max_len);
        }
        return TRUE;

        case IDC_EDT_INSTALLNOTES:
        {
            GetDlgItemTextA(hwndDlg, LOWORD(wParam), nfo_struct.install_notes, max_len);
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}
