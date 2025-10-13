// operating_system.hpp
#include <string>
#include <map>
#include <chrono>
#include <bitset>
#include <vector>
#include "helper_funcs.hpp"
#include "physical_memory.hpp"
#include "virtual_memory.hpp"
#include "process.hpp"
#include <algorithm>
#include "swap_file.hpp"

using namespace std;

class operating_system
{
public:
    physical_memory ph_m;
    vector<process *> process_list;
    // frame number , being_used
    map<int, bool> frame_table;
    Swap_file SF1;


    operating_system()
    {
        // inshileze frame table
        for (int i = 0; i < 16; ++i)
        {
            frame_table[i] = ph_m.frames[i]->free;
        }
    }
    int create_process(string process_name)
    {
        // find uniqe process id
        int new_process_id;

        if (process_list.size() > 0)
        {

            while (true)
            {
                new_process_id = randomFiveDigit();
                bool duplcate_found = false;

                for (auto &prc : process_list)
                {
                    if (prc->process_id == new_process_id)
                    {
                        duplcate_found = true;
                        break;
                    }
                }
                if (!duplcate_found)
                {
                    break;
                }
            }
        }
        else
        {
            new_process_id = randomFiveDigit();
        }
        // find free frames
        // if not found os loades least used process into swap file based on priorty

        frame *avalible_frames[4] = {};
        int frames_found = 0;
        for (auto &f : ph_m.frames)
        {
            if (f->free)
            {
                avalible_frames[frames_found] = f;
                f->free = false;
                ++frames_found;
            }
            if (frames_found >= 4)
            {
                break;
            }
        }
        if (frames_found != 4)
        {
            // os should load the least used process int the swap file
            cout << "no frames are free \n";
            cout<<"loading into swap file\n";
        }
        update_frame_table();

        process *temp_prc_ptr = new process(new_process_id, avalible_frames[0], avalible_frames[1], avalible_frames[2], avalible_frames[3], process_name);
        process_list.push_back(temp_prc_ptr);

        return 0;
    }

    void print_frame_table()
    {
        for (auto &pair : frame_table)
        {
            cout << "frame " << pair.first << " is " << print_if_used(pair.second) << "\n";
        }
    }
    void update_frame_table()
    {
        for (int i = 0; i < 16; ++i)
        {
            frame_table[i] = ph_m.frames[i]->free;
        }
    }
    void delete_process(int delete_process_id)
    {
        for (auto &prc : process_list)
        {
            // find process to delete
            if (prc->process_id == delete_process_id)
            {
                // mark the frames fress
                for (int i = 0; i < 4; ++i)
                {
                    prc->process_virtual_memory->page_table_ptr[prc->process_virtual_memory->pages[i]]->free = true;
                }
                process_list.erase(remove(process_list.begin(), process_list.end(), prc), process_list.end());

                // call process deconstructor

                delete prc;
                update_frame_table();
                break;
            }
        }
    }
    string read(int process_id, string address)
    {
        process *target_prc = nullptr;
        for (auto &prc : process_list)
        {
            if (process_id == prc->process_id)
            {
                target_prc = prc;
                break;
            }
        }
        if (target_prc == nullptr)
        {
            throw out_of_range("process not found because it does not exsit");
        }

        int virtual_address_value = binary_to_deciaml(address);

        int page_size = target_prc->process_virtual_memory->pages[0]->page_size;

        int page_number = virtual_address_value / page_size;
        frame *target_frame = target_prc->process_virtual_memory->pages[page_number]->frame_ptr;

        int offset_in_page = virtual_address_value % page_size;

        if (page_number < 0 || page_number >= 4)
        {
            throw std::out_of_range("Page number out of range");
        }

        unsigned char *real_address = target_frame->start + offset_in_page;
        unsigned char value = target_frame->read(real_address);

        return uchar_to_exact_string(value);
    }

    int write(int process_id, string address, string value)
    {
        process *target_prc = nullptr;
        for (auto &prc : process_list)
        {
            if (process_id == prc->process_id)
            {
                target_prc = prc;
                break;
            }
        }
        if (target_prc == nullptr)
        {
            throw out_of_range("process not found because it does not exsit");
            return -1;
        }

        int virtual_address_value = binary_to_deciaml(address);
        int page_size = target_prc->process_virtual_memory->pages[0]->page_size;

        int page_number = virtual_address_value / page_size;
        frame *target_frame = target_prc->process_virtual_memory->pages[page_number]->frame_ptr;

        int offset_in_page = virtual_address_value % page_size;

        if (page_number < 0 || page_number >= 4)
        {
            throw std::out_of_range("Page number out of range");
            return -1;
        }

        // turn string into unsigned char to write
        unsigned char unsch_value = string_to_uchar(value);

        unsigned char *real_address = target_frame->start + offset_in_page;

        *real_address = unsch_value;
        return 0;
    }
};
