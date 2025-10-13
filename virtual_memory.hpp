// virtual_memory.hpp
#include <string>
#include <map>
#include <chrono>
#include <bitset>
#include <vector>
#include "helper_funcs.hpp"
#include "physical_memory.hpp"
#pragma once

struct page
{
    bool being_used = false;
    int page_id = 0;
    frame *frame_ptr;
    int page_size;
    page(int para_id, frame *free_ptr = nullptr)
    {
        page_id = para_id;
        if (free_ptr != nullptr)
        {
            frame_ptr = free_ptr;
        }
        else
        {
            throw invalid_argument("frame ptr not provied is still a null pointer");
        }
        page_size = frame_ptr->size;
    }
};
struct vitual_memory
{
    page *pages[4] = {};

    map<page *, frame *> page_table_ptr;

    vitual_memory(frame *free_frames[4])
    {
        for (int i = 0; i < 4; ++i)
        {
            free_frames[i]->free = false;
            pages[i] = new page(i, free_frames[i]);
            page_table_ptr[pages[i]] = free_frames[i];
        }
        cout << "[vitual memory constructor] frames have been added \n";
    }

    unsigned char read(int page_number, unsigned char *address) // os resonsbilty to change user input address "0101010101" to unsigned char *
    {
        return pages[page_number]->frame_ptr->read(address);
    }

    void write(int page_number, unsigned char *address, unsigned char value) // os resonsbilty to change user input address "0101010101" to unsigned char *
    {
        pages[page_number]->frame_ptr->write(address, value);
    }

    void print_page(int page_number, int amount_to_print = -1)
    {
        if (page_number < 0 || page_number >= 4)
        {
            cerr << "Error: invalid page number.\n";
            return;
        }

        page *pg = pages[page_number];
        frame *fr = pg->frame_ptr;

        if (!fr || !fr->start)
        {
            cerr << "Error: frame pointer is null.\n";
            return;
        }

        int limit = (amount_to_print < 0 || amount_to_print > pg->page_size)
                        ? pg->page_size
                        : amount_to_print;

        cout << "=== Page " << page_number << " (" << limit << " bytes) ===\n";
        for (int i = 0; i < limit; ++i)
        {
            unsigned char value = fr->start[i];
            cout << "Address " << static_cast<void *>(fr->start + i)
                 << " | Decimal: " << static_cast<int>(value)
                 << " | Hex: 0x" << hex << static_cast<int>(value) << dec
                 << " | Char: '" << (isprint(value) ? static_cast<char>(value) : '.') << "'"
                 << "\n";
        }
        cout << "=============================\n";
    }
    ~vitual_memory()
    {
        for (auto &page : pages)
        {
            delete page;
        }
    }
};
