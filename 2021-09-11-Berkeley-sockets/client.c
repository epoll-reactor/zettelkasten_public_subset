#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 1100

int main() {
  struct sockaddr_in sock_addr;
  int result;
  int sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_fd == -1) {
    perror("socket() failed");
    return -1;
  }
  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = PF_INET;
  sock_addr.sin_port = htons(PORT);
  sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (result < 0) {
    perror("first parameter isn't correct address category");
    close(sock_fd);
    return -1;
  } else if (!result) {
    perror("second parameter isn't correct IP-address");
    close(sock_fd);
    return -1;
  }
  if (connect(sock_fd, (struct sockaddr*)(&sock_addr), sizeof(sock_addr)) == -1) {
    perror("connect() failed");
    close(sock_fd);
    return -1;
  }
  char buffer[] = "123";
  send(sock_fd, buffer, 4, MSG_CONFIRM);
  shutdown(sock_fd, SHUT_RDWR);
  close(sock_fd);
}
