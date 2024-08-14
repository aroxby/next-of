#include <iostream>
#include <string>
#include <windows.h>
#include <psapi.h>
using namespace std;

string getProcessName(DWORD processID) {
    char szProcessName[MAX_PATH] = "";

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, processID);

    if(hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(szProcessName[0]));
        } else {
            cerr << "Skipping errant modules for process: " << processID << endl;
        }
    } else {
        // Access denied, this is expected for some number of processes
    }

    return szProcessName;
}

int main(int argc, char *argv[]) {
    static constexpr unsigned max_processes  = 1024;

    DWORD aProcesses[max_processes], cbNeeded, cProcesses;
    if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        cerr << "Error: Failed to count processes!\n";
        return 5;
    }

    cProcesses = cbNeeded / sizeof(aProcesses[0]);
    if(cProcesses == max_processes) {
        cerr << "Error: too many processes running!\n";
        return 10;
    }

    for(unsigned i = 0; i < cProcesses; i++) {
        if(aProcesses[i] != 0) {
            string pname = getProcessName(aProcesses[i]);
            if(pname[0]) {
                cout << pname << endl;
            }
        }
    }

    return 0;
}
