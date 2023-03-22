#include <cstring>
#include <memory>
#include <systemc.h>

#include <board_casmp.h>
#include <signal_redirector.h>

using namespace std;

void init_debug_units()
{
#ifndef RELEASE
    setl(d_inst);
    //setl(d_armv7a_rf);
    //setl(d_armv7a_cmsa);
#endif
}

auto_ptr<board_casmp>board;
auto_ptr<signal_redirector>sig_redirector;

int sc_main(int argc, char* argv[])
{
    printm(d_main, "disable SystemC's warning messages of duplicated object names");
    sc_report_handler::set_actions(SC_ID_OBJECT_EXISTS_, SC_DO_NOTHING);

    init_debug_units();

    int i = 1;
    bool using_prog = false; //the application program
    char path_prog[128];
    bool using_gdb = false; //the GDB stub
    int gdb_port = 0;

    /* parsing arguments */
    while(i < argc)
    {
        if(!strcmp(argv[i], "--prog"))
        {
            i++;

            if(i < argc)
            {
                strcpy(path_prog, argv[i]);
                using_prog = true;
            }

            else
            {
                printb(d_main, "error loading program");
            }
        }

        else if(!strcmp(argv[i], "--gdb"))
        {
            using_gdb = true;
        }

        else if(!strcmp(argv[i], "--port"))
        {
            i++;
            gdb_port = atoi(argv[i]);
        }

        else
        {
            printb(d_main, "unknown argument: %s", argv[i]);
        }

        i++;
    }

    printm(d_main, "initialize signal_redirector");
    sig_redirector.reset(new signal_redirector());
    printm(d_main, "initializing CAS Multicore Platform...");
    board.reset(new board_casmp("casmp", using_gdb, gdb_port));
    printm(d_main, "CAS Multicore Platform initialized");

    if(using_prog)
    {
        board->load_prog(path_prog);
        board->start_addr = 0;
    }

    else
    {
        printb(d_main, "parameter error");
    }

    printm(d_main, "start simulating...");
    sc_start();

    return 0;
}
