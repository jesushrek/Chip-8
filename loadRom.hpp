#ifndef LOADROM_HPP
#define LOADROM_HPP

#include "chip8.hpp"

#include <iostream>
#include <string>
#include <fstream>

void loadRom(Chip8* subject, char* pathToRom)
{ 
    std::ifstream file(pathToRom, std::ios::binary | std::ios::ate);

    if (file.is_open())
    { 
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        unsigned int starting { 0x200 };

        for (int i = 0; i < size; ++i)
            subject->Memory[i + starting] = buffer[i];

        delete[]  buffer;
    }
}

#endif
