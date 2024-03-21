#include "_global.h"

void ListAdd(HWND hwndDlg, int id, const char* text)
{
    SendDlgItemMessageA(hwndDlg, IDC_LIST_FILES, LB_ADDSTRING, 0, (LPARAM)text);
}

char* GetFileNameNoExt(const char* filename)
{
    int len=strlen(filename);
    while(filename[len]!='\\' and len)
        len--;
    return (char*)filename+len+1;
}

bool RemoveFileNameNoExtFromList(HWND hwndDlg, const char* filename_noext)
{
    int len=strlen(filename_noext);
    int len2=strlen(release_file_list);
    char* filename_noext_lower=(char*)malloc(len+1);
    filename_noext_lower[0]=0;
    strcpy(filename_noext_lower, filename_noext);
    _strlwr(filename_noext_lower);
    char* release_file_list_lower=(char*)malloc(len2+1);
    release_file_list_lower[0]=0;
    strcpy(release_file_list_lower, release_file_list);
    _strlwr(release_file_list_lower);

    char* found=strstr(release_file_list_lower, filename_noext_lower);
    free(filename_noext_lower);
    if(found)
    {
        if(hwndDlg)
            if(MessageBoxA(hwndDlg, "Duplicate filename found...\n\nDo you want to replace the current?", "Error", MB_ICONWARNING|MB_YESNO)==IDNO)
                return false;
        int i=found-release_file_list_lower;
        int j=0;
        while(release_file_list_lower[i]!='\n' and i)
        {
            i--;
            j++;
        }
        int start=i;
        if(start)
            start++;
        int end=i+j+len;

        char* new_list=(char*)malloc(len2);
        memset(new_list, 0, len2);
        strncpy(new_list, release_file_list, start);
        strcat(new_list, release_file_list+end+1);
        strcpy(release_file_list, new_list);
        free(new_list);
        free(release_file_list_lower);
        return true;
    }
    free(release_file_list_lower);
    return false;
}

void ParseFileList(HWND hwndDlg, int id)
{
    HWND list_hwnd=GetDlgItem(hwndDlg, id);
    SendMessageA(list_hwnd, LB_RESETCONTENT, 0, 0);
    int len=strlen(release_file_list);
    char filename[256]="";
    for(int i=0,j=0; i<len; i++)
    {
        while(release_file_list[i]!='\n')
            i++;
        strncpy(filename, release_file_list+j, i);
        SendMessageA(list_hwnd, LB_ADDSTRING, 0, (LPARAM)GetFileNameNoExt(filename));
    }
}

char* GetRoundedReleaseSize(unsigned int filesize)
{
    char* text=(char*)malloc(25);
    text[0]=0;
    float bytes=filesize;
    float result;
    char format[10]="";
    if(bytes>1024*1024)
    {
        result=bytes/(1024*1024);
        if(bytes<1024*1024*10)
            strcpy(format, "~%.2fmb");
        else if(bytes<1024*1024*100)
            strcpy(format, "~%.1fmb");
        else
            strcpy(format, "~%.0fmb");
    }
    else
    {
        result=bytes/1024;
        if(bytes<1024*10)
            strcpy(format, "~%.2fkb");
        else if(bytes<1024*100)
            strcpy(format, "~%.1fkb");
        else
            strcpy(format, "~%.0fkb");
    }
    sprintf(text, format, result);
    int len=strlen(text);
    for(int i=0; i<len; i++)
    {
        if(text[i]=='.')
            text[i]=',';
    }
    return text;
}

BOOL CALLBACK DlgPackRelease(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        if(!release_file_list)
        {
            release_file_list=(char*)malloc(max_len);
            memset(release_file_list, 0, max_len);
        }
        else
            ParseFileList(hwndDlg, IDC_LIST_FILES);
        char final_text[256]="";
        char company[50]="";
        char program[50]="";
        char type[50]="";
        char date[50]="";
        int a=GetDlgItemTextA(hwndMainDlg, IDC_EDT_COMPANY, company, 50);
        int b=GetDlgItemTextA(hwndMainDlg, IDC_EDT_RLZTYPE, type, 50);
        int c=GetDlgItemTextA(hwndMainDlg, IDC_EDT_PROGRAM, program, 50);
        GetDlgItemTextA(hwndMainDlg, IDC_EDT_RLZDATE, date, 50);
        SetDlgItemTextA(hwndDlg, IDC_EDT_DATECHECK, date);
        if(a and b and c)
        {
            sprintf(final_text, "%s.%s.%s-TPoDT", RemoveCrap(company), RemoveCrap(program), RemoveCrap(type));
            SetDlgItemTextA(hwndDlg, IDC_EDT_RELEASENAME, final_text);
        }
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_DROPFILES:
    {
        char filename[256]="";
        char filename_noext_lwr[256]="";
        char* filename_noext=0;

        DragQueryFileA((HDROP)wParam, 0, filename, 256);

        filename_noext=GetFileNameNoExt(filename);
        strcpy(filename_noext_lwr, filename_noext);
        _strlwr(filename_noext_lwr);
        if(strstr(filename_noext_lwr, ".nfo") or strstr(filename_noext_lwr, ".diz"))
        {
            MessageBoxA(hwndDlg, "NFO & DIZ files should not be added here, they are auto-generated :)", "Notice", MB_ICONWARNING);
            return TRUE;
        }
        bool duplicate_replaced=RemoveFileNameNoExtFromList(hwndDlg, filename_noext);
        sprintf(release_file_list, "%s%s\n", release_file_list, filename);
        if(!duplicate_replaced)
            ListAdd(hwndDlg, IDC_LIST_FILES, filename_noext);
        else
            ParseFileList(hwndDlg, IDC_LIST_FILES);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_PACK:
        {
            SetFocus(0);
            bool pack_success=false;
            char curdir[256]="";
            char temp_path[256]="";
            char rar_exe_path[256]="";
            char list_file_path[256]="";
            char tpodt_nfo_path[256]="";
            char file_id_diz_path[256]="";
            char release_name_norar[256]="";
            char release_name[256]="";
            char commandline1[256]="";
            char commandline2[256]="";
            char release_path[256]="";
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            GetCurrentDirectoryA(256, curdir);
            sprintf(temp_path, "%s\\temp", curdir);
            if(!CreateDirectoryA(temp_path, 0))
            {
                if(GetLastError()!=ERROR_ALREADY_EXISTS)
                {
                    strcpy(temp_path, curdir);
                    MessageBoxA(hwndDlg, "Could not create /temp directory, using the current DIR.", "Warning", MB_ICONWARNING);
                }
            }
            sprintf(rar_exe_path, "%s\\rar.exe", temp_path);

            if(!ExtractResource(hInst, "RAR_EXE", "DATA", rar_exe_path))
            {
                MessageBoxA(hwndDlg, "Could not extract RAR.exe from the resources...", "Error", MB_ICONERROR);
                return TRUE;
            }
            sprintf(tpodt_nfo_path, "%s\\TPoDT.nfo", temp_path);
            sprintf(file_id_diz_path, "%s\\file_id.diz", temp_path);
            strcpy(nfo_struct.rlz_size, "~1,00mb");
            MatchLength(nfo_struct.rlz_size, col2_len);
            if(WriteNfo(tpodt_nfo_path, &nfo_struct))
            {
                if(WriteDiz(file_id_diz_path, &nfo_struct))
                {
                    char* release_file_list_final=(char*)malloc(strlen(release_file_list)+1024);
                    release_file_list_final[0]=0;
                    sprintf(release_file_list_final, "%s%s\n%s", release_file_list, tpodt_nfo_path, file_id_diz_path);
                    sprintf(list_file_path, "%s\\release.lst", temp_path);
                    HANDLE hFile=CreateFileA(list_file_path, GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
                    if(hFile!=INVALID_HANDLE_VALUE)
                    {
                        DWORD written=0;
                        if(WriteFile(hFile, release_file_list_final, strlen(release_file_list_final), &written, 0))
                        {
                            free(release_file_list_final);
                            CloseHandle(hFile);
                            GetDlgItemTextA(hwndDlg, IDC_EDT_RELEASENAME, release_name, 256);
                            strcpy(release_name_norar, release_name);
                            strcat(release_name, ".rar");
                            sprintf(commandline1, "%s a -m5 -ep %s @release.lst", rar_exe_path, release_name);
                            sprintf(commandline2, "%s k %s", rar_exe_path, release_name);
                            memset(&pi, 0, sizeof(PROCESS_INFORMATION));
                            memset(&si, 0, sizeof(STARTUPINFO));
                            if(CreateProcessA(0, commandline1, 0, 0, 0, CREATE_NO_WINDOW, 0, temp_path, &si, &pi))
                            {
                                DWORD proc_ret=STILL_ACTIVE;
                                while(proc_ret==STILL_ACTIVE)
                                {
                                    GetExitCodeProcess(pi.hProcess, &proc_ret);
                                    Sleep(100);
                                }
                                sprintf(release_path, "%s\\%s", temp_path, release_name);
                                hFile=CreateFileA(release_path, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
                                if(hFile!=INVALID_HANDLE_VALUE)
                                {
                                    unsigned int filesize=GetFileSize(hFile, 0);
                                    CloseHandle(hFile);
                                    if(DeleteFile(release_path))
                                    {
                                        strcpy(nfo_struct.rlz_size, GetRoundedReleaseSize(filesize));
                                        SetDlgItemTextA(hwndMainDlg, IDC_EDT_RLZSIZE, nfo_struct.rlz_size);
                                        MatchLength(nfo_struct.rlz_size, col2_len);
                                        if(DeleteFileA(tpodt_nfo_path))
                                        {
                                            if(WriteNfo(tpodt_nfo_path, &nfo_struct))
                                            {
                                                memset(&pi, 0, sizeof(PROCESS_INFORMATION));
                                                memset(&si, 0, sizeof(STARTUPINFO));
                                                if(CreateProcessA(0, commandline1, 0, 0, 0, CREATE_NO_WINDOW, 0, temp_path, &si, &pi))
                                                {
                                                    memset(&pi, 0, sizeof(PROCESS_INFORMATION));
                                                    memset(&si, 0, sizeof(STARTUPINFO));
                                                    if(CreateProcessA(0, commandline2, 0, 0, 0, CREATE_NO_WINDOW, 0, temp_path, &si, &pi))
                                                    {
                                                        proc_ret=STILL_ACTIVE;
                                                        while(proc_ret==STILL_ACTIVE)
                                                        {
                                                            GetExitCodeProcess(pi.hProcess, &proc_ret);
                                                            Sleep(100);
                                                        }
                                                        CopyToClipboard(release_name_norar);
                                                        MessageBoxA(hwndDlg, "Release Packed! (Release Name is on clipboard)", "Done", MB_ICONINFORMATION);
                                                        char new_location[256]="";
                                                        strcpy(new_location, release_name);
                                                        OPENFILENAME ofstruct;
again_selectfile:
                                                        memset(&ofstruct, 0, sizeof(ofstruct));
                                                        ofstruct.lStructSize=sizeof(ofstruct);
                                                        ofstruct.hwndOwner=hwndDlg;
                                                        ofstruct.hInstance=hInst;
                                                        ofstruct.lpstrFilter="Release files (*.rar)\0*.rar\0\0";
                                                        ofstruct.lpstrFile=new_location;
                                                        ofstruct.nMaxFile=MAX_PATH;
                                                        ofstruct.lpstrInitialDir=0;
                                                        ofstruct.lpstrTitle="Save file";
                                                        ofstruct.lpstrDefExt="rar";
                                                        ofstruct.Flags=OFN_EXTENSIONDIFFERENT|OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_OVERWRITEPROMPT;
                                                        if(!GetSaveFileName(&ofstruct))
                                                            goto again_selectfile;

                                                        if(CopyFile(release_path, new_location, false))
                                                        {
                                                            if(MessageBoxA(hwndDlg, "Are you finished with the current release?\n\n(All fields and the list of files will be cleared)", "All Done!", MB_ICONQUESTION|MB_YESNO)==IDYES)
                                                                pack_success=true;
                                                        }
                                                        else
                                                            MessageBoxA(hwndDlg, "Could not copy the final release file", "Error", MB_ICONERROR);
                                                    }
                                                    else
                                                        MessageBoxA(hwndDlg, "Could not create process rar.exe (3rd time)", "Error", MB_ICONERROR);
                                                }
                                                else
                                                    MessageBoxA(hwndDlg, "Could not create process rar.exe (2nd time)", "Error", MB_ICONERROR);
                                            }
                                            else
                                                MessageBoxA(hwndDlg, "Failed to create final TPoDT.nfo...", "Error", MB_ICONERROR);
                                        }
                                        else
                                            MessageBoxA(hwndDlg, "Could not delete the semi-final TPoDT.nfo...", "Error", MB_ICONERROR);
                                    }
                                    else
                                        MessageBoxA(hwndDlg, "Could not delete the semi-final release file...", "Error", MB_ICONERROR);
                                }
                                else
                                    MessageBoxA(hwndDlg, "Could not open the semi-final release file, probably something went wrong :(", "Error", MB_ICONERROR);
                            }
                            else
                                MessageBoxA(hwndDlg, "Could not create process rar.exe (1st time)", "Error", MB_ICONERROR);
                        }
                        else
                            MessageBoxA(hwndDlg, "Could not write to file release.lst...\n\nMake sure it's not in use.", "Error", MB_ICONERROR);
                    }
                    else
                        MessageBoxA(hwndDlg, "Could not create release.lst in the temporary directory...\n\nMake sure it's not in use.", "Error", MB_ICONERROR);
                }
                else
                    MessageBoxA(hwndDlg, "Failed to create file_id.diz...", "Error", MB_ICONERROR);
            }
            else
                MessageBoxA(hwndDlg, "Failed to create semi-final TPoDT.nfo...", "Error", MB_ICONERROR);

            DeleteFile(tpodt_nfo_path);
            DeleteFile(file_id_diz_path);
            DeleteFile(list_file_path);
            DeleteFile(rar_exe_path);
            DeleteFile(release_path);

            if(strcmp(temp_path, curdir))
                RemoveDirectoryA(temp_path);
            if(pack_success)
            {
                ClearAllNfoInfo(GetParent(hwndDlg));
                SendMessageA(hwndDlg, WM_CLOSE, 0, 0);
            }
        }
        return TRUE;

        case IDC_BTN_REMOVE:
        {
            SetFocus(0);
            char filename[256]="";
            HWND list_hwnd=GetDlgItem(hwndDlg, IDC_LIST_FILES);
            int total=SendMessageA(list_hwnd, LB_GETCOUNT, 0, 0);
            if(!total)
                return TRUE;
            int cursel=SendMessageA(list_hwnd, LB_GETCURSEL, 0, 0);
            SendMessageA(list_hwnd, LB_GETTEXT, cursel, (LPARAM)filename);
            RemoveFileNameNoExtFromList(0, GetFileNameNoExt(filename));
            if(total>1)
                ParseFileList(hwndDlg, IDC_LIST_FILES);
            else
                SendMessageA(list_hwnd, LB_RESETCONTENT, 0, 0);
        }
        return TRUE;

        case IDC_CHK_ONTOP:
        {
            SetFocus(0);
            if(IsDlgButtonChecked(hwndDlg, LOWORD(wParam)))
            {
                SetWindowPos(hwndDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
            }
            else
            {
                SetWindowPos(hwndDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
            }
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}
