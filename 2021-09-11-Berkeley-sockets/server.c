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
  int sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_fd == -1) {
    perror("socket() failed");
    return -1;
  }
  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = PF_INET;
  sock_addr.sin_port = htons(PORT);
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock_fd, (struct sockaddr*)(&sock_addr), sizeof(sock_addr)) == -1) {
    perror("bind() failed");
    close(sock_fd);
    return -1;
  }
  if (listen(sock_fd, 10) == -1) {
    perror("listen() failed");
    close(sock_fd);
    return -1;
  }
  while (1) {
    int connect_fd = accept(sock_fd, 0, 0);
    printf("accept\n");
    if (connect_fd < 0) {
      perror("accept() failed");
      close(sock_fd);
      return -1;
    }
    char buffer[1024];
    recv(connect_fd, buffer, 1024, 0);
    printf("received: %s\n", buffer);
    close(connect_fd);
  }
  close(sock_fd);
}
