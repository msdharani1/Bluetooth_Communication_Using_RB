#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <termios.h>

#define TRIG_PATH "/sys/class/gpio/PC11/"
#define ECHO_PATH "/sys/class/gpio/PC25/"
#define UART_DEV "/dev/ttyS3"  // Update this if your UART device differs

void gpio_write(const char* path, const char* file, const char* value) {
    char full_path[128];
    snprintf(full_path, sizeof(full_path), "%s%s", path, file);
    int fd = open(full_path, O_WRONLY);
    if (fd < 0) {
        perror("gpio_write");
        exit(1);
    }
    write(fd, value, strlen(value));
    close(fd);
}

char gpio_read(const char* path, const char* file) {
    char full_path[128], value_str[3];
    snprintf(full_path, sizeof(full_path), "%s%s", path, file);
    int fd = open(full_path, O_RDONLY);
    if (fd < 0) {
        perror("gpio_read");
        exit(1);
    }
    read(fd, value_str, sizeof(value_str));
    close(fd);
    return value_str[0];
}

void delay_microseconds(long us) {
    struct timespec req = {0};
    req.tv_sec = us / 1000000;
    req.tv_nsec = (us % 1000000) * 1000;
    nanosleep(&req, NULL);
}

long get_microseconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

int setup_uart() {
    int uart_fd = open(UART_DEV, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Unable to open UART");
        exit(1);
    }

    struct termios options;
    tcgetattr(uart_fd, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &options);
    return uart_fd;
}

void uart_send(int fd, const char* message) {
    write(fd, message, strlen(message));
}

int main() {
    gpio_write(TRIG_PATH, "direction", "out");
    gpio_write(ECHO_PATH, "direction", "in");

    int uart_fd = setup_uart();

    while (1) {
        // Trigger the ultrasonic sensor
        gpio_write(TRIG_PATH, "value", "0");
        delay_microseconds(2);
        gpio_write(TRIG_PATH, "value", "1");
        delay_microseconds(10);
        gpio_write(TRIG_PATH, "value", "0");

        // Wait for echo to go HIGH
        while (gpio_read(ECHO_PATH, "value") == '0');
        long start = get_microseconds();

        // Wait for echo to go LOW
        while (gpio_read(ECHO_PATH, "value") == '1');
        long end = get_microseconds();

        // Calculate distance in cm
        long duration = end - start;
        float distance_cm = (duration * 0.0343f) / 2.0f;

        // Check if within 20 cm range
        if (distance_cm <= 20.0f) {
            float percent = ((20.0f - distance_cm) / 20.0f) * 100.0f;
            if (percent < 0) percent = 0;
            printf("Distance: %.2f cm\tProximity: %.1f%%\n", distance_cm, percent);

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "Proximity: %.1f%%\n", percent);
            uart_send(uart_fd, buffer);
        } else {
            printf("Distance: %.2f cm\tOUT OF BOX\n", distance_cm);
            uart_send(uart_fd, "OUT OF BOX\n");
        }

        sleep(1);
    }

    close(uart_fd);
    return 0;
}

