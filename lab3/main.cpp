#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
using namespace std;

void clientProcess(int _pipedes[]);

int main(int argc, char* argv[]) {
    int pipedes[2] = {0};
    bool flag = true;
    pid_t childPid;
    string message;
    int callback;
    //
    int status = 0;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    //
    pipe(pipedes);
    childPid = fork();
    while (flag == true) {
        switch (childPid) {
            case -1: {
                cout << "Creating process error!\n";
                flag = false;
                break;
            }
            case 0: {
                clientProcess(pipedes);
                break;
            }
            default: {
                close(pipedes[0]);

                while (true) {                
                    callback = 0;
                    while (callback < 1 || callback > 2) {
                        cout << "1 -> Send message\n"
                            << "2 -> Exit\n";
                        cin >> callback;
                        if (callback == 1) {
                            cout << "Enter string: ";
                            cin.get();
                            fflush(stdin);
                            getline(cin, message);

                            write(pipedes[1], (void*)message.c_str(), message.size() + 1);

                            kill(childPid, SIGUSR1);
                            sigwait(&mask, &status);
                        }
                        if (callback == 2) {
                            close(pipedes[0]);
                            kill(childPid, SIGTERM);
                            kill(getpid(), SIGTERM);
                            exit(0);
                            flag = false;
                        }
                    }
                }

                break;
            }
        }
    }
    return 0;
}

void clientProcess(int _pipedes[]) {
    //
    int status = 0;
    sigset_t cmask;
    sigemptyset(&cmask);
    sigaddset(&cmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &cmask, NULL);
    //
    char input_message[100] = {0};
    int lenght;
    close(_pipedes[1]);
    while (true) {
        sigwait(&cmask, &status);
        lenght = read(_pipedes[0], input_message, 100);
        cout << "Child " << getpid() <<  " wrote:\n";
        for (int i = 0; i < lenght; i++) {
            fflush(stdout);
            cout << input_message[i];
            usleep(20000);
        }
        cout << "\n";
        kill(getppid(), SIGUSR2);
    }
}