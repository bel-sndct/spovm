#include <iostream>
using namespace std;

extern char memory[20];
extern int number_of_pools;

class MemInfo {
public:
    void* free_mem_begin;
    int available_memory;

    MemInfo() {
        this->free_mem_begin = &memory;
        this->available_memory = sizeof(memory);
    }

    ~MemInfo() {
        this->free_mem_begin = nullptr;
        this->available_memory = 0;
    }
};

class MemoryPool {
public:
    void* beginning_segment;
    void* end_segment;
    int segment_size;
    bool is_available;

    MemoryPool() {
        this->beginning_segment = nullptr;
        this->end_segment = nullptr;
        this->segment_size = 0;
        is_available = false;
    }

    ~MemoryPool() {
        this->beginning_segment = nullptr;
        this->end_segment = nullptr;
        this->segment_size = 0;
    }
};

extern MemInfo zero_pool;
extern MemoryPool pools_array[20];

void* my_malloc(int);
void my_free(void*);
void defrag_memory();
void output_information();