#include <iostream>
#include "physical_memory.hpp"
#include "helper_funcs.hpp"
#include "virtual_memory.hpp"
#include "operating_system.hpp"

using namespace std;

int main()
{
    operating_system os;
    os.create_process("vs code");

    os.create_process("word");
    os.print_frame_table();
    cout << "[main] exiting \n";
    

    return 0;
}
