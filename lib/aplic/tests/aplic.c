#include "rvbl/test/rvbl_test.h"

int main(void)
{
    rvbl_hart_hang_if_not(0);
    rvbl_test_initialize();

    // See lib/device/imsic/tests for a joint IMSIC+APLIC test
    SKIP();

    return 0;
}