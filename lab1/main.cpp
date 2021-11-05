#include "library.h"


int main(int argc, char* argv[]) {

    initscr();
    curs_set(0);

    int* child_status;
    pid_t child_pid = fork();
    halfdelay(10);

    switch(child_pid) {
        case -1: {
            printw("Creating child process error!\n");
            refresh();
            rewind(stdin);
            getch();
            break;
        }
        case 0: {
            for (char ext = 0; ext != '\n'; ext = getch()) {
                time_t currenttime = time(NULL);
                struct tm* child_time = localtime(&currenttime);

                move(0, 0);
                printw("Current time by Child process: %s", asctime(child_time));
                printw("Child process pid: %d", getpid());
                refresh();

                rewind(stdin);
                napms(10);
            }
            break;
        }
        default: {
            while (true) {
                time_t currenttime = time(NULL);
                struct tm* parent_time = localtime(&currenttime);

                move(4, 4);
                printw("Current time by Parent process: %s", asctime(parent_time));
                printw("Parent child pid: %d", getpid());
                refresh();

                if (waitpid(child_pid, child_status, WNOHANG) > 0) break;
            }

            printw("Chiled exited with status of %d\n", WEXITSTATUS(*child_status));
            
            refresh();

            break;
        }
    }

    printw("Press any key to exit...");
    rewind(stdin);
    getch();
    endwin();

    return 0;
}