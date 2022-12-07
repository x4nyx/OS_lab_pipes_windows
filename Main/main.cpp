#include "windows.h"
#include <iostream>

using namespace std;

LPCSTR mPath = R"(D:\BSU\PROGA\OS_labs\PipesWin\M\cmake-build-debug\M.exe)";
LPCSTR aPath = R"(D:\BSU\PROGA\OS_labs\PipesWin\A\cmake-build-debug\A.exe)";
LPCSTR pPath = R"(D:\BSU\PROGA\OS_labs\PipesWin\P\cmake-build-debug\P.exe)";
LPCSTR sPath = R"(D:\BSU\PROGA\OS_labs\PipesWin\S\cmake-build-debug\S.exe)";

const unsigned int Size = 50;


int main() {
    PROCESS_INFORMATION pi1 = {0}, pi2 = {0}, pi3 = {0}, pi4 = {0};
    STARTUPINFO sa1 = {0}, sa2 = {0}, sa3 = {0}, sa4 = {0};

    HANDLE pipe1 = CreateNamedPipe(R"(\\.\pipe\$Pipe1$)",PIPE_ACCESS_OUTBOUND,
                                   PIPE_TYPE_BYTE,1,Size,
                                   Size,0,nullptr);

    if (pipe1 == nullptr || pipe1 == INVALID_HANDLE_VALUE) {
        wcout << "Error during pipe creating!";
        system("pause");
        return 1;
    }

    if (!CreateProcess(pPath, nullptr,
                       nullptr, nullptr, FALSE,
                       0, nullptr, nullptr,
                       &sa1, &pi1)){
        cerr << "Error during creating P process!\n";
        return 1;
    }
    ConnectNamedPipe(pipe1, nullptr);
    const char* data = "1 2 3 4 5 6 7";
    DWORD numBytesWritten = 0;
    if (!WriteFile(pipe1,data,strlen(data),
                   &numBytesWritten,nullptr)){
        cerr << "Error while writing!\n";
    }
    CloseHandle(pipe1);
    if (!CreateProcess(aPath, nullptr,
                       nullptr, nullptr, FALSE,
                       0, nullptr, nullptr,
                       &sa2, &pi2)){
        cerr << "Error during creating A process!\n";
        return 1;
    }
    if (!CreateProcess(mPath, nullptr,
                       nullptr, nullptr, FALSE,
                       0, nullptr, nullptr,
                       &sa3, &pi3)){
        cerr << "Error during creating M process!\n";
        return 1;
    }
    if (!CreateProcess(sPath, nullptr,
                       nullptr, nullptr, FALSE,
                       0, nullptr, nullptr,
                       &sa4, &pi4)){
        cerr << "Error during creating S process!\n";
        return 1;
    }
    HANDLE pipe5;
    while((pipe5 = CreateFile(
            R"(\\.\pipe\$Pipe5$)",
            PIPE_ACCESS_INBOUND,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    )) == INVALID_HANDLE_VALUE){
    }

    if (pipe5 == INVALID_HANDLE_VALUE) {
        wcout << "Failed connection to pipe!" << endl;
        system("pause");
        return 1;
    }

    char buffer[Size];
    DWORD numBytesRead = 0;
    if (!ReadFile(pipe5,buffer,Size,
                  &numBytesRead,nullptr)){
        cerr << "Error while reading!\n";
    }

    string res(buffer);

    cout << "Result: " << res << "\n";

    CloseHandle(pipe5);
    system("pause");
    return 0;
}