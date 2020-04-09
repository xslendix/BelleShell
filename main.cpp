#include "main.hpp"
#include "admintools.hpp"

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>

#include <time.h>

#include <windows.h>
#include <winuser.h>
#include <wtsapi32.h>
#include <tchar.h>
#include <psapi.h>

std::time_t tt;
std::tm tmm;

void PrintProcessNameAndID( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    TCHAR unknownString[] = TEXT("<unknown>");

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.
    if (_tcscmp(szProcessName, unknownString) == 1) {
        std::cout << '[' << std::put_time(&tmm, "%H:%M:%S") << "] - " << szProcessName << "  (PID: " << processID << ")" << std::endl;
    }

    // Release the handle to the process.
    CloseHandle( hProcess );
}

int Main()
{
    std::cout << '[' << std::put_time(&tmm, "%H:%M:%S") << "] :: BelleShell has started!" << std::endl;

    std::cout << '[' << std::put_time(&tmm, "%H:%M:%S") << "] :: Getting all processes and removing window decoration..." << std::endl;

    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 && aProcesses[i] != GetCurrentProcessId() )
        {
            PrintProcessNameAndID( aProcesses[i] );
        }
    }

    return 0;
}

int main()
{
    int ret = 0;

    freopen( "log.txt", "w", stdout );
    tt = std::time(nullptr);
    tmm = *std::localtime(&tt);

    clock_t t1,t2;
    t1=clock();

    if(IsRunAsAdministrator())
    {
        ret = Main();
    } else
    {
        /*if(MessageBox(0,"Need To Elevate","Critical Disk Error",MB_SYSTEMMODAL|MB_ICONERROR|MB_YESNO) == IDYES)
        {
            ElevateNow();
        } else
        {*/
            MessageBox(0,"This program requires administrator to run!","System Critical Error",MB_SYSTEMMODAL|MB_OK|MB_ICONERROR);
        /*    ElevateNow();
        }*/
        //return ElevateNow();
    }
    t2=clock();
    float diff ((float)t2-(float)t1);
    std::cout << '[' << std::put_time(&tmm, "%H:%M:%S") << "] :: BelleShell ran for " << diff << " miliseconds." << std::endl;

    return ret;
}
