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
        
        if(activity == "SYSCALL")
        {
            auto pair = intr_boilerplate(time, duration_intr,  CONTEXT_TIME, vectors);
            auto[execution_return, time_return ] = pair;
            execution += execution_return;
            time = time_return;
            //example ISR activities
            //assume each activities takes approx 1/3 of time
            int call_driver, transfer_data, check_errors;
            call_driver = (delays[duration_intr - 1]) / 3;
            transfer_data = (delays[duration_intr - 1]) / 3;
            check_errors = (delays[duration_intr - 1]) / 3 + (delays[duration_intr - 1]) % 3;

            execution += std::to_string(time) + ", "  + std::to_string(call_driver)  + ", call device driver\n"; 
            time += call_driver;

            execution += std::to_string(time) + ", "  + std::to_string(transfer_data)  + ", transfer data\n"; 
            time += transfer_data;

            execution += std::to_string(time) + ", "  + std::to_string(check_errors)  + ", check for errors\n"; 
            time += check_errors;

            execution += std::to_string(time) + ", "  + std::to_string(IRET_TIME)  + ",  IRET \n"; 
            time += IRET_TIME;
            
        }

        if(activity == "END_IO")
        {
            auto pair = intr_boilerplate(time, duration_intr,  CONTEXT_TIME, vectors);
            auto[execution_return, time_return ] = pair;
            execution += execution_return;
            time = time_return;
            //example end io activities
            //assume each activities takes approx 1/3 of time
            int store_info, check_errors, finish_io;
            store_info = (delays[duration_intr - 1]) / 3;
            check_errors = (delays[duration_intr - 1]) / 3;
            finish_io = (delays[duration_intr - 1]) / 3 + (delays[duration_intr - 1]) % 3;

            execution += std::to_string(time) + ", "  + std::to_string(store_info)  + ", store information in memory\n"; 
            time += store_info;

            execution += std::to_string(time) + ", "  + std::to_string(check_errors)  + ", check for errors\n"; 
            time += check_errors;

            execution += std::to_string(time) + ", "  + std::to_string(finish_io)  + ", finish END_IO process\n"; 
            time += finish_io;

            execution += std::to_string(time) + ", "  + std::to_string(IRET_TIME)  + ",  IRET \n"; 
            time += IRET_TIME;
            
        }
        //CPU burst
       if(activity == "CPU")
       {
            execution += std::to_string(time) + ", "  + std::to_string(duration_intr)  + ", CPU burst \n"; 
            time += duration_intr;
        }
        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
