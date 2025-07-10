#pragma once
#include "Bus.h"

class BusDevice
{
protected:
    Bus* bus;
public:
    void connect(Bus* bus);
};
