
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vptb.h"

Vptb *top;
#if VM_TRACE
VerilatedVcdC *trace;
#endif


int main(int argc, char *argv[])
{
    unsigned int lasttick;
    unsigned int tick;
    unsigned int addr;
    unsigned int mask;
    unsigned int simhalt;
    unsigned int did_reset;

    FILE *fp;

//    Verilated::commandArgs(argc, argv);


#if VM_TRACE
    Verilated::traceEverOn(true);
#endif

    top = new Vptb;

#if VM_TRACE
    trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("dut.vcd");
#endif

	top->mem_ready = 0;
	top->mem_rdata = 0;
    top->resetn = 0;
    simhalt=0;
    did_reset=0;
    tick=0;
    lasttick=tick;
    while (!Verilated::gotFinish())
    {

        tick++;
        if(tick<lasttick) printf("tick rollover\n");
        lasttick=tick;

        if(did_reset)
        {
            top->mem_ready = 0;
            if(top->mem_valid)
            {
                if(top->mem_instr)
                {
                    top->mem_ready = 1;
                    top->mem_rdata = 0x00758593;
                }
            }
        }
        else
        {
            if (tick > 11)
            {
                top->resetn = 1;
                did_reset = 1;
            }
        }
        top->clk = (tick & 1);
        top->eval();
#if VM_TRACE
        trace->dump(tick);
        if(tick>2000) break;
#endif
        if(simhalt) break;
    }
#if VM_TRACE
    trace->close();
#endif
    top->final();
    return 0;
}

