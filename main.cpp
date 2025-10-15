#include <iostream>
#include "operating_system.hpp"
using namespace std;

int main()
{
    cout << "=== Memory Management System Test ===\n";

    operating_system os;

    // Create first process
    int p1 = os.create_process("Process_A");
    cout << "\n[main] Created process with ID: " << p1 << "\n";

    // Write to process A memory
    cout << "\n[main] Writing to Process_A memory...\n";
    os.write(p1, "0000000000000000", "65"); // write ASCII 'A'
    os.write(p1, "0000000000000001", "66"); // write ASCII 'B'
    os.write(p1, "0000000000000010", "67"); // write ASCII 'C'

    cout << "\n[main] Reading from Process_A memory...\n";
    cout << "Address 0: " << os.read(p1, "0000000000000000") << "\n";
    cout << "Address 1: " << os.read(p1, "0000000000000001") << "\n";
    cout << "Address 2: " << os.read(p1, "0000000000000010") << "\n";

    // Create multiple processes to fill memory
    cout << "\n[main] Creating more processes...\n";
    int p2 = os.create_process("Process_B");
    int p3 = os.create_process("Process_C");
    int p4 = os.create_process("Process_D");
    int p5 = os.create_process("Process_E"); // should trigger swap if no frames left

    cout << "\n[main] Current frame table:\n";
    os.print_frame_table();

    // Change priority of one process
    cout << "\n[main] Changing priority of Process_B...\n";
    for (auto prc : os.process_list)
    {
        if (prc->process_name == "Process_B")
        {
            prc->change_priorty(prc_priorty::CRITICAL);
            cout << "[main] Process_B priority set to CRITICAL\n";
        }
    }

    // Delete one process
    cout << "\n[main] Deleting Process_A...\n";
    os.delete_process(p1);

    cout << "\n[main] Frame table after deletion:\n";
    os.print_frame_table();

    cout << "\n=== Test completed successfully ===\n";
    return 0;
}
