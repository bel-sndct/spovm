#include "header.h"

char memory[20];
int number_of_pools = 0;

MemInfo zero_pool;
MemoryPool pools_array[20];

//memory allocation
void* my_malloc(int _size_to_fill) {
    if (_size_to_fill <= zero_pool.available_memory) {
        pools_array[number_of_pools].beginning_segment = zero_pool.free_mem_begin;

        zero_pool.available_memory -= _size_to_fill;
        zero_pool.free_mem_begin = &memory[sizeof(memory) - zero_pool.available_memory];

        pools_array[number_of_pools].end_segment = &memory[sizeof(memory) - zero_pool.available_memory];
        pools_array[number_of_pools].segment_size = _size_to_fill;

        number_of_pools++;

        return pools_array[number_of_pools - 1].beginning_segment;
    }
    else {
        bool access_flag = false;
        int iter = 0;
        for (; iter < number_of_pools; iter++) {
            if (pools_array[iter].is_available == true && pools_array[iter].segment_size == _size_to_fill) {
                access_flag = true;
                break;
            }
        }
        if (access_flag == true) {
            number_of_pools++;
            return pools_array[iter].beginning_segment;
        }
        else {
            cout << "Not enought memory to allocation!\n";
            exit(0);
        }
    }
}

//clearing the memory
void my_free(void* _segment_ptr) {
    int iter = 0;
    while (pools_array[iter].beginning_segment != _segment_ptr) {
        iter++;
    }

    char* char_ptr = (char*)pools_array[iter].beginning_segment;
    for (int i = 0; i < pools_array[iter].segment_size; i++) {
        char_ptr[i] = 0;
    }

    pools_array[iter].is_available = true;
}

//defragmentation memory
void defrag_memory() {
    for (int i = 0; i < number_of_pools; i++) {
        if (i != 0 && pools_array[i - 1].is_available == true) {
            i--;
        }
        if (pools_array[i].is_available == true) {
            char* beg_del_pool = (char*)pools_array[i].beginning_segment;
            int offset_ptr = pools_array[i].segment_size;

            for (int j = i + 1; j < number_of_pools; j++) {
                char* old_pool_ptr = (char*)pools_array[j].beginning_segment;
                int offset = pools_array[j].segment_size;

                for (int k = 0; k < offset; k++) {
                    *beg_del_pool = *old_pool_ptr;
                    beg_del_pool++;
                    old_pool_ptr++;
                }

                old_pool_ptr = (char*)pools_array[j].beginning_segment;

                char* char_ptr = (char*)pools_array[j].beginning_segment;
                char_ptr -= offset_ptr;
                pools_array[j].beginning_segment = char_ptr;

                char_ptr = (char*)pools_array[j].end_segment;
                char_ptr -= offset_ptr;
                pools_array[j].end_segment = char_ptr;
            }

            pools_array[i].~MemoryPool();

            for (int j = i; j < number_of_pools; j++) {
                pools_array[j] = pools_array[j + 1];
                if (j == number_of_pools - 1) {
                    pools_array[j].~MemoryPool();
                }
                else {
                    pools_array[j] = pools_array[j + 1];
                }
            }

            zero_pool.free_mem_begin = ++beg_del_pool;
            zero_pool.available_memory += offset_ptr;
            number_of_pools--;
        }
    }
}

void output_information() {
    cout << "\n";
    for (int i = 0; i < number_of_pools; i++) {
        char* char_ptr = (char*)pools_array[i].beginning_segment;
        for (int j = 0; j < pools_array[i].segment_size; j++) {
            cout << "|" << (int)char_ptr[j] << "| ";
        }
    }

    for (int i = 0; i < zero_pool.available_memory; i++) {
        cout << "|_| ";
    }
    cout << "\n";
}