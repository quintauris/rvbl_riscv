.global machine_trap_table
.global supervisor_trap_table
.extern rvbl_interrupt_hart_internal_select_trap_handler
.extern interrupt_stack_top
.extern supervisor_stack_top

.section .text

.align 4
machine_trap_table:
    ;/* Alignment above is in GNU/RISC-V format (2^4=16 bytes)*/
    j machine_trap_wrapper

.align 4
supervisor_trap_table:
    ;/* Alignment above is in GNU/RISC-V format (2^4=16 bytes)*/
    j supervisor_trap_wrapper

.macro save_registers to scratch
    ;/* Save stack pointer (sp, x2) to scratch CSR */
    csrw \scratch, sp
    ;/* Setup trap stack */
    la sp, \to
    addi sp, sp, -140
    ;/* Save return address (ra, x1) */
    sw ra, 0(sp)
    ;/* Use return adddress (ra, x1) as temporary to store stack pointer */
    csrr ra, \scratch
    sw ra, 4(sp)
    sw x3, 8(sp)
    sw x4, 12(sp)
    sw x5, 16(sp)
    sw x6, 20(sp)
    sw x7, 24(sp)
    sw x8, 28(sp)
    sw x9, 32(sp)
    sw x10, 36(sp)
    sw x11, 40(sp)
    sw x12, 44(sp)
    sw x13, 48(sp)
    sw x14, 52(sp)
    sw x15, 56(sp)
#ifndef ILP32E
    sw x16, 60(sp)
    sw x17, 64(sp)
    sw x18, 68(sp)
    sw x19, 72(sp)
    sw x20, 76(sp)
    sw x21, 80(sp)
    sw x22, 84(sp)
    sw x23, 88(sp)
    sw x24, 92(sp)
    sw x25, 96(sp)
    sw x26, 100(sp)
    sw x27, 104(sp)
    sw x28, 108(sp)
    sw x29, 112(sp)
    sw x30, 116(sp)
    sw x31, 120(sp)
#endif
.endm

.macro restore_registers
    lw ra, 0(sp)
    lw x3, 8(sp)
    lw x4, 12(sp)
    lw x5, 16(sp)
    lw x6, 20(sp)
    lw x7, 24(sp)
    lw x8, 28(sp)
    lw x9, 32(sp)
    lw x10, 36(sp)
    lw x11, 40(sp)
    lw x12, 44(sp)
    lw x13, 48(sp)
    lw x14, 52(sp)
    lw x15, 56(sp)
#ifndef ILP32E
    lw x16, 60(sp)
    lw x17, 64(sp)
    lw x18, 68(sp)
    lw x19, 72(sp)
    lw x20, 76(sp)
    lw x21, 80(sp)
    lw x22, 84(sp)
    lw x23, 88(sp)
    lw x24, 92(sp)
    lw x25, 96(sp)
    lw x26, 100(sp)
    lw x27, 104(sp)
    lw x28, 108(sp)
    lw x29, 112(sp)
    lw x30, 116(sp)
    lw x31, 120(sp)
#endif
    lw sp, 4(sp)
.endm

machine_trap_wrapper:
    save_registers interrupt_stack_top mscratch

    ;/* Use a1 as temporary to save mepc, mtval and mcause */
    csrr a1, mepc
    sw a1, 124(sp)
    csrr a1, mtval
    sw a1, 128(sp)
    csrr a1, mcause
    sw a1, 132(sp)
    sw zero, 136(sp)

    ;/* Pass mode (machine, a0), mcause (a1) and sp(a2) to rvbl_interrupt_hart_internal_select_trap_handler */
    li a0, 3
    mv a2, sp
    call rvbl_interrupt_hart_internal_select_trap_handler

    ;/* Call selected trap handler */
    lw t0, 0(a0)
    mv a0, sp
    jalr t0

    ;/* Use a0 as temporary to restore mepc, mtval and mcause */
    lw a0, 124(sp)
    csrw mepc, a0

    restore_registers
    mret

supervisor_trap_wrapper:
    save_registers supervisor_stack_top sscratch

    ;/* Use a1 as temporary to save mepc, mtval and mcause */
    csrr a1, sepc
    sw a1, 124(sp)
    csrr a1, stval
    sw a1, 128(sp)
    csrr a1, scause
    sw a1, 132(sp)
    sw zero, 136(sp)

    ;/* Pass mode (supervisor, a0), mcause (a1) and sp (a2) to rvbl_interrupt_hart_internal_select_trap_handler */
    li a0, 1
    mv a2, sp
    call rvbl_interrupt_hart_internal_select_trap_handler

    ;/* Call selected trap handler */
    lw t0, 0(a0)
    mv a0, sp
    jalr t0

    ;/* Use a0 as temporary to restore sepc, stval and scause */
    lw a0, 124(sp)
    csrw sepc, a0

    restore_registers
    sret

.end
