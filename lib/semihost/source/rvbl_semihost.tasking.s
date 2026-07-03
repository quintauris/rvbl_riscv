.global rvbl_semihost_interface

.section .text

rvbl_semihost_interface:
    slli    x0, x0, 0x1f
    ; /* Force *uncompressed* ebreak instruction */
    .db     0x73
    .db     0x00
    .db     0x10
    .db     0x00
    srai    x0, x0,  0x7
    ret

.end
