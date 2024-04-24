#include <iostream>
#include <unistd.h>

char secretData[100];  // Змінюємо на масив символів

int main()
{
    std::cout << "Writer PID: " << getpid() << std::endl;

    while (true)
    {
        std::cout << "Введіть секретні дані: ";
        std::cin.getline(secretData, sizeof(secretData));
        std::cout << "Адреса змінної secretData: " << static_cast<void*>(secretData) << std::endl;

    }
    return 0;
}
