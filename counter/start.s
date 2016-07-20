
.global _start
_start:
    lui sp,0x10
    jal notmain
    .word 0xFFFFFFFF
    j .

.globl dummy
dummy:
    ret
