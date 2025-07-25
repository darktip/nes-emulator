#include "bitwise_utils.h"

bool isSigned(uint8_t val)
{
    return val & SIGN_BIT_INT8;
}
