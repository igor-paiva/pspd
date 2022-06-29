#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define VECTOR_SIZE 500000
#define MAX_BYTES_PER_WRITE 65536

void handle_failure(int rt, char message[]) {
  if (rt < 0) {
    fprintf(stdout, "%s\n", message);
    exit(1);
  }
}

void set_server_addr(struct sockaddr_in * serv_addr, char * addr, char * port) {
  int errcode;
  struct addrinfo hints, *result;

  memset (&hints, 0, sizeof (hints));

  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;

  errcode = getaddrinfo(addr, NULL, &hints, &result);

  if (errcode != 0) {
    perror ("getaddrinfo");
    return;
  }

  serv_addr->sin_family = AF_INET;
  struct sockaddr_in * temp_addr = (struct sockaddr_in *) result->ai_addr;
  serv_addr->sin_addr = temp_addr->sin_addr;
  serv_addr->sin_port = htons(atoi(port));
}

float f_aleat() {
  return ((float) rand() / (float) RAND_MAX) * 100;
}

void fill_array(float * array, long int size) {
  srand((unsigned int) time(NULL));

  for (int i = 0; i < size; i++) {
    float random_f = f_aleat();

    array[i] = sqrt((i - (random_f / 2)) * (i - (random_f / 2)));
  }
}

void send_request(int descriptor, float * v) {
  ssize_t written_bytes = 0;
  ssize_t vector_bytes = VECTOR_SIZE * sizeof(float);

  while (written_bytes < vector_bytes) {
    ssize_t to_write = (vector_bytes - written_bytes) >= MAX_BYTES_PER_WRITE ? MAX_BYTES_PER_WRITE : (vector_bytes - written_bytes);

    ssize_t bytes_written = write(descriptor, ((void *) v) + written_bytes, to_write);

    if (bytes_written == -1) {
      fprintf(stdout, "Write fail\n");
      exit(1);
    }

    written_bytes += bytes_written;
  }
}

int main(int argc, char * argv[]) {
  struct sockaddr_in server_addr;
  int sd, connect_res;
  float * v;
  float answer[2];

  if (argc < 2) {
    fprintf(
      stdout,
      "São necessários 2 parâmetros. Formato:\
       ./bin <ip servidor> <porta servidor>\n"
    );

    exit(1);
  }

  v = (float *) malloc(VECTOR_SIZE * sizeof(float));

  fill_array(v, VECTOR_SIZE);

  memset((char *) &server_addr, 0, sizeof(server_addr));

  // used to allow "localhost" to be passed as the server IP (argv[1])
  set_server_addr(&server_addr, argv[1], argv[2]);

  sd = socket(AF_INET, SOCK_STREAM, 0);

  handle_failure(sd, "Não foi possível criar o socket\n");

  connect_res = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));

  handle_failure(connect_res, "Não foi possível conectar");

  // send the request to the server
  send_request(sd, v);

  // wait for the server response
  recv(sd, answer, 2 * sizeof(float), 0);

  fprintf(stdout, "Menor: %f - Maior: %f\n", answer[0], answer[1]);

  free(v);

  return 0;
}
