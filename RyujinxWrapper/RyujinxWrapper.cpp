#include "pch.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char *argv[])
{
    char game[300];

    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD exitCode;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (argc > 1)
    {
        sprintf_s(game, "bin\\Ryujinx.exe \"%s\" -WRAPPED_BUILD", argv[1]);
    }
    else
    {
        sprintf_s(game, "bin\\Ryujinx.exe -WRAPPED_BUILD");
    }

    // start the program up
    bool result = CreateProcess(NULL,
                                game,  // Command line
                                NULL,  // Process handle not inheritable
                                NULL,  // Thread handle not inheritable
                                FALSE, // Set handle inheritance to FALSE
                                0,     // No creation flags
                                NULL,  // Use parent's environment block
                                NULL,  // Use parent's starting directory 
                                &si,   // Pointer to STARTUPINFO structure
                                &pi);  // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)

    if (!result)
    {
        // CreateProcess() failed
        // Get the error from the system
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

        // Display the error
        char* strError = (LPTSTR)lpMsgBuf;
        printf_s("Failed at CreateProcess()\nMessage=%s\nPath=%s\n\n", strError, game);

        // Free resources created by the system
        LocalFree(lpMsgBuf);

        // We failed.
        return FALSE;
    }
    else
    {
        // Successfully created the process.  Wait for it to finish.
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Get the exit code.
        result = GetExitCodeProcess(pi.hProcess, &exitCode);

        // Close the handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (!result)
        {
            // Could not get exit code.
            printf("Couldn't get exit code.\nCommand=%s\n", argv[0]);
            return FALSE;
        }


        // We succeeded.
        return TRUE;
    }
}
