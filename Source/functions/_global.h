#ifndef _GLOBAL_H
#define _GLOBAL_H

#define max_len 409600
#define line_size 69
#define col1_len 26
#define col2_len 16
#define website_len 62
#define ClientVersion "v0.5a"

char ServerVersion[100]="";
char ini_file[256]="";
char default_cracker[50]="";
char default_platform[50]="";
char* nfo_format;
char* diz_format;
char* team_info;
char* release_file_list=0;
char* nfo_viewer_text;
bool nfo_viewer_mustscroll=false;

typedef struct _NFO_STRUCT
{
    char program[50];
    char company[50];
    char cracker[50];
    char protection[50];
    char rlz_type[50];
    char rlz_date[50];
    char rlz_date_diz[50];
    char rlz_size[50];
    char platform[50];
    char website[100];
    char release_notes[max_len];
    char install_notes[max_len];
} NFO_STRUCT;

NFO_STRUCT nfo_struct;
HWND hwndMainDlg;
HINSTANCE hInst;

bool ExtractResource(HINSTANCE hInst, const char* resource_name, const char* resource_category, const char* filename)
{
    DWORD dwBytesWritten=0;
    HRSRC res=FindResource(hInst, resource_name, resource_category);
    if(!res)
        return false;
    DeleteFileA(filename);
    HANDLE hFile=CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
    if(hFile==INVALID_HANDLE_VALUE)
        return false;
    if(!WriteFile(hFile, LoadResource(hInst, res), SizeofResource(hInst, res), &dwBytesWritten, 0))
    {
        CloseHandle(hFile);
        return false;
    }
    CloseHandle(hFile);
    return true;
}

bool WriteNfo(char* nfo_path, NFO_STRUCT* nfo)
{
    char final_nfo_text[65535]="";
    char all_nfo_vals[65535]="";
    sprintf(all_nfo_vals, nfo_format, nfo->program, nfo->rlz_type, nfo->company, nfo->rlz_date, nfo->cracker, nfo->rlz_size, nfo->protection, nfo->platform, nfo->website, nfo->release_notes, nfo->install_notes);
    sprintf(final_nfo_text, "%s%s", all_nfo_vals, team_info);
    HANDLE hFile=CreateFileA(nfo_path, GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile==INVALID_HANDLE_VALUE)
        return false;
    DWORD written=0;
    if(!WriteFile(hFile, final_nfo_text, strlen(final_nfo_text), &written, 0))
    {
        CloseHandle(hFile);
        DeleteFile(nfo_path);
        return false;
    }
    CloseHandle(hFile);
    return true;
}

bool WriteDiz(char* diz_path, NFO_STRUCT* nfo)
{
    char final_diz_text[10000]="";
    sprintf(final_diz_text, diz_format, nfo->rlz_date_diz, nfo->program, nfo->rlz_type, nfo->platform);
    HANDLE hFile=CreateFileA(diz_path, GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile==INVALID_HANDLE_VALUE)
        return false;
    DWORD written=0;
    if(!WriteFile(hFile, final_diz_text, strlen(final_diz_text), &written, 0))
    {
        CloseHandle(hFile);
        DeleteFile(diz_path);
        return false;
    }
    CloseHandle(hFile);
    return true;
}

void CopyToClipboard(const char* text)
{
    HGLOBAL hText;
    char *pText;

    hText = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, strlen(text)+1);
    pText = (char*)GlobalLock(hText);
    strcpy(pText, text);

    OpenClipboard(0);
    EmptyClipboard();
    if(!SetClipboardData(CF_OEMTEXT, hText))
    {
        MessageBeep(MB_ICONERROR);
    }
    CloseClipboard();
}

void ClearAllNfoInfo(HWND hwndDlg)
{
    memset(&nfo_struct, 0, sizeof(NFO_STRUCT));
    SetDlgItemTextA(hwndDlg, IDC_EDT_CRACKER, default_cracker);
    SetDlgItemTextA(hwndDlg, IDC_EDT_PLATFORM, default_platform);

    SetDlgItemTextA(hwndDlg, IDC_EDT_PROGRAM, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_COMPANY, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_PROTECTION, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_RLZTYPE, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_RLZSIZE, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_WEBSITE, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_RELEASENOTES, "");
    SetDlgItemTextA(hwndDlg, IDC_EDT_INSTALLNOTES, "");

    SYSTEMTIME curtime= {0};
    GetSystemTime(&curtime);
    char current_date[20]="";
    sprintf(current_date, "%.2d-%.2d-%.4d", curtime.wDay, curtime.wMonth, curtime.wYear);
    SetDlgItemTextA(hwndDlg, IDC_EDT_RLZDATE, current_date);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_PROGRAM, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_COMPANY, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_CRACKER, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_PROTECTION, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_RLZTYPE, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_RLZSIZE, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_PLATFORM, 0);
    SendMessageA(hwndDlg, WM_COMMAND, IDC_EDT_WEBSITE, 0);
    EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_PACKRELEASE), 0);
    if(release_file_list)
    {
        free(release_file_list);
        release_file_list=0;
    }
}

#endif
