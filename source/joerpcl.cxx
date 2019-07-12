#include <windows.h>
#include <TlHelp32.h>

#include <iostream>
#include <sstream>
#include <string>

bool IsProcessRunning(const char* process_image) {
    void* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(snapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(snapshot, &process_entry)) return false;

    do {
        if(!_stricmp(process_entry.szExeFile, process_image)) return true;
    } while(Process32Next(snapshot, &process_entry));

    CloseHandle(snapshot);
    return false;
}

int main(int argc, char** argv) {
    std::stringstream launch_command_stream;
    launch_command_stream << "./Joe.exe";
    for(std::size_t i=1; i<argc; ++i) {
        launch_command_stream << " " << argv[i];
    }

    const std::string launch_command_string = launch_command_stream.str();
    char* launch_command = new char[launch_command_string.size()];
    strcpy(launch_command, launch_command_string.c_str());

    STARTUPINFO joe_suinfo = {sizeof(STARTUPINFO)};
    STARTUPINFO rpc_suinfo = {sizeof(STARTUPINFO)};

    PROCESS_INFORMATION joe_prinfo;
    PROCESS_INFORMATION rpc_prinfo;

    ZeroMemory(&joe_suinfo, sizeof(STARTUPINFO));
    ZeroMemory(&rpc_suinfo, sizeof(STARTUPINFO));
    ZeroMemory(&joe_prinfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&rpc_prinfo, sizeof(PROCESS_INFORMATION));

    std::cout << "Launch command: " << launch_command << std::endl;
    if(!CreateProcess(nullptr, launch_command, NULL, NULL, false, 0, NULL, NULL, &joe_suinfo, &joe_prinfo)) {
        std::cout << "Failed to create process with launch command: " << launch_command << std::endl;
    }
        
    if(!IsProcessRunning("joerpc.exe")) {
        char* joerpc_launch_command = const_cast<char*>("./joerpc.exe");
        if(!CreateProcess(nullptr, joerpc_launch_command, NULL, NULL, false, DETACHED_PROCESS, NULL, NULL, &joe_suinfo, &joe_prinfo)) {
            std::cout << "Failed to create process with launch command: " << launch_command << std::endl;
        }
    }

    CloseHandle(&joe_suinfo);
    CloseHandle(&rpc_suinfo);
    CloseHandle(&joe_prinfo);
    CloseHandle(&rpc_prinfo);

    return 0;
}