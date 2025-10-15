// process.hpp
// Defines the process class representing an OS process.
#include <string>
#include <vector>
#include <iostream>
#include "virtual_memory.hpp"
enum prc_priorty
{
    IDLE = 50,
    BELOW_NORMAL = 60,
    NORMAL = 70,
    IMPORTANT = 80,
    VERY_IMPORTANT = 90,
    CRITICAL = 100

};

class process
{
public:
    int process_id = 0;
    string process_name;
    vitual_memory *process_virtual_memory;
    prc_priorty IMPORTANT = prc_priorty::NORMAL;

    // Constructor: Initializes a process with given frames and name.
    process(int id, frame* f1, frame* f2, frame* f3, frame* f4, std::string name)
    {
        cout << "[process manger]" << name << " process is being created \n";
        process_name = name;
        frame *arr[4] = {f1, f2, f3, f4};
        cout << "[process manger] process " << process_name << " has been assigned frames " << arr[0]->frame_id << " " << arr[1]->frame_id << " " << arr[2]->frame_id << " " << arr[3]->frame_id << "\n";
        process_virtual_memory = new vitual_memory(arr);

        process_id = id;
        cout << "[process manger] " << process_name << "has been created with id" << process_id << '\n';
    }

    void change_priorty(prc_priorty para_priorty)
    {
        IMPORTANT = para_priorty;
    }

    ~process()
    {
        delete process_virtual_memory;
    }
};