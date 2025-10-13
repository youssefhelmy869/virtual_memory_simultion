// process.hpp
#include <string>
#include <vector>
#include <iostream>
#include "virtual_memory.hpp"
enum prc_priorty
{
    IDLE,
    BELOW_NORMAL,
    NORMAL,
    IMPORTANT,
    VERY_IMPORTANT,
    CRITICAL

};

class process
{
public:
    int process_id = 0;
    string process_name;
    vitual_memory *process_virtual_memory;
    prc_priorty IMPORTANT = prc_priorty::NORMAL;

    process(int para_process_id, frame *f1, frame *f2, frame *f3, frame *f4, string para_process_name)
    {
        process_name = para_process_name;
        frame *arr[4] = {f1, f2, f3, f4};
        cout << "process " << process_name << " has been assigned frames " << arr[0]->frame_id << " " << arr[1]->frame_id << " " << arr[2]->frame_id << " " << arr[3]->frame_id << "\n";
        process_virtual_memory = new vitual_memory(arr);

        process_id = para_process_id;
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