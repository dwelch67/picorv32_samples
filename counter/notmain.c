void dummy ( unsigned int );

#define HEXOUT (*((volatile unsigned int *)0xD0000000))

int notmain ( void )
{
    unsigned int ra;
    dummy(5);
    for(ra=0;ra<10;ra++) HEXOUT=ra;
    return(0);
}
