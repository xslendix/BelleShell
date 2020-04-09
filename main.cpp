#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <wtsapi32.h>

int main()
{
    std::cout << "BelleShell has started!" << std::endl;

    std::cout << "Getting all processes and removing window decoration..." << std::endl;

    WTS_PROCESS_INFO* pWPIs = NULL;
    DWORD dwProcCount = 0;
    if(WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
    {
        //Go through all processes retrieved
        for(DWORD i = 0; i < dwProcCount; i++)
        {
            //pWPIs[i].pProcessName = process file name only, no path!
            //pWPIs[i].ProcessId = process ID
            //pWPIs[i].SessionId = session ID, if you need to limit it to the logged in user processes
            //pWPIs[i].pUserSid = user SID that started the process

            if (GetGUIThreadInfo(pWPIs[i].ProcessId, 0)) {
                // Process has a window. Print process name.
                std::cout << "Process found! ID: " << pWPIs[i].ProcessId << " Name: " << pWPIs[i].pProcessName;
            }
        }
    }

    //Free memory
    if(pWPIs)
    {
        WTSFreeMemory(pWPIs);
        pWPIs = NULL;
    }

    return 0;
}
