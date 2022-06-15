/*
 * ioctl_user_space.c - Example usage of ioctl.
 */
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include <sys/ioctl.h>

int main()
{
	int fd = open("/dev/ioctl_device", O_WRONLY);
	if (fd < 0) {
		perror("ioctl_device");
		return -1;
	}

	for (;;) {
		int cmd = 0;
		printf("Choose command (1 or 2, other to exit)\n>>> ");
		scanf("%d", &cmd);

		switch (cmd) {
		case 1:
		case 2:
			int ret = ioctl(fd, 0xFFAA + cmd, 8);
			if (ret < 0) {
				perror("ioctl");
				return -1;
			}
			break;
			
		default:
			printf("Exit...\n");
			return 0;
		}
	}
}
