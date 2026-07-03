.global rvbl_semihost_interface

.section .text

rvbl_semihost_interface:
    slli    x0, x0, 0x1f
    ; /* Force *uncompressed* ebreak instruction */
    .byte   0x73
    .byte   0x00
    .byte   0x10
    .byte   0x00
    srai    x0, x0,  0x7
    ret

.end
