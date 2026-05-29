.global rvbl_atomic_compare_and_set

.section .text

rvbl_atomic_compare_and_set:
    lr.w t0, (a0)
    bne t0, a1, fail
    sc.w t0, a2, (a0)
    bnez t0, rvbl_atomic_compare_and_set
    li a0, 1
    ret
fail:
    li a0, 0
    ret

.end
