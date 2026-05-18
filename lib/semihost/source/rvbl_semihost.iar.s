    public rvbl_semihost_interface

    section `.text`:CODE

rvbl_semihost_interface:
    slli    x0, x0, 0x1f
    /* Force *uncompressed* ebreak instruction */
    dc8     0x73
    dc8     0x00
    dc8     0x10
    dc8     0x00
    srai    x0, x0,  0x7
    ret

    end
