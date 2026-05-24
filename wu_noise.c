#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")

void run_silent(const wchar_t *cmd) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = {0};
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    wchar_t cmdLine[512];
    wcscpy(cmdLine, cmd);
    
    CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE,
                   CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void open_url(const wchar_t *url) {
    // Method 1: Use ShellExecuteW (most reliable)
    HINSTANCE result = ShellExecuteW(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
    
    if ((INT_PTR)result <= 32) {
        // Method 2: Try with cmd.exe /c start
        wchar_t cmd[512];
        wsprintfW(cmd, L"cmd.exe /c start %s", url);
        run_silent(cmd);
        Sleep(1000);
        
        // Method 3: Try direct chrome.exe from registry
        wchar_t chromePath[MAX_PATH] = {0};
        HKEY hKey;
        DWORD size = MAX_PATH;
        
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe",
            0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            RegQueryValueExW(hKey, NULL, NULL, NULL, (LPBYTE)chromePath, &size);
            RegCloseKey(hKey);
            
            if (chromePath[0] != 0) {
                wchar_t chromeCmd[512];
                wsprintfW(chromeCmd, L"\"%s\" %s", chromePath, url);
                run_silent(chromeCmd);
            }
        }
    }
}

void launch_payload(void) {
    printf("[*] Opening churchofmalware.org...\n");
    open_url(L"https://churchofmalware.org");
}

int main(void) {
    printf("========================================\n");
    printf("WU-NOISE // Windows Update Cover Generator\n");
    printf("Church of Malware\n");
    printf("========================================\n\n");
    
    printf("[*] Triggering Windows Update scan...\n");
    run_silent(L"UsoClient StartScan");
    Sleep(3000);
    
    printf("[*] Triggering Windows Update download...\n");
    run_silent(L"UsoClient StartDownload");
    Sleep(5000);
    
    printf("[*] Triggering Windows Update installation...\n");
    run_silent(L"UsoClient StartInstall");
    Sleep(8000);
    
    printf("[*] Windows Update noise complete.\n");
    printf("[*] Launching payload...\n");
    launch_payload();
    
    printf("[+] Done. Browser should open shortly.\n");
    return 0;
}
