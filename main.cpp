#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <psapi.h>
using namespace std;

string getProcessName(DWORD processID) {
    char szProcessName[MAX_PATH] = "";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processID);

    if(hProcess) {
        DWORD buffer_length = sizeof(szProcessName) / sizeof(szProcessName[0]);
        if(!QueryFullProcessImageName(hProcess, 0, szProcessName, &buffer_length)) {
            cerr << "Skipping errant process: " << processID << endl;
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

bool stringContains(const string &super, const string &sub) {
    auto it = search(super.begin(), super.end(), sub.begin(), sub.end(), [](char a, char b) {
        return tolower(a) == tolower(b);
    });
    return it != super.end();
}

bool start(char *cmdline) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD flags = CREATE_NEW_CONSOLE;   // Sharing a console doesn't share STDIN but does share STDOUT?

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);

    BOOL ret = CreateProcess(nullptr, cmdline, nullptr, nullptr, FALSE, flags, nullptr, nullptr, &si, &pi);
    if(!ret) {
        cerr << "Process creation error: " << GetLastError() << endl;
        return false;
    } else {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return true;
    }
}

int main(int argc, char *argv[]) {
    vector<bool> running(argc - 1, false);

    auto processes = getAllProcessIds();
    if(processes.empty()) {
        return 5;
    }

    for(auto processId : processes) {
        string pname = getProcessName(processId);
        if(pname[0]) {
            for(int i = 1; i < argc; i++) {
                if(stringContains(argv[i], pname)) {
                    running[i] = true;
                }
            }
        }
    }

    // Note: might be possible find the program file without a fully qualified path
    // Use CommandLineToArgvW to get the program portion of a command line
    // Use regular file commands to search relative paths for the program
    // Use PathFindFileNameA to search %PATH% for the program
    for(int i = 1; i < argc; i++) {
        cout << running[i] << " : " << argv[i] << endl;
        if(!running[i]) {
            start(argv[i]);
            break;
        }
    }

    return 0;
}
