#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <vector>
#include <sys/wait.h>
#include <signal.h>
#include <condition_variable>

using namespace std;

pthread_mutex_t function_mutex;

void* common_function(void*);

int main() {
    vector<pthread_t> threads_vector;
    pthread_mutex_init(&function_mutex, NULL);

    cout << "Main process pid - " << getpid() << "\n"
         << "--> Press '+' to create new thread\n"
         << "--> Press '-' to remove last created thread\n"
         << "--> Press 'q' to exit from program\n";

    for (char ch = 0; ch != 'q';) {
        fflush(stdin);
        ch = getchar();
        switch (ch) {
            case '+': {
                cout << "Adding new thread\n";
                pthread_t new_thread;
                pthread_create(&new_thread, NULL, common_function, NULL);
                threads_vector.push_back(new_thread);
                break;
            }
            case '-': {
                if (threads_vector.size() != 0) {
                    pthread_cancel(threads_vector.back());
                    threads_vector.pop_back();
                    fflush(stdout);
                    cout << "Last created thread has deleted\n";
                }
                break;
            }
            case 'q': {
                while (threads_vector.size() != 0) {
                    pthread_cancel(threads_vector.back());
                    threads_vector.pop_back();
                }
                pthread_mutex_destroy(&function_mutex);
                break;
            }
            default: continue;
        }
    }

    return 0;
}

void* common_function(void*) {
    char out_str[] = "Thread identifier - ";
    short lenght = strlen(out_str);

    for (;;) {
        pthread_mutex_lock(&function_mutex);
        for (short i = 0; i < lenght; i++) {
            fflush(stdout);
            cout << out_str[i];
            usleep(1000);
        }
        cout << pthread_self() << "\n";
        pthread_mutex_unlock(&function_mutex);
        usleep(1500000);
    }
}