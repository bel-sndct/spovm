#include "library.h"
#define ARRAY_PID_LEN 10
using namespace std;

int childProcess();

int main(int argc, char* argv[]) {

    initscr();

    int arrayProcessesPids[ARRAY_PID_LEN] = {0};
    int currentLen = 1;
    int status = 0;

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    arrayProcessesPids[0] = getpid();
    printw("\nMain process pid - %d\n\r", arrayProcessesPids[0]);
    printw("--> Press '+' to create new process\n\r");
    printw("--> Press '-' to remove last created process\n\r");
    printw("--> Press 'q' to exit from program\n\r");
    refresh();

    for (char ch = 0; ch != 'q';) {
        //keyboard feedback
        fflush(stdin);
        ch = getch();
        switch (ch) {
            case '+': {
                pid_t pid;
                if (currentLen < ARRAY_PID_LEN) {
                    currentLen++;
                    pid = fork();
                }
                else {
                    cout << "\tMaximum number of processes has been reached!\n\r";
                    break;
                }
                //child-process processing
                switch (pid) {
                    case -1: {
                        cout << "Error creating child process" << "\n\r" << endl;
                        exit(-1);
                    }
                    case 0: {
                        childProcess();
                        exit(0);
                    }
                    default: {
                        arrayProcessesPids[currentLen - 1] = pid;
                        break;
                    }
                }
                break;
            }
            case '-': {
                if (currentLen == 1) {
                    cout << "\tThere are no processes available for deletion!" << "\n\r" << endl;
                    break;
                }
                else {
                    currentLen--;
                    kill(arrayProcessesPids[currentLen], SIGKILL);
                    arrayProcessesPids[currentLen] = 0;
                }
                break;
            }
            case 'q': {
                for (int i = currentLen - 1; i >= 0; i--) {
                    kill(arrayProcessesPids[i], SIGKILL);
                }
                break;
            }
        }
        //output processes to sreen
        cout << "\r-----------------------------" << endl;
        char str[] = "\n\rActive processes:";
        for (int i = 0; i < strlen(str); i++) {
            printf("%c", str[i]);
            fflush(stdout);
            usleep(50000);
        }
        cout << currentLen << "\n\r" << endl;
        for (int i = 1; i < currentLen; i++) {
            kill(arrayProcessesPids[i], SIGUSR1);
            sigwait(&mask, &status); 
        }
        sleep(1);
    }
    endwin();
    return 0;
}

int childProcess() {
    int status = 0;
    sigset_t childmask;
    sigemptyset(&childmask);
    sigaddset(&childmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &childmask, NULL);

    char childOutput[] = "Child process PID - ";
    int lenght = strlen(childOutput);

    while (true) {
        sigwait(&childmask, &status);
        cout << "\r" << endl;
        for (int i = 0; i < lenght; i++) {
            printf("%c", childOutput[i]);
            fflush(stdout);
            usleep(50000);
        }
        cout << getpid()  << "\n\r";
        kill(getppid(), SIGUSR2);
    }

    return 0;
}