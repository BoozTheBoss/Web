#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 1024

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  char buffer[MAXBUF];
  char filename[MAXBUF];

  // Create a new socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error opening socket");
    exit(1);
  }

  // Set up the server address structure
  memset(&serv_addr, 0, sizeof(serv_addr));
  portno = 8080;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  // Bind the socket to the specified port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("Error binding socket");
    exit(1);
  }

  // Listen for incoming connections
  listen(sockfd, 5);
  printf("Listening on port %d\n", portno);

  while (1) {
    // Accept incoming connections
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
      perror("Error accepting connection");
      exit(1);
    }

    // Read the filename from the client
    bzero(filename, MAXBUF);
    int n = read(newsockfd, filename, MAXBUF);
    if (n < 0) {
      perror("Error reading filename");
      exit(1);
    }

    // Open the file for writing
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
      perror("Error opening file");
      exit(1);
    }

    // Read data from the client and write it to the file
    while (1) {
      bzero(buffer, MAXBUF);
      n = read(newsockfd, buffer, MAXBUF);
      if (n < 0) {
        perror("Error reading data");
        exit(1);
      }
      if (n == 0) {
        break;
      }
      if (write(fd, buffer, n) < 0) {
        perror("Error writing to file");
        exit(1);
      }
    }

    // Close the file and the socket
    close(fd);
    close(newsockfd);
  }

  // Close the server socket
  close(sockfd);
  return 0;
}
