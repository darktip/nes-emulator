#include <iostream>
#include "NES/NES.h"

int main(char* args)
{
    nes::NES nes;
    nes.LoadProgram("A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA", 0x8000);

    while (true)
    {
        nes.Run();
    }
    
    system("Pause");
    return 0;
}
