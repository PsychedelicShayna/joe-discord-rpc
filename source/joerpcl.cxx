#include <windows.h>
#include <TlHelp32.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

std::vector<std::string> SplitString(const std::string& string, const char token) {
    std::istringstream stream(string);
    std::vector<std::string> entries;
    for(std::string entry; std::getline(stream, entry, token); entries.emplace_back(entry));
    return entries;
}

int main(int argc, char** argv) {
    std::stringstream joe_launch_command_stream;

    char module_filename[512];
    ZeroMemory(module_filename, 512);
    GetModuleFileNameA(NULL, module_filename, 512);

    std::vector<std::string> module_split = SplitString(module_filename, '\\');
    module_split.erase(module_split.end()-1);

    std::string module_path;
    for(std::size_t i=0; i<module_split.size(); ++i) {
        module_path += module_split.at(i);
        module_path += '\\';
    }
    
    joe_launch_command_stream << '"' << module_path << "Joe.exe" << '"';
    for(std::size_t i=1; i<argc; ++i) {
        joe_launch_command_stream << " " << argv[i];
    }

    const std::string joe_launch_command_string = joe_launch_command_stream.str();
    char* joe_launch_command = new char[joe_launch_command_string.size()];
    strcpy(joe_launch_command, joe_launch_command_string.c_str());

    STARTUPINFO joe_suinfo = {sizeof(STARTUPINFO)};
    STARTUPINFO rpc_suinfo = {sizeof(STARTUPINFO)};

    PROCESS_INFORMATION joe_prinfo;
    PROCESS_INFORMATION rpc_prinfo;

    ZeroMemory(&joe_suinfo, sizeof(STARTUPINFO));
    ZeroMemory(&rpc_suinfo, sizeof(STARTUPINFO));
    ZeroMemory(&joe_prinfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&rpc_prinfo, sizeof(PROCESS_INFORMATION));

    if(!CreateProcess(nullptr, joe_launch_command, NULL, NULL, false, 0, NULL, NULL, &joe_suinfo, &joe_prinfo)) {
        std::cout << "Failed to create process with launch command: " << joe_launch_command << std::endl;
    }
        
    if(!IsProcessRunning("joerpc.exe")) {
        std::stringstream rpc_launch_command_stream;
        rpc_launch_command_stream << '"' << module_path << "joerpc.exe" << '"';
        
        const std::string& rpc_launch_command_string = rpc_launch_command_stream.str();
        char* rpc_launch_command = const_cast<char*>(rpc_launch_command_string.c_str());

        if(!CreateProcess(nullptr, rpc_launch_command, NULL, NULL, false, DETACHED_PROCESS, NULL, NULL, &rpc_suinfo, &rpc_prinfo)) {
            std::cout << "Failed to create process with launch command: " << rpc_launch_command << std::endl;
        }
    }

    CloseHandle(&joe_suinfo);
    CloseHandle(&rpc_suinfo);
    CloseHandle(&joe_prinfo);
    CloseHandle(&rpc_prinfo);

    return 0;
}