
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

static FILE *fpsrec;

#define HEXOUT 0xD0000000
#define MEMMASK 0xFFFF
static unsigned int ram[(MEMMASK+1)>>2];

static unsigned int hexchar ( unsigned char x )
{
    x-=0x30;
    if(x>9) x-=7;
    return(x&15);
}
static int readsrec ( void )
{
    char newline[1024];
    unsigned char dataline[300];
    unsigned int line;
    unsigned int address;
    unsigned int len;
    unsigned int sum;
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    memset(ram,0xFF,sizeof(ram));

    line=0;
    while(fgets(newline,sizeof(newline)-1,fpsrec))
    {
        line++;
        if(newline[0]!='S')
        {
            printf("syntax error line %u\n",line);
            return(1);
        }
        switch(newline[1])
        {
            case 0x30:
            {
                break;
            }
            case 0x33:
            {
                //address big endian, data little
                //012345
                //S31500000000EF00400137210000FFFFFFFF6F000000F7
                //   0: 014000ef            jal 14 <notmain>
                len=hexchar(newline[2]);
                len<<=4;
                len|=hexchar(newline[3]);
                //printf("%u len 0x%02X\n",line,len);
                if(len<6)
                {
                    printf("%u len too short %u\n",line,len);
                    return(1);
                }
                if((len-1)&3)
                {
                    printf("%u expecting whole words\n",line);
                    return(1);
                }
                sum=len;
                for(ra=0;ra<len;ra++)
                {
                    rb=hexchar(newline[(ra<<1)+4]);
                    rb<<=4;
                    rb|=hexchar(newline[(ra<<1)+5]);
                    sum+=rb;
                    dataline[ra]=rb;
                }
                //printf("sum 0x%X\n",sum);
                sum&=0xFF;
                if(sum!=0xFF)
                {
                    printf("%u checksum error 0x%02X\n",line,sum);
                    return(1);
                }
                ra=0;
                address=0;
                address<<=8; address|=dataline[ra++];
                address<<=8; address|=dataline[ra++];
                address<<=8; address|=dataline[ra++];
                address<<=8; address|=dataline[ra++];
                for(;ra<len;)
                {
                    rc=0;
                    rb=dataline[ra++]; rc>>=8; rc|=rb<<24;
                    rb=dataline[ra++]; rc>>=8; rc|=rb<<24;
                    rb=dataline[ra++]; rc>>=8; rc|=rb<<24;
                    rb=dataline[ra++]; rc>>=8; rc|=rb<<24;
                    printf("%08X : 0x%08X\n",address,rc);
                    ram[(address&MEMMASK)>>2]=rc;
                    address+=4;
                }
                break;
            }
            case 0x37:
            {
                break;
            }
        }
    }
    printf("------------\n");

    return(0);
}

int main(int argc, char *argv[])
{
    unsigned int lasttick;
    unsigned int tick;
    unsigned int addr;
    unsigned int mask;
    unsigned int simhalt;
    unsigned int did_reset;


//    Verilated::commandArgs(argc, argv);
    if(argc<2)
    {
        printf("Vsim filename.srec\n");
        return(1);
    }
    fpsrec=fopen(argv[1],"rt");
    if(fpsrec==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    if(readsrec())
    {
        fclose(fpsrec);
        return(1);
    }
    fclose(fpsrec);




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
                    //top->mem_ready = 1;
                    top->mem_rdata = ram[(top->mem_addr&MEMMASK)>>2];
                }
                else
                if(top->mem_wstrb)
                {
                    if((tick&1)==0)
                    {
                        if(top->mem_wstrb==0xF)
                        {
                            if(top->mem_addr==HEXOUT)
                            {
                                printf("%08X\n",top->mem_wdata);
                            }
                            else
                            {
                                //printf("write %08X : %08X\n",top->mem_addr,top->mem_wdata);
                                ram[(top->mem_addr&MEMMASK)>>2]=top->mem_wdata;

                            }
                        }
                        else
                        {
                            unsigned int mask;
                            unsigned int data;
                            //printf("not a word write 0x%08X 0x%08X 0x%08X\n",top->mem_wstrb,top->mem_addr,top->mem_wdata);
                            data=ram[(top->mem_addr&MEMMASK)>>2];
                            mask=0;
                            if(top->mem_wstrb&0x8) mask|=0xFF000000;
                            if(top->mem_wstrb&0x4) mask|=0x00FF0000;
                            if(top->mem_wstrb&0x2) mask|=0x0000FF00;
                            if(top->mem_wstrb&0x1) mask|=0x000000FF;
                            data&=~(mask);
                            data|=top->mem_wdata&mask;
                            ram[(top->mem_addr&MEMMASK)>>2]=data;
                        }
                    }
                }
                else
                {
                    if((tick&1)==0)
                    {
                        top->mem_rdata = ram[(top->mem_addr&MEMMASK)>>2];
                        //printf("read %08X : %08X\n",top->mem_addr,top->mem_rdata);
                    }
                }
                top->mem_ready = 1;
            }
            if(top->trap)
            {
                printf("trap\n");
                break;
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
        if(tick>200000) break;
#endif
        if(simhalt) break;
    }
#if VM_TRACE
    trace->close();
#endif
    top->final();
    return 0;
}

