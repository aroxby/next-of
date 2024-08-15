#include <iostream>
#include <string>
#include <vector>
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

vector<DWORD> getAllProcessIds() {
    // TODO: Would be nice to call EnumProcesses repeatedly and dynamically allocate the array
    // but the process count can change between calls
    static constexpr unsigned int max_processes  = 1024;

    vector<DWORD> output;

    DWORD aProcesses[max_processes], cbNeeded;
    if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        cerr << "Error: Failed to count processes!\n";
        return output;
    }

    unsigned int cProcesses = cbNeeded / sizeof(aProcesses[0]);
    if(cProcesses == max_processes) {
        cerr << "Error: too many processes running!\n";
        return output;
    }

    output = {aProcesses, aProcesses + cProcesses};
    return output;
}

int main(int argc, char *argv[]) {
    auto processes = getAllProcessIds();
    if(processes.empty()) {
        return 5;
    }

    for(auto processId : processes) {
        string pname = getProcessName(processId);
        if(pname[0]) {
            cout << pname << endl;
        }
    }

    return 0;
}
