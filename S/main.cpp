#include "windows.h"
#include <iostream>
#include "string"

using namespace std;

const unsigned int Size = 80;

string parse(string input){
    int sum = 0;
    while (!input.empty()) {
        auto end = find(input.begin(), input.end(), ' ');
        string tmp = input.substr(0, end - input.begin());
        if (end == input.end()) break;
        end++;
        input.erase(input.begin(), end);

        int num = stoi(tmp, nullptr, 10);
        sum += num;
    }
    return to_string(sum);
}


int main(){
    HANDLE pipe4;
    while ((pipe4 = CreateFile(R"(\\.\pipe\$Pipe4$)",PIPE_ACCESS_INBOUND,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr,
                               OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr))
           == INVALID_HANDLE_VALUE){
    }

    if (pipe4 == INVALID_HANDLE_VALUE) {
        wcout << "Failed during connection to pipe." << endl;
        system("pause");
        return 1;
    }

    char buffer[Size];
    DWORD numBytesRead = 0;

    if (!ReadFile(pipe4, buffer, Size,
                  &numBytesRead, nullptr)) {
        cerr << "Reading failed!\n";
    }

    string str(buffer);
    string input = parse(str);

    HANDLE pipe5 = CreateNamedPipe(
            R"(\\.\pipe\$Pipe5$)",PIPE_ACCESS_OUTBOUND,PIPE_TYPE_BYTE,
            1,Size,Size,0,nullptr);

    if (pipe5 == nullptr || pipe5 == INVALID_HANDLE_VALUE) {
        wcout << "Failed during creation outbound pipe instance.\n";
        system("pause");
        return 1;
    }

    ConnectNamedPipe(pipe5, nullptr);

    DWORD numBytesWritten = 0;

    if (!WriteFile(pipe5, input.c_str(), strlen(buffer),
                   &numBytesWritten, nullptr)){
        cerr << "Writing failed!\n";
    }

    CloseHandle(pipe4);
    CloseHandle(pipe5);

    system("pause");
    return 0;
}