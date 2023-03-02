#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 102400

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
    uint32_t clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
      perror("Error accepting connection");
      exit(1);
    }

    // Read the filename from the client
    bzero(filename, MAXBUF);
    int n = read(newsockfd, filename, MAXBUF);
    printf("printing filenames: %s", filename);
    printf("printing size: %d", n);
    if (n < 0) {
      perror("Error reading filename");
      exit(1);
    }

    // Note: we were able to send a txt file into example.txt

    // TODO: Look in http spec to find out when body starts
    // TODO: return http response to socket

    FILE *file = fopen("example.txt", "w+");
    if (file == NULL) {
    perror("fopen");
    return 1;
    }

    // Write some text to the file
    fputs(filename, file);

    // Close the file
    fclose(file);

    close(newsockfd);
  }

  // Close the server socket
  close(sockfd);
  return 0;
}
