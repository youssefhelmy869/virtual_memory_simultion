// physical_memory.hpp
// Defines the physical_memory class and frame structure for managing physical memory frames.

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "helper_funcs.hpp"
#include <stdexcept>
#pragma once

using namespace std;

struct frame
{
    int frame_id = 0;
    unsigned char *start;
    int size;
    unsigned char *end = nullptr;
    bool free = true;


    frame(int para_size, int para_id, unsigned char *para_start = nullptr)
    {
        frame_id = para_id;
        start = para_start;
        size = para_size;
        if (para_start != nullptr)
        {
            end = start + size;

            memset(start,0, size);
        }
        
    }
    unsigned char read(unsigned char *address)
    {
        if (is_ptr_in_block(start, size, address))
        {
            return *address;
        }
        else
        {
            cout << address << "\n";
            throw out_of_range(" memory address does not exsit");
        }
    }
    void write(unsigned char *address, unsigned char value)
    {
        if (is_ptr_in_block(start, size, address))
        {
            *address = value;
        }
        else
        {
            cout << address << "\n";
            throw out_of_range(" memory address does not exsit");
        }
    }
};
struct physical_memory
{
    frame *frames[16] = {};
    static const int memory_size = 41943040;
    static const int frames_size = memory_size / 16;
    unsigned char *memory;
    physical_memory()
    {
        memory = (unsigned char *)malloc(memory_size);
        for (int i = 0; i < 16; ++i)
        {
            frames[i] = new frame(frames_size, i, (memory + i * frames_size));
        }
    }
    unsigned char read(unsigned char *para_ptr)
    {
        if (is_ptr_in_block(memory, memory_size, para_ptr))
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
        if (is_ptr_in_block(memory, memory_size, address))
        {
            *address = value;
        }
        else
        {
            cout << address << "\n";
            throw out_of_range(" memory address does not exsit");
        }
    }
    void print_memory_layout()
    {
        for (int i = 0; i < 16; ++i)
            cout << "Frame " << i << ": "
                 << (void *)frames[i]->start << " - "
                 << (void *)frames[i]->end << "\n";
    }

    ~physical_memory()
    {
        for (int i = 0; i < 16; ++i)
        {
            delete frames[i];
        }
        free(memory);
    }
    void print_frame(int frame_number, int bytes_to_show = physical_memory::frames_size)
    {
        frame *f = frames[frame_number];
        cout << "Frame " << frame_number << " (" << f->size << " bytes)\n";
        cout << "Address\t\t\tValue\n";
        cout << "-------------------------------\n";

        for (int i = 0; i < bytes_to_show && i < f->size; ++i)
        {
            // Print the address safely using static_cast<void*>
            cout << static_cast<void *>(f->start + i) << "\t";

            // Print the byte as an integer (0–255)
            cout << static_cast<int>(*(f->start + i)) << "\n";
        }
    }
};