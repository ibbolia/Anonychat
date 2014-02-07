#include       <stdlib.h>
#include       <sys/socket.h>
#include       <netinet/in.h>
#include       <arpa/inet.h>
#include       <stdio.h>
#include       <string.h>
#include       <netdb.h>
#include       <stdlib.h>
#include       <time.h>

#define         BUFFER_SIZE     512

main (int argc, char **argv) {
  
  int portNo; // Your port number
  int fd;
  struct sockaddr_in sa;
  int lsa = sizeof(sa);

  char IPAddress[100];
  char ip_input_buffer[BUFFER_SIZE];
  char ip_output_buffer[BUFFER_SIZE];

  // Input arguements
  if ( argc < 1 )  {
        printf( "Usage: %s <hostname or IP Address, Port Number>\n", argv[0]);
        exit(0);
  }

  if ( argc < 3) {
      printf( "The program expects two arguments\n" );
      printf( "Browser <ipaddress> <port number>\n" );
      exit(0);
  }

  portNo = atoi(argv[1]);
  printf("Your port: %d\n", portNo);

  if ((fd = socket ( AF_INET, SOCK_STREAM, 0 )) < 0)   {
      printf( "The socket call failed\n");
      exit(1);
  }

  // set up the sockaddr_in structure.  This uses, among other things the 
  // port you've just entered.
  sa.sin_family       = AF_INET;
  sa.sin_port         = htons(portNo);     // client & server see same port
  // Use 127.0.0.1 for local host
  //printf("Your port: %d\n", sa.sin_port );
  sa.sin_addr.s_addr  = inet_addr(IPAddress); // the kernel assigns the IP ad

  if (connect(fd, (struct sockaddr *)&sa, lsa) == -1)  {
      perror( "Failed to connect");
      exit(2);
  }

  // Connection successful!
  printf("Connected\n");

  // First clean out the output buffer (bzero is my recommendation), and then
  //   copy the get message into it.

  bzero(ip_output_buffer, BUFFER_SIZE);

  bzero( ip_input_buffer, sizeof(ip_input_buffer) );
  while(0 == 0) {
    if (recv( fd, ip_input_buffer, sizeof(ip_input_buffer) - 2, 0 ) <= 0 ) {
      printf( "Error encountered: Terminating\n");
      exit(0);
    } else {
      printf( "String received: %s\n", ip_input_buffer);
    }
  }
}
