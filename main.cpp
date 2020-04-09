#include "main.hpp"
#include "admintools.hpp"

#include "simpleini/SimpleIni.h"

#include <iostream>
#include <vector>
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

bool running = true;

// https://stackoverflow.com/a/48207646
void GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND> &vhWnds)
{
    // find all hWnds (vhWnds) associated with a process id (dwProcessID)
    HWND hCurWnd = NULL;
    do
    {
        hCurWnd = FindWindowEx(NULL, hCurWnd, NULL, NULL);
        DWORD dwProcessID = 0;
        GetWindowThreadProcessId(hCurWnd, &dwProcessID);
        if (dwProcessID == dwProcessID)
        {
            vhWnds.push_back(hCurWnd);
        }
    }
    while (hCurWnd != NULL);
}

std::vector <DWORD> pids;

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
        pids.push_back(processID);
        std::cout << '[' << std::put_time(&tmm, "%H:%M:%S") << "] - " << szProcessName << "  (PID: " << processID << ")" << std::endl;
    }

    // Release the handle to the process.
    CloseHandle( hProcess );
}

int Main()
{
    for (int i=0; i < 512; ++i) pids[i] = -1;

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
    std::vector <HWND> hwnds;
    for (DWORD pid : pids)
    {
        std::vector <HWND> temp;
        GetAllWindowsFromProcessID(pid, &temp);
        hwnds.insert(hwnds.end(), temp.begin(), temp.end());
    }

    while (running)
    {

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
    }

    t2=clock();
    float diff ((float)t2-(float)t1);
    std::cout << '[' << std::put_time(&tmm, "%H:%M:%S") << "] :: BelleShell ran for " << diff << " miliseconds." << std::endl;

    return ret;
}
