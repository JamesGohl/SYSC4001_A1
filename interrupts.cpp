/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>
#include <string>
#include <iostream>
#define CONTEXT_TIME 10
#define ISR_TIME 40
#define IRET_TIME 1

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int time = 0;

    /******************************************************************/


    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);
        /******************ADD YOUR SIMULATION CODE HERE*************************/
        
        if(activity == "SYSCALL" || activity == "END_IO")
        {
            auto pair = intr_boilerplate(time, duration_intr,  CONTEXT_TIME, vectors);
            auto[execution_return, time_return ] = pair;
            execution += execution_return;
            time = time_return;
            execution += std::to_string(time) + ", "  + std::to_string(ISR_TIME)  + ",  ISR Body\n"; 
            time += ISR_TIME;
            execution += std::to_string(time) + ", "  + std::to_string(IRET_TIME)  + ",  IRET \n"; 
            time += IRET_TIME;

            //wait for device to be done???????
            if (activity == "SYSCALL")
            {
                execution += std::to_string(time) + ", "  + std::to_string(delays[duration_intr - 1])  + ",  Wait for end IO\n"; 
                time += delays[duration_intr - 1];
            }
            
            
        }
        else {
            execution += std::to_string(time) + ", "  + std::to_string(duration_intr)  + ", " + activity + "\n"; 
            time += duration_intr;
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
