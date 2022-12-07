#include "windows.h"
#include <iostream>
#include "string"

using namespace std;

const unsigned int Size = 50;


string parseString(string input){
    string output;
    while (!input.empty()) {
        auto end = find(input.begin(), input.end(), ' ');
        string tmp = input.substr(0, end - input.begin());
        if (end == input.end()) break;
        end++;
        input.erase(input.begin(), end);

        int num = stoi(tmp, nullptr, 10);
        output.append(to_string(num + 23) + " ");
    }
    return output;
}


int main(){

    HANDLE pipe2;
    while((pipe2 = CreateFile(R"(\\.\pipe\$Pipe2$)",PIPE_ACCESS_INBOUND,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr,
                              OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr))
          == INVALID_HANDLE_VALUE){
    }

    if (pipe2 == INVALID_HANDLE_VALUE) {
        wcout << "Failed during connection to pipe." << endl;
        system("pause");
        return 1;
    }

    char buffer[Size];
    if (!ReadFile(pipe2,buffer,Size,
                  nullptr,nullptr)) {
        cerr << "Read failed!\n";
    }

    string str(buffer);
    string input = parseString(str);

    HANDLE pipe3 = CreateNamedPipe(
            R"(\\.\pipe\$Pipe3$)",PIPE_ACCESS_OUTBOUND,PIPE_TYPE_BYTE,
            1,Size,Size,0,nullptr);

    if (pipe3 == nullptr || pipe3 == INVALID_HANDLE_VALUE) {
        wcout << "Failed to create outbound pipe instance.\n";
        system("pause");
        return 1;
    }

    ConnectNamedPipe(pipe3, nullptr);

    if (!WriteFile(pipe3, input.c_str(), strlen(buffer),
                   nullptr, nullptr)){
        cerr << "Writing failed!\n";
    }

    CloseHandle(pipe2);
    CloseHandle(pipe3);

    system("pause");
    return 0;
}