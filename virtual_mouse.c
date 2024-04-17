#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <errno.h>

int setup_virtual_mouse(int fd)
{
    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Virtual Mouse");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0xfedc;
    uidev.id.version = 1;

    if (write(fd, &uidev, sizeof(uidev)) < 0)
        return -1;

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) < 0)
        return -1;

    if (ioctl(fd, UI_SET_EVBIT, EV_REL) < 0 ||
        ioctl(fd, UI_SET_RELBIT, REL_X) < 0 ||
        ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        return -1;

    if (ioctl(fd, UI_DEV_CREATE) < 0)
        return -1;

    return 0;
}

void emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    if (write(fd, &ie, sizeof(ie)) < 0) {
        perror("Error in emit function");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("Opening /dev/uinput failed");
        exit(EXIT_FAILURE);
    }

    if (setup_virtual_mouse(fd) < 0)
    {
        perror("Setup failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 20; i++)
    {
        emit(fd, EV_KEY, BTN_LEFT, 1); // press left button
        emit(fd, EV_REL, REL_X, i * 10); // ьove right
        emit(fd, EV_REL, REL_Y, i * 20); // move down
        emit(fd, EV_SYN, SYN_REPORT, 0); // sync the report
        emit(fd, EV_KEY, BTN_LEFT, 0); // release left button
        usleep(1500000); // sleep for 1.5 second
    }

    // Destroy the virtual device
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);

    return 0;
}
