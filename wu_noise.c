#include <windows.h>
#include <stdio.h>

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

void launch_payload(void) {
    wchar_t chromePath[MAX_PATH] = {0};
    HKEY hKey;
    DWORD size = MAX_PATH;
    
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueExW(hKey, NULL, NULL, NULL, (LPBYTE)chromePath, &size);
        RegCloseKey(hKey);
    }
    
    if (chromePath[0] != 0) {
        wchar_t cmd[512];
        wsprintfW(cmd, L"\"%s\" https://churchofmalware.org", chromePath);
        run_silent(cmd);
        printf("[+] Chrome launched to churchofmalware.org\n");
    } else {
        run_silent(L"cmd.exe /c start https://churchofmalware.org");
        printf("[+] Default browser launched to churchofmalware.org\n");
    }
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
    
    printf("[+] Done.\n");
    return 0;
}
