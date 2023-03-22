#ifndef _BOARD_CASMP_H_
#define _BOARD_CASMP_H_

#include <memory>
#include <stdint.h>
#include <boost/shared_ptr.hpp>

#include <systemc.h>
#include <debug_utils.h>

#include <armv7a.h>
#include <ahb.h>
#include <gic2_cpu_if.h>
#include <gic2_dist.h>
#include <ps2.h>
#include <ram.h>
#include <timer.h>
#include <shm_uart.h>

class board_casmp: public sc_module
{
    public:

        SC_HAS_PROCESS(board_casmp);
        board_casmp(sc_module_name name, bool using_gdb, unsigned int gdb_port);
        ~board_casmp();

        uint32_t size_kernel;           //size of kernel
        uint32_t size_prog;             //size of application program
        uint32_t size_initrd;           //size of initrd image

        uint32_t size_ram;

        uint32_t start_addr;
        uint32_t smp_start_addr;

        auto_ptr<armv7a>arm_0;
        auto_ptr<armv7a>arm_1;
        auto_ptr<armv7a>arm_2;
        auto_ptr<armv7a>arm_3;

        auto_ptr<ahb>my_bus;
        boost::shared_ptr<gic2_dist>gic_dist;
        auto_ptr<gic2_cpu_if>gic_cpu_if;

        auto_ptr<RAM>ram;
        auto_ptr<TIMER>timer_0, timer_2;
        auto_ptr<shm_uart>uart_0, uart_1, uart_2, uart_3;

        auto_ptr<sc_clock>clk_fast;                             //200MHz clock
        auto_ptr<sc_clock>clk_slow;                             //4MHz clock

        sc_signal<bool>rst_n;                       //reset signal
        sc_signal<bool>irq_n[4];                       //IRQ signal to CPU
        sc_signal<bool>virq_n[4];                      //virtual IRQ
        sc_signal<bool>channel[16 + 16 + 32];          //sgi + ppi + spi
        sc_signal<bool>always_1;

        void load_prog(char* path_prog);

        bool using_gdb;
        unsigned int gdb_port;

};
#endif
