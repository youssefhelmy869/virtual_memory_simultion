// operating_system.hpp
// This header defines the operating_system class, which simulates process and memory management,
// including physical memory, virtual memory, frame allocation, and swap file operations.

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

// The operating_system class manages processes, memory frames, and swap file operations.
class operating_system
{
public:
    physical_memory ph_m; // Physical memory manager
    vector<process *> process_list; // List of active processes
    // Frame table: maps frame number to usage status (true = free, false = used)
    map<int, bool> frame_table;
    Swap_file SF1; // Swap file for paging out processes

    // Constructor initializes the frame table based on physical memory
    operating_system()
    {
        // Initialize frame table with frame usage status
        for (int i = 0; i < 16; ++i)
        {
            frame_table[i] = ph_m.frames[i]->free;
        }
    }

    // Creates a new process with a unique ID and allocates memory frames
    // Returns the process ID or -1 if creation fails
    int create_process(string process_name)
    {
        // Find unique process id
        int new_process_id;

        if (process_list.size() > 0)
        {
            // Generate a unique 5-digit process ID
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

        // Find free frames for the new process
        // If not enough frames, swap out the least important process
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
            // Not enough free frames, swap out least important process
            cout << "no frames are free \n";
            cout << "loading into swap file\n";

            if (SF1.free)
            {
                cout << "swap file is free\n";
            }
            else
            {
                cout << "there is no memory capcity\n";
                return -1;
            }
            process *least_used_process = nullptr;
            prc_priorty smallest_proirity = prc_priorty::NORMAL;
            // Find process with smallest importance
            for (auto &prc : process_list)
            {
                if (prc->IMPORTANT > smallest_proirity)
                {
                    smallest_proirity = prc->IMPORTANT;
                }
            }
            for (auto &prc : process_list)
            {
                if (prc->IMPORTANT == smallest_proirity)
                {
                    least_used_process = prc;
                }
            }
            // Move least used process to swap file
            SF1.process_id_in_file = least_used_process->process_id;
            SF1.process_in_file = least_used_process;
            // List of frames to mark free
            frame *frames_freed[4] = {least_used_process->process_virtual_memory->pages[0]->frame_ptr,
                                      least_used_process->process_virtual_memory->pages[1]->frame_ptr,
                                      least_used_process->process_virtual_memory->pages[2]->frame_ptr,
                                      least_used_process->process_virtual_memory->pages[3]
                                          ->frame_ptr};

            for (auto &fr : frames_freed)
            {
                fr->free = true;
                cout << "[operating system] frame " << fr->frame_id << " has been freed free = true \n";
            }
            // Add freed frames to available frames
            copy(begin(frames_freed), end(frames_freed), avalible_frames);

            cout << "[operating system]" << least_used_process->process_name << "has been move to swap file\n";
            for (int i = 0; i < 4; ++i)
            {
                SF1.frame_list[i] = least_used_process->process_virtual_memory->pages[i]->frame_ptr;
            }
        }
        update_frame_table();

        // Create and register the new process
        process *temp_prc_ptr = new process(new_process_id, avalible_frames[0], avalible_frames[1], avalible_frames[2], avalible_frames[3], process_name);
        process_list.push_back(temp_prc_ptr);

        update_frame_table();
        return temp_prc_ptr -> process_id;
    }

    // Prints the current frame table status
    void print_frame_table()
    {
        for (auto &pair : frame_table)
        {
            cout << "frame " << pair.first << " is " << print_if_used(pair.second) << "\n";
        }
    }

    // Updates the frame table to reflect current usage
    void update_frame_table()
    {
        for (int i = 0; i < 16; ++i)
        {
            frame_table[i] = ph_m.frames[i]->free;
        }
    }

    // Deletes a process by its ID, freeing its frames and removing it from the process list
    // Returns 0 on success
    int delete_process(int delete_process_id)
    {
        for (auto &prc : process_list)
        {
            // Find process to delete
            if (prc->process_id == delete_process_id)
            {
                // Mark the frames free
                for (int i = 0; i < 4; ++i)
                {
                    prc->process_virtual_memory->page_table_ptr[prc->process_virtual_memory->pages[i]]->free = true;
                }
                process_list.erase(remove(process_list.begin(), process_list.end(), prc), process_list.end());

                // Call process destructor
                delete prc;
                update_frame_table();
                return 0;
                break;
            }
        }
        // Also check swap file for process to delete
        for (auto &prc : process_list)
        {
            if (prc->process_id == SF1.process_id_in_file)
            {
                for (int i = 0; i < 4; ++i)
                {
                    prc->process_virtual_memory->page_table_ptr[prc->process_virtual_memory->pages[i]]->free = true;
                }
                process_list.erase(remove(process_list.begin(), process_list.end(), prc), process_list.end());

                // Call process destructor
                delete prc;
                update_frame_table();
                return 0;
                break;
            }
        }
    }

    // Reads a value from a process's virtual address
    // Throws out_of_range if process or page is not found
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
            if (process_id == SF1.process_id_in_file)
            {
                target_prc = SF1.process_in_file;
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

    // Writes a value to a process's virtual address
    // Throws out_of_range if process or page is not found
    int write(int process_id, string address, string value)
    {
        // Find target process
        process *target_prc = nullptr;
        for (auto &prc : process_list)
        {
            if (prc->process_id == process_id)
            {
                target_prc = prc;
                break;
            }
        }
        if (target_prc == nullptr)
        {
            if (process_id == SF1.process_id_in_file)
            {
                target_prc = SF1.process_in_file;
            }
        }
        if (target_prc == nullptr)
        {
            throw out_of_range("process does not exsit");
            return -1;
        }
        cout<<"[operating system] process "<< target_prc->process_name<<" has been found \n";
        // Value to write
        unsigned char uchar_value = string_to_uchar(value);

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

        target_prc->process_virtual_memory->pages[page_number]->frame_ptr->write(real_address, uchar_value);
        return 0;
    }
};
