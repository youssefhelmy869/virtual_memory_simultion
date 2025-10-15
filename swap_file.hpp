// swap_file.hpp

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "helper_funcs.hpp"
#include <stdexcept>
#include "physical_memory.hpp"

using namespace std;

struct Swap_file
{
    static const int SWAP_FILE_SIZE = 4;
    frame *frame_list[SWAP_FILE_SIZE] = {};
    unsigned char *memory;
    static const int swap_file_memory_size = physical_memory::frames_size * 4;
    int process_id_in_file =0;
    process * process_in_file = nullptr;

    bool free = true;
    Swap_file()
    {
        memory = (unsigned char *)malloc(physical_memory::frames_size * 4);

        for (int i = 0; i < SWAP_FILE_SIZE; ++i)
        {
            frame *temp_ptr = new frame(physical_memory::frames_size, i, (memory + i * physical_memory::frames_size));
            frame_list[i] = temp_ptr;
        }
    }
    unsigned char read(unsigned char *para_ptr)
    {
        if (is_ptr_in_block(memory, swap_file_memory_size, para_ptr))
        {
            return *para_ptr;
        }
        else
        {
            cout << para_ptr << "\n";
            throw out_of_range(" memory address does not exsit");
        }
    }
    void write(unsigned char *address, unsigned char value)
    {
        if (is_ptr_in_block(memory, swap_file_memory_size, address))
        {
            *address = value;
        }
        else
        {
            cout << address << "\n";
            throw out_of_range(" memory address does not exsit");
        }
    }

    ~Swap_file()
    {
        for (auto &fr : frame_list)
        {
            delete fr;

        }
    }
};
