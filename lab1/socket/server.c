#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define QLEN 10
#define TRUE 1
#define FALSE 0
#define VECTOR_SIZE 500000
#define MAX_PAGE_BYTES 65536

typedef int boolean;

void handle_critical_failure(int rt, char * message) {
  if (rt < 0) {
    fprintf(stdout, "%s\n", message);
    exit(1);
  }
}

void set_server_addr(struct sockaddr_in * serv_addr, char * addr, char * port) {
  int errcode;
  struct sockaddr_in * temp_addr;
  struct addrinfo hints, * result;

  memset(&hints, 0, sizeof (hints));

  hints.ai_family = PF_UNSPEC;
  hints.ai_flags |= AI_CANONNAME;
  hints.ai_socktype = SOCK_STREAM;

  errcode = getaddrinfo(addr, NULL, &hints, &result);

  if (errcode != 0) {
    perror("getaddrinfo");
    return;
  }

  serv_addr->sin_family = AF_INET;
  temp_addr = (struct sockaddr_in *) result->ai_addr;
  serv_addr->sin_addr = temp_addr->sin_addr;
  serv_addr->sin_port = htons(atoi(port));
}

void get_min_max_value(float * v, float * result) {
  result[0] = v[0];
  result[1] = v[0];

  for (int i = 1; i < VECTOR_SIZE; i++) {
    if (v[i] < result[0])
      result[0] = v[i];

    if (v[i] > result[1])
      result[1] = v[i];
  }
}

void answer_client(int descriptor) {
  float * bufin;
  float answer[2];
  ssize_t vector_bytes = VECTOR_SIZE * sizeof(float);

  bufin = (float *) malloc(vector_bytes);

  ssize_t total_read_bytes = 0;

  while (total_read_bytes < vector_bytes) {
    ssize_t to_read = (vector_bytes - total_read_bytes) >= MAX_PAGE_BYTES ? MAX_PAGE_BYTES: (vector_bytes - total_read_bytes);

    ssize_t read_bytes = read(descriptor, ((void *) bufin) + total_read_bytes, to_read);

    total_read_bytes += read_bytes;
  }

  get_min_max_value(bufin, answer);

  send(descriptor, answer, 2 * sizeof(float), 0);

  free(bufin);
}

void print_receive_req(struct sockaddr_in client_addr, char * msg) {
  fprintf(
    stdout,
    "%s => %s:%d\n",
    msg,
    inet_ntoa(client_addr.sin_addr),
    ntohs(client_addr.sin_port)
  );
}

int main(int argc, char * argv[]) {
  struct sockaddr_in server_addr;
  int sd, bind_res, listen_res;

  if (argc < 3) {
    printf("You must provide 2 arguments: the server address and port \n");
    exit(1);
  }

  memset((char *) &server_addr, 0, sizeof(server_addr));

  // used to allow "localhost" to be passed as the server IP (argv[1])
  set_server_addr(&server_addr, argv[1], argv[2]);

  sd = socket(AF_INET, SOCK_STREAM, 0);

  handle_critical_failure(sd, "Fail to create the socket\n");

  bind_res = bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));

  handle_critical_failure(bind_res, "Bind fail\n");

  listen_res = listen(sd, QLEN);

  handle_critical_failure(listen_res, "Fail to listen on socket\n");

  fprintf(stdout, "Server listening in %s:%s...\n\n", argv[1], argv[2]);

  while (TRUE) {
    int accept_sd;
    socklen_t addr_len;
    struct sockaddr_in client_addr;

    addr_len = sizeof(client_addr);
    accept_sd = accept(sd, (struct sockaddr *) &client_addr, &addr_len);

    if (accept_sd < 0) {
      fprintf(
        stdout,
        "\n%s:%d: Accept fail\n",
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port)
      );

      continue;
    }

    print_receive_req(client_addr, "Request received");

    answer_client(accept_sd);

    print_receive_req(client_addr, "Request answered");

    fprintf(stdout, "\n");

  }

  return 0;
}
