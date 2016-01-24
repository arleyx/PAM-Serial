#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "serial/serial.c"

#define SERIAL "⁠⁠⁠75HBZN1"
#define SERIAL_ENCODE "000111000101010001001011100011010111000001"
#define BAUDRAGE B9600
#define TIMEOUT 1000000

int main(int argc, char const *argv[]) {

    char* ports[] = {"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7", "/dev/ttyUSB8", "/dev/ttyUSB9"};

	for (int i = 0; i < 10; i++) {
	    int serial_fd = serial_open(ports[0], BAUDRAGE);

	    if (serial_fd > 0) {
			sleep(2);

		    serial_send(serial_fd, SERIAL_ENCODE, strlen(SERIAL_ENCODE));

		    int dataLength;
		    char data[6];

			time_t start = time(NULL);
		    while (1) {
		        dataLength = serial_read(serial_fd, data, 6, TIMEOUT);

		        if (dataLength > 0) {
		            printf("Length data: %d\n", dataLength);
		            printf("Data: %s\n", data);

                    if (strcmp(data, "101000") == 0) {
                        printf("OK!!!\n");
                    } else {
                        printf("ERROR!!!\n");
                        exit(0);
                    }

		            break;
		        }
				if (difftime(time(NULL), start) > 5) { exit(0); }
		    }

		    serial_close(serial_fd);

			exit(0);
	    }
	}

    /*time_t start = time(NULL);

    sleep(1);

    printf("Tiempo: %f\n", difftime(time(NULL), start));
    //printf("Tiempo end: %f\n", end);*/

    /*char* ports[] = {"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7", "/dev/ttyUSB8", "/dev/ttyUSB9"};

    int serial_fd = serial_open(ports[0], BAUDRAGE);

    if (serial_fd == -1) {
        printf("Error opening the serial device: %s\n", ports[0]);
        perror("OPEN");
        exit(0);
    }

    sleep(2);

    serial_send(serial_fd, SERIAL_ENCODE, strlen(SERIAL_ENCODE));

    int dataLength;
    char data[6];

    while (1) {
        dataLength = serial_read(serial_fd, data, 6, TIMEOUT);

        if (dataLength > 0) {
            printf("Length data: %d\n", dataLength);
            printf("Data: %s\n", data);
            break;
        }
    }

    serial_close(serial_fd);

    exit(0);*/

    return 0;
}
