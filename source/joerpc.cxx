#include "discord-rpc/include/discord_register.h"
#include "discord-rpc/include/discord_rpc.h"

#include <windows.h>
#include <TlHelp32.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

std::ostream& operator <<(std::ostream& stream, const std::vector<std::string>& vector) {
    stream << '{';
    for(std::size_t i=0; i<vector.size(); ++i) {
        if(i) stream << ", ";
        stream << vector.at(i);
    }
    stream << '}';
    return stream;
}

std::vector<std::string> SplitString(const std::string& string, const char token) {
    std::istringstream stream(string);
    std::vector<std::string> entries;
    for(std::string entry; std::getline(stream, entry, token); entries.emplace_back(entry));
    return entries;
}

void InitializeDiscord(const char* apiKey = "542490207795019794") {
    DiscordEventHandlers discord_event_handlers;
    memset(&discord_event_handlers, 0x00, sizeof(DiscordEventHandlers));
    Discord_Initialize(apiKey, &discord_event_handlers, 1, NULL);
}

uint16_t GetPidFromImage(const char* processImage) {
    uint16_t process_id = 0;
        
    void* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);        
    if(snapshot == INVALID_HANDLE_VALUE) return process_id;
    
    PROCESSENTRY32 process; process.dwSize = sizeof(process);
    
    if(!Process32First(snapshot, &process)) {
        return process_id; 
    }
    
    do {
        if(!_stricmp(process.szExeFile, processImage)) {
            process_id = (uint16_t)process.th32ProcessID;
            break;
        }
    } while(Process32Next(snapshot, &process)); 
    
    CloseHandle(snapshot); 
    
    return process_id;
}

void* GetHandleFromPid(uint16_t processId) {
    void* process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
    return process_handle;
}

uint64_t GetModuleAddress(const char* module, uint16_t processId) {
    uint64_t module_address = NULL;
    void* snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if(snapshot_handle == INVALID_HANDLE_VALUE) return 1;

    MODULEENTRY32 module_entrypoint;

    module_entrypoint.dwSize = sizeof(module_entrypoint);

    if(Module32First(snapshot_handle, &module_entrypoint)) {
        do {
            if(!_stricmp(module_entrypoint.szModule, module)) {
                module_address = reinterpret_cast<uint64_t>(module_entrypoint.modBaseAddr);
                break;
            }
        } while(Module32Next(snapshot_handle, &module_entrypoint));

        CloseHandle(snapshot_handle);
    } 

    return module_address;
}

int main() {
    InitializeDiscord();

    ShowWindow(GetConsoleWindow(), SW_HIDE);

    std::time_t time_acquired = NULL;

    for(;;) {
        Sleep(2000);

        uint16_t joe_process_id = GetPidFromImage("Joe.exe");
        void* joe_process_handle = nullptr;

        if(joe_process_id == 0) {
            std::cout << "Waiting for Joe.exe ..." << std::endl;
            std::time_t time_acquired = NULL;
            Discord_ClearPresence();
            continue;
        }

        joe_process_handle = GetHandleFromPid(joe_process_id);

        if(joe_process_handle == nullptr) {
            std::cout << "Waiting for non-null handle." << std::endl;
            std::time_t time_acquired = NULL;
            Discord_ClearPresence();
            continue;
        }

        uint64_t joewin_module_address = GetModuleAddress("joewin.dll", joe_process_id);
        
        if(joewin_module_address == NULL) {
            std::cout << "Waiting for a valid joewin.dll module address." << std::endl;
            std::time_t time_acquired = NULL;
            Discord_ClearPresence();
            continue;
        }

        if(!time_acquired) time_acquired = std::time(nullptr);

        uint64_t joewin_string_address = joewin_module_address + 0xCBA08;
        std::vector<char> characters;

        char last_char = 0x00;
        uint64_t offset = 0;

        do {
            uint64_t char_address = joewin_string_address + offset++;

            ReadProcessMemory(
                joe_process_handle, 
                reinterpret_cast<void*>(char_address), 
                &last_char, 
                sizeof(last_char), 
                0
            );

            if(last_char != 0x00) characters.push_back(last_char);
        } while(last_char != 0x00);

        std::string joewin_string(characters.begin(), characters.end());
        if(!joewin_string.size()) joewin_string = "Untitled Buffer";

        std::vector<std::string> joewin_string_split = SplitString(joewin_string, '.');
        std::string file_extension = joewin_string_split.back();
        
        const static std::vector<std::pair<std::string, std::vector<std::string>>> extension_resolver {
            {"C++",                 {"cpp", "cxx", "cc", "hpp", "hxx", "hh"}},
            {"C",                   {"c", "h"}},
            {"Python",              {"py", "pyc"}},
            {"Java",                {"class", "java", "jar"}},
            {"Ruby",                {"rb"}},
            {"Makefile",            {"makefile"}},
            {"Hexadecimal",         {"hex"}},
            {"Lua",                 {"lua"}},
            {"Markdown",            {"md"}},
            {"Haskell",             {"hs"}},
            {"Gitignore",           {"gitignore"}},
            {"C#",                  {"cs"}},
            {"Rust",                {"rs", "rust"}},
            {"JSON",                {"json"}},
            {"Binary",              {"bin"}},
            {"XML",                 {"xml"}},
            {"Powershell Script",   {"ps", "powershell"}},
            {"Command Script",      {"cmd", "bat"}},
            {"Plain Text",          {"txt"}}
        };

        const static std::vector<std::pair<std::string, std::vector<std::string>>> asset_resolver {
            {"cpp3",                {"cpp", "cxx", "cc", "hpp", "hxx", "hh"}},
            {"cicon2",              {"c", "h"}},
            {"python",              {"py", "pyc"}},
            {"java",                {"class", "java", "jar"}},
            {"ruby",                {"rb"}},
            {"make",                {"makefile"}},
            {"hex",                 {"hex"}},
            {"lua",                 {"lua"}},
            {"markdown",            {"md"}},
            {"haskell",             {"hs"}},
            {"git",                 {"gitignore"}},
            {"csharp",              {"cs"}},
            {"rust",                {"rs", "rust"}},
            {"json",                {"json"}},
            {"bin",                 {"bin"}},
            {"xml",                 {"xml"}},
            {"ps",                  {"ps", "powershell"}},
            {"ps",                  {"cmd", "bat"}},
            {"plain",               {"txt"}}
        };

        std::string file_language;
        for(const auto& pair : extension_resolver) {
            if(std::find(pair.second.begin(), pair.second.end(), file_extension) != pair.second.end()) {
                file_language = pair.first;
            }
        }

        std::string file_asset;
        for(const auto& pair : asset_resolver) {
            if(std::find(pair.second.begin(), pair.second.end(), file_extension) != pair.second.end()) {
                file_asset = pair.first;
            }
        }

        if(!file_language.size()) file_language = "Unknown";
        if(!file_asset.size()) file_asset = "joe3";

        DiscordRichPresence discord_rich_presence;
        memset(&discord_rich_presence, 0x00, sizeof(DiscordRichPresence));
        
        std::string state = "Editing in ";
        state += file_language;

        std::string details = "Editing ";
        details += joewin_string;
        
        discord_rich_presence.state = state.c_str();
        discord_rich_presence.details = details.c_str();
        discord_rich_presence.startTimestamp = time_acquired;

        discord_rich_presence.largeImageKey = file_asset.c_str();
        discord_rich_presence.largeImageText = file_language.c_str();

        discord_rich_presence.smallImageKey = "joe3";
        discord_rich_presence.smallImageText = "J.O.E Editor";

        std::cout << "Transmitting {" << std::endl;
        std::cout << "  State:" << state << std::endl;
        std::cout << "  Details: " << details << std::endl;
        std::cout << "  Timestamp: " << time_acquired << std::endl;
        std::cout << "  LargeKey: " << file_asset << std::endl;
        std::cout << "  LargeText: " << file_language << std::endl;
        std::cout << "  SmallKey: " << "joe3"  << std::endl;
        std::cout << "  SmallText: " << "J.O.E Editor" << std::endl;
        std::cout << "}" << std::endl << std::endl;

        Discord_UpdatePresence(&discord_rich_presence);
    }

    return 0;
}