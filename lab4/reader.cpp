#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>

int main()
{
    pid_t target_pid;
    std::cout << "Введіть PID процесу Writer: ";
    std::cin >> target_pid;

    std::ifstream mem_file;
    std::string mem_path = "/proc/" + std::to_string(target_pid) + "/mem";
    mem_file.open(mem_path, std::ios::binary);

    if (!mem_file.is_open())
    {
        std::cerr << "Не вдалося відкрити файл пам'яті." << std::endl;
        return 1;
    }

    std::string address_str;
    std::cout << "Введіть адресу змінної secretData (в шістнадцятковому форматі): ";
    std::cin >> address_str;
    std::stringstream ss;
    ss << std::hex << address_str;
    uintptr_t address;
    ss >> address;

    char last_buffer[100] = {0};
    char buffer[100];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));  // Clear buffer before read
        mem_file.seekg(address);  // Seek to the right spot
        mem_file.read(buffer, sizeof(buffer));

        if (strcmp(buffer, last_buffer) != 0)
        {
            std::cout << "Дані з адреси " << std::hex << address << " змінилися: " << buffer << std::endl;
            memcpy(last_buffer, buffer, sizeof(buffer));  // Update the last_buffer
        }

        sleep(1);  // Wait for a second before reading again
    }

    mem_file.close();
    return 0;
}
