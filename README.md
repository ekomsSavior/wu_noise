# wu_noise

Windows Update noise generator for operational security. Creates legitimate Windows Update activity to obscure malicious actions in event logs.

## How It Works

The tool triggers Windows Update through Microsoft's official UsoClient utility. This generates hundreds of legitimate events across System, Security, and WindowsUpdateClient logs. The flood of noise makes it difficult for incident responders to distinguish malicious activity from normal system behavior.

## Features

- Silent execution with no console window during payload launch
- Triggers Update scan, download, and installation sequentially
- Multiple fallback methods for URL opening
- Lightweight - single executable around 60KB
- No external dependencies

## Build Instructions

### Prerequisites

- Visual Studio 2022 with C/C++ development tools
- Windows SDK

### Compilation

Open Developer Command Prompt for VS 2022 as Administrator, then run:

```cmd
cl /MT /O1 /GS- /Fe:wu_noise.exe wu_noise.c /link user32.lib shell32.lib advapi32.lib
```

### Compiler Flags Explained

| Flag | Purpose |
|------|---------|
| /MT  | Static link to C runtime (no external DLLs) |
| /O1  | Optimize for small binary size |
| /GS- | Disable security checks (not needed for this tool) |
| /Fe  | Output executable name |

## Usage

Run the executable from an elevated command prompt:

```cmd
wu_noise.exe
```

The tool will:
1. Trigger Windows Update scan
2. Wait 3 seconds
3. Trigger Windows Update download
4. Wait 5 seconds
5. Trigger Windows Update installation
6. Wait 8 seconds
7. Launch the default browser to churchofmalware.org

## Customizing the Payload

Edit the `launch_payload()` function in `wu_noise.c` to run any executable or command:

```c
void launch_payload(void) {
    // Example: Run GreenPlasma.exe from the same directory
    run_silent(L"GreenPlasma.exe");
    
    // Example: Run a PowerShell script
    // run_silent(L"powershell.exe -WindowStyle Hidden -File C:\\path\\to\\script.ps1");
    
    // Example: Run a custom command
    // run_silent(L"cmd.exe /c whoami > C:\\temp\\output.txt");
}
```

Recompile after making changes.

## Stealth Mode

To hide the console window completely, compile with the Windows subsystem flag:

```cmd
cl /MT /O1 /GS- /Fe:wu_noise.exe wu_noise.c /link user32.lib shell32.lib advapi32.lib /SUBSYSTEM:WINDOWS
```

The executable will run with no visible window.

## Verification

Check Event Viewer after running wu_noise:

1. Open Event Viewer (Win + X -> Event Viewer)
2. Navigate to Windows Logs -> System
3. Look for events from source Microsoft-Windows-WindowsUpdateClient
4. Event IDs 1, 2, 3, 4, 19, 20, 21 confirm successful trigger

## Use Cases

- Pre-execution noise generation for red team operations
- Testing blue team log analysis capabilities
- Evaluating EDR telemetry volume handling
- Forensic analysis training for noisy environments


## Legal

This software is provided for educational and authorized security testing purposes only. 
