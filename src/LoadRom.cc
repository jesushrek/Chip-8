#include "Chip8.hh"

#include <string>
#include <fstream>

void loadRom(Chip8& subject, const char* path) 
{ 
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (file.is_open()) 
    { 
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);

        char* buffer = new char[size];

        file.read(buffer, size);
        file.close();

        unsigned int starting { 0x200 };
        for (int i = 0; i < size; ++i) { 
            subject.memory[starting + i] = buffer[i];
        }
        delete[] buffer;
    }
}
