#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstring>
#include <errno.h>

int main() {
    const char* device = "/dev/input/event4";
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Error opening device: " << strerror(errno) << std::endl;
        return 1;
    }

    struct input_event ie;
    while (read(fd, &ie, sizeof(ie)) > 0) {
        if (ie.type == EV_KEY && ie.value >= 0 && ie.value <= 1) {
            // Check for mouse button presses
            if (ie.code == BTN_LEFT)
                std::cout << "Left button " << (ie.value ? "pressed" : "released") << std::endl;
            else if (ie.code == BTN_RIGHT)
                std::cout << "Right button " << (ie.value ? "pressed" : "released") << std::endl;
        } else if (ie.type == EV_REL) {
            // Check for relative movements
            if (ie.code == REL_X)
                std::cout << "Relative movement X: " << ie.value << std::endl;
            if (ie.code == REL_Y)
                std::cout << "Relative movement Y: " << ie.value << std::endl;
        }
    }

    close(fd);
    return 0;
}
