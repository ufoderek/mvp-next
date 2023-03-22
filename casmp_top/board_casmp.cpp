#include <fcntl.h>
#include <board_casmp.h>

/* load the application program to RAM */
void board_casmp::load_prog(char* path_prog)
{
    if(path_prog != NULL)
    {
        int fid = open(path_prog, O_RDONLY);

        if(fid > 0)
        {
            uint32_t addr = 0x0;
            uint32_t buff;
            int count;
            bool result;

            do
            {
                count = read(fid, &buff, 4);
                result = ram->write(buff, addr, 4);
                size_prog += count;

                if(result == false)
                {
                    printb(d_casmp, "cannot load program");
                }

                else
                {
                    addr += 4;
                }
            }
            while(count > 0);
        }

        else
        {
            printb(d_casmp, "cannot load program");
        }
    }
}

board_casmp::~board_casmp(void)
{
    printm(d_casmp, "board_casmp destructor");
}

#define print_line() printm(d_casmp,"===============================================");

board_casmp::board_casmp(sc_module_name name, bool using_gdb, unsigned int gdb_port): sc_module(name)
{
    int i;
    uint32_t n_cores = 4;
    uint32_t slave_id = 0;
    start_addr = 0;
    size_initrd = 0;
    size_ram = 128 << 20; //in bytes
    clk_fast.reset(new sc_clock("clk_fast", 5.0, SC_NS, 2.5, 0, SC_NS, true)); //200MHz clock
    clk_slow.reset(new sc_clock("clk_slow", 250.0, SC_NS, 125.0, 0, SC_MS, true)); //4MHz clock
    rst_n.write(true); //reset signal

    for(i = 0; i < n_cores; i++)
    {
        irq_n[i].write(true); //IRQ signal to CPU
        virq_n[i].write(true); //virtual IRQ signal to CPU
    }

    for(i = 0; i < 16 + 16 + 32; i++)   //sgi + ppi + spi = 16 + 16 + 32
    {
        channel[i].write(true);
    }

    always_1 = true;

    print_line();
    printm(d_casmp, "AHB Bus");
    my_bus.reset(new ahb("ahb_bus"));

    print_line();
    printm(d_casmp, "Fake DRAM, size: %x", size_ram);
    ram.reset(new RAM("RAM", size_ram));
    my_bus->ahb_to_slave_socket.bind(ram->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0, size_ram);

    print_line();
    printm(d_casmp, "ARMv7-A Core 0");
    arm_0.reset(new armv7a("armv7a_0", start_addr, 0, using_gdb, gdb_port));
    arm_0->clk(clk_fast->signal());
    arm_0->irq_n(irq_n[0]);
    arm_0->ahb_master_socket.bind(my_bus->ahb_from_master_socket);

    print_line();
    printm(d_casmp, "ARMv7-A Core 1");
    arm_1.reset(new armv7a("armv7a_1", start_addr, 1, using_gdb, gdb_port + 1));
    arm_1->clk(clk_fast->signal());
    arm_1->irq_n(irq_n[1]);
    arm_1->ahb_master_socket.bind(my_bus->ahb_from_master_socket);

    print_line();
    printm(d_casmp, "ARMv7-A Core 2");
    arm_2.reset(new armv7a("armv7a_2", start_addr, 2, using_gdb, gdb_port + 2));
    arm_2->clk(clk_fast->signal());
    arm_2->irq_n(irq_n[2]);
    arm_2->ahb_master_socket.bind(my_bus->ahb_from_master_socket);

    print_line();
    printm(d_casmp, "ARMv7-A Core 3");
    arm_3.reset(new armv7a("armv7a_3", start_addr, 3, using_gdb, gdb_port + 3));
    arm_3->clk(clk_fast->signal());
    arm_3->irq_n(irq_n[3]);
    arm_3->ahb_master_socket.bind(my_bus->ahb_from_master_socket);

    print_line();
    printm(d_casmp, "SHM UART 0");
    uart_0.reset(new shm_uart("shm_uart_0", 4096, true, 0)); //set stdin to be non-blocking
    uart_0->clk(clk_slow->signal());
    uart_0->rst_n(rst_n);
    uart_0->irq_n(channel[44]);
    my_bus->ahb_to_slave_socket.bind(uart_0->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x10009000, 4096);

    print_line();
    printm(d_casmp, "SHM UART 1");
    uart_1.reset(new shm_uart("shm_uart_1", 4096, true, 1));
    uart_1->clk(clk_slow->signal());
    uart_1->rst_n(rst_n);
    uart_1->irq_n(channel[45]);
    my_bus->ahb_to_slave_socket.bind(uart_1->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x1000A000, 4096);

    print_line();
    printm(d_casmp, "SHM UART 2");
    uart_2.reset(new shm_uart("shm_uart_2", 4096, true, 2));
    uart_2->clk(clk_slow->signal());
    uart_2->rst_n(rst_n);
    uart_2->irq_n(channel[46]);
    my_bus->ahb_to_slave_socket.bind(uart_2->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x1000B000, 4096);

    print_line();
    printm(d_casmp, "SHM UART 3");
    uart_3.reset(new shm_uart("shm_uart_3", 4096, true, 3));
    uart_3->clk(clk_slow->signal());
    uart_3->rst_n(rst_n);
    uart_3->irq_n(channel[47]);
    my_bus->ahb_to_slave_socket.bind(uart_3->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x1000C000, 4096);

    print_line();
    printm(d_casmp, "Timer 0");
    timer_0.reset(new TIMER("timer_0", 4096));
    timer_0->clk(clk_slow->signal());
    timer_0->rst_n(rst_n);
    timer_0->irq_n(channel[36]);
    my_bus->ahb_to_slave_socket.bind(timer_0->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x10011000, 4096);

    print_line();
    printm(d_casmp, "Timer 2");
    timer_2.reset(new TIMER("timer_2", 4096));
    timer_2->clk(clk_slow->signal());
    timer_2->rst_n(rst_n);
    timer_2->irq_n(channel[37]);
    my_bus->ahb_to_slave_socket.bind(timer_2->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x10012000, 4096);

    print_line();
    printm(d_casmp, "Generic Interrupt Controller Distributer (on board)");
    gic_dist.reset(new gic2_dist("gic_dist", 4096, 1, false, n_cores, 64));
    my_bus->ahb_to_slave_socket.bind(gic_dist->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x1E001000, 4096);

    print_line();
    printm(d_casmp, "Generic Interrupt Controller CPU interface (on board)");
    gic_cpu_if.reset(new gic2_cpu_if("gic_cpu_if", 4096, gic_dist, n_cores, 64));
    gic_cpu_if->clk(clk_slow->signal());

    for(i = 0; i < n_cores; i++)
    {
        gic_cpu_if->irq_n[i](irq_n[i]);
        gic_cpu_if->virq_n[i](virq_n[i]);
    }

    my_bus->ahb_to_slave_socket.bind(gic_cpu_if->ahb_slave_socket);
    my_bus->add_mapping(slave_id++, 0x1E000000, 4096);

    printm(d_casmp, "Connecting GIC PPI (IGNORED)");

    for(i = 16; i < 32; i++)
    {
        //gic_dist->ppi_n[0][i - 16] (channel[i]);
    }

    printm(d_casmp, "Connecting GIC SPI");

    for(i = 32; i < 64; i++)
    {
        gic_dist->spi_n[i - 32](channel[i]);
    }

    print_line();
}


