#define WINVER 0x0501
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <wininet.h>
#include <ctype.h>
#include "resource.h"
#include "functions/format.h"
#include "functions/packrelease.h"
#include "functions/nfoviewer.h"
#include "functions/namegen.h"
#include "functions/settings.h"
#include "functions/nfocreator.h"

char* File2Char(const char* filename)
{
    HANDLE hFile;
    hFile=CreateFileA(filename, GENERIC_ALL, 0, 0, OPEN_EXISTING, 0, 0);
    if(hFile==INVALID_HANDLE_VALUE)
        return 0;
    unsigned int filesize=GetFileSize(hFile, 0);
    char* result=(char*)VirtualAlloc(0, filesize, MEM_COMMIT, PAGE_READWRITE);
    if(!result)
        return 0;
    DWORD readbytes=0;
    if(!ReadFile(hFile, result, filesize, &readbytes, 0))
    {
        CloseHandle(hFile);
        return 0;
    }
    CloseHandle(hFile);
    return result;
}

int RetrieveServerVersion(char* return_buffer, int buffer_size)
{
    HINTERNET connect=InternetOpen("TPoDT", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
    if(!connect)
        return 0;
    HINTERNET OpenAddress=InternetOpenUrl(connect, "http://tpodt.net16.net/tpodt_nfo_version.php", 0, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_KEEP_CONNECTION, 0);
    if(!OpenAddress)
    {
        OpenAddress=InternetOpenUrl(connect, "http://tpodt.com/tpodt_nfo_version.php", 0, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_KEEP_CONNECTION, 0);
        if(!OpenAddress)
        {
            InternetCloseHandle(connect);
            return 0;
        }
    }
    DWORD NumberOfBytesRead=0;
    InternetReadFile(OpenAddress, return_buffer, buffer_size, &NumberOfBytesRead);
    InternetCloseHandle(OpenAddress);
    InternetCloseHandle(connect);
    return NumberOfBytesRead;
}

DWORD WINAPI VersionCheckThread(LPVOID lpParam)
{
    Sleep(500);
    char serverversion[100]="";
    if(RetrieveServerVersion(serverversion, 100))
    {
        if(strcmp(serverversion, ClientVersion))
            MessageBoxA(hwndMainDlg, "The server is running a different version, please update...", "Notice", MB_ICONINFORMATION|MB_SETFOREGROUND);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    LoadLibrary("riched20.dll");
    if(lpCmdLine[0])
    {
        HANDLE hFile=CreateFileA(lpCmdLine, GENERIC_ALL, 0, 0, OPEN_EXISTING, 0, 0);
        if(hFile!=INVALID_HANDLE_VALUE)
        {
            int filesize=GetFileSize(hFile, 0);
            DWORD read=0;
            nfo_viewer_text=(char*)malloc(filesize+3);
            memset(nfo_viewer_text, 0, filesize+3);
            ReadFile(hFile, nfo_viewer_text, filesize+3, &read, 0);
            strcat(nfo_viewer_text, "\r\n");
            CloseHandle(hFile);
            int retn=1;
            if(nfo_viewer_text[0])
                retn=DialogBox(hInst, MAKEINTRESOURCE(DLG_NFOVIEWER), NULL, (DLGPROC)DlgNfoViewer);
            free(nfo_viewer_text);
            return retn;
        }
    }
    int i=GetModuleFileNameA(GetModuleHandle(0), ini_file, 256);
    while(ini_file[i]!='\\')
        i--;
    ini_file[i]=0;
    SetCurrentDirectoryA(ini_file);
    ExtractResource(hInst, "NFO_FORMAT", "DATA", "nfo_format.dat");
    ExtractResource(hInst, "DIZ_FORMAT", "DATA", "diz_format.dat");
    ExtractResource(hInst, "TEAM_INFO", "DATA", "team_info.dat");
    //CreateThread(0, 0, VersionCheckThread, 0, 0, 0); //version check (server is down, so it doesn't work anymore)
    strcat(ini_file, "\\settings.ini");
    GetPrivateProfileStringA("Defaults", "cracker", "", default_cracker, 50, ini_file);
    GetPrivateProfileStringA("Defaults", "platform", "", default_platform, 50, ini_file);
    nfo_format=File2Char("nfo_format.dat");
    if(!nfo_format)
    {
        MessageBoxA(0, "Could not load nfo_format.dat, make sure the file is not in use and exsists...", "Error!", MB_ICONERROR);
        return 1;
    }
    team_info=File2Char("team_info.dat");
    if(!team_info)
    {
        MessageBoxA(0, "Could not load team_info.dat, make sure the file is not in use and exsists...", "Error!", MB_ICONERROR);
        return 1;
    }
    diz_format=File2Char("diz_format.dat");
    if(!diz_format)
    {
        MessageBoxA(0, "Could not load diz_format.dat, make sure the file is not in use and exsists...", "Error!", MB_ICONERROR);
        return 1;
    }
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
