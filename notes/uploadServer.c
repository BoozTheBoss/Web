#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 8000000

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;

void build_decoding_table()
{

  decoding_table = malloc(256);

  for (int i = 0; i < 64; i++)
    decoding_table[(unsigned char)encoding_table[i]] = i;
}

int getBase64_size(size_t input_size)
{
  return 4 * ((input_size + 2) / 3);
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length)
{

  if (decoding_table == NULL)
    build_decoding_table();

  if (input_length % 4 != 0)
    return NULL;

  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=')
    (*output_length)--;
  if (data[input_length - 2] == '=')
    (*output_length)--;

  unsigned char *decoded_data = malloc(*output_length);
  if (decoded_data == NULL)
    return NULL;

  for (int i = 0, j = 0; i < input_length;)
  {

    uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

    uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

    if (j < *output_length)
      decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
    if (j < *output_length)
      decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
    if (j < *output_length)
      decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
  }

  return decoded_data;
}

int main(int argc, char *argv[])
{
  printf("Starting upload server\n");

  int sockfd, newsockfd, portno, clilen;

  struct sockaddr_in serv_addr, cli_addr;

  const char *empty_line_token = "\r\n\r\n";
  const char *new_line_token = "\r\n";

  char receivedData[MAXBUF];

  FILE *targetFilePtr;

  // Create a new socket
  int option = 1;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  if (sockfd < 0)
  {
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
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("Error binding socket");
    exit(1);
  }

  // Listen for incoming connections
  listen(sockfd, 5);
  printf("Listening on port %d\n", portno);

  while (1)
  {
    // Accept incoming connections
    uint32_t clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
      perror("Error accepting connection\n");
      exit(1);
    }

    // Read the fileContentBase64 from the client
    bzero(receivedData, MAXBUF);
    int requestSize = read(newsockfd, receivedData, MAXBUF);
    if (requestSize < 0)
    {
      perror("Error reading request\n");
      exit(1);
    }

    char *receivedDataContent;

    receivedDataContent = strstr(receivedData, empty_line_token);
    receivedDataContent += strlen(empty_line_token);

    if (receivedDataContent == NULL)
    {
      perror("Could not read body\n");
    }

    size_t fileSize64 = strlen(receivedDataContent);
    // size_t realFileSize = getBase64_size(fileSize64);
    size_t realFileSize; // wurst?

    printf("Received content fileSize64 %zu\n", fileSize64);
    printf("Received content realFileSize %zu\n", realFileSize);

    unsigned char *decodedContent;
    bzero(receivedDataContent, realFileSize);

    decodedContent = base64_decode(
        receivedDataContent, fileSize64, &realFileSize);

    printf("New realFileSize %zu\n", realFileSize);

    // Write decoded content to the file
    targetFilePtr = fopen("data.mp3", "w");
    if (targetFilePtr == NULL)
    {
      perror("Error opening file");
      return 1;
    }

    // fileSize64 = 0;
    // fwrite(decodedContent, 1, fileSize64, targetFilePtr); // fileSize64 too large
    fwrite(decodedContent, 1, realFileSize, targetFilePtr); 
    printf("new fileSize64 %zu\n", fileSize64);

    // Close the file
    fclose(targetFilePtr);
    close(newsockfd);
  }

  close(sockfd);
  return 0;
}