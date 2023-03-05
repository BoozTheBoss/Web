#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "base64converter"


int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  char *receivedData;
  char *fileContentBase64;

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

    receivedData = 0;  // TODO: get body

    size_t fileSize64 = 0; // TODO: get size from request // allocate for targetFilePtr


    // Read the fileContentBase64 from the client
    bzero(fileContentBase64, fileSize64);
    int fileSize = read(newsockfd, fileContentBase64, fileSize64);
    printf("printing filenames: %s", fileContentBase64);
    printf("printing size: %d", fileSize);
    if (fileSize < 0) {
      perror("Error reading receivedData");
      exit(1);
    }

    bzero(file_content, fileSize);
    file_content = base64_decode(fileContentBase64, fileSize64, &fileSize);

    // for(int i=0;i<fileSize;i++) {
    // }

    targetFilePtr = fopen("Target.wav", "w");

    fwrite(file_content, 1, fileSize, targetFilePtr);

    fclose(targetFilePtr);

    close(newsockfd);
  }

  // Close the server socket
  close(sockfd);
  return 0;
}
