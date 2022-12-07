#include "windows.h"
#include <iostream>
#include "string"

using namespace std;

const unsigned int Size = 50;


string parse(string input){
    string output;
    while (!input.empty()) {
        auto end = find(input.begin(), input.end(), ' ');
        string tmp = input.substr(0, end - input.begin());
        if (end == input.end()) break;
        end++;
        input.erase(input.begin(), end);

        int num = stoi(tmp, nullptr, 10);
        output.append(to_string(num * 7) + " ");
    }
    return output;
}


int main(){

    HANDLE pipe3;
    while ((pipe3 = CreateFile(R"(\\.\pipe\$Pipe3$)",PIPE_ACCESS_INBOUND,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr,
                               OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr))
           == INVALID_HANDLE_VALUE){
    }

    if (pipe3 == INVALID_HANDLE_VALUE) {
        wcout << "Failed while connection to pipe." << endl;
        return 1;
    }

    char buffer[Size];
    DWORD numBytesRead = 0;
    if (!ReadFile(pipe3,buffer,Size,
                  &numBytesRead,nullptr)){
        cout << "Reading failed!\n";
    }

    string str(buffer);
    string input = parse(str);

    HANDLE pipe4 = CreateNamedPipe(
            R"(\\.\pipe\$Pipe4$)",PIPE_ACCESS_OUTBOUND,PIPE_TYPE_BYTE,
            1,80,80,0,nullptr);

    if (pipe4 == nullptr || pipe4 == INVALID_HANDLE_VALUE) {
        wcout << "Failed to create outbound pipe instance.\n";
        system("pause");
        return 1;
    }

    ConnectNamedPipe(pipe4, nullptr);

    DWORD numBytesWritten = 0;

    if (!WriteFile(pipe4, input.c_str(), 80,
                   &numBytesWritten, nullptr)){
        cout << "Writing failed!\n";
    }

    CloseHandle(pipe3);
    CloseHandle(pipe4);
    system("pause");
    return 0;
}