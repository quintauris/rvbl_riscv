#include "rvbl/memory/rvbl_coherence.h"
#include "rvbl/test/rvbl_test.h"

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();
    rvbl_memory_coherence_instruction_fence();
    PASS();
    return 0;
}
