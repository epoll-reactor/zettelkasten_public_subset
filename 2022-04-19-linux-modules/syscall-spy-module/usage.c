#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <sys/syscall.h>

int main()
{
	const char filename[] = "buffer.txt";
	syscall(SYS_open, filename, O_CREAT, S_IRUSR | S_IWUSR);
	int fd = syscall(SYS_open, filename, O_WRONLY);
	syscall(SYS_write, fd, "hello", 5);
	syscall(SYS_close, fd);
	syscall(SYS_fchmod, fd, S_IXUSR);
}
