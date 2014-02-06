#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h> //?

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#include "name_server.h"


int main(int argc, char* argv[]) {

	//initialize the server socket
	int socket_fd = init_server(SERVER_DEF_PORT);
	
	
	//listen for clients
	listen_for_clients(socket_fd);
}

/** Thread function for handling a new client that has connected to the server
	@param arg Pointer to a client struct, represnting the client that has connected
*/

void handle_client(void* arg) {

	int client_socket; // the socket that this client is connected on
	struct sockaddr_storage client_addr; // the address of the client

	char host_addr[NI_MAXHOST];
	
	int res = getnameinfo( (struct sockaddr *) &client_addr, sizeof(struct sockaddr_storage),
		host_addr, sizeof(host_addr), NULL, 0, NI_NAMEREQD);
		
	if (res) {
		printf("Error receiving hostname! \n");
	}
	else {
		printf("Connection started from %s!\n", host_addr);
	}

	//handle the client, 
		//add the client to list of connected clients
		//handle any requests from the client.
			//as of now, just requests for peers
		//remove the client from the list when it disconnects
		
		
		
}



/** Sets up the socket the server will use to listen on for new clients
	@param port cstring that contains the port to start the server on
	@return The descriptor of the socket that was created, or -1 if it failed
*/

int init_server(char* port) {
	
	struct addrinfo hints; // our address information
	struct addrinfo* server_info;
	struct addrinfo* server_connect;
	
	int socket_fd;
	
	int res = 0; //generic results
	
	//clear out the hints struct
	memset(&hints, 0, sizeof hints);
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; // TCP connection
	hints.ai_flags = AI_PASSIVE;
	
	res = getaddrinfo(NULL, port, &hints, &server_info);
	
	if (res) { // error occured
		printf("Error occured while retrieving our address info.\n Unable to start server. \n");
		return -1;
	}
	
	//bind the server to an address
	for (server_connect = server_info; server_connect != NULL;
		server_connect = server_connect->ai_next) {
			
		socket_fd = socket(server_connect->ai_family, server_connect->ai_socktype,
			server_connect->ai_protocol);
		
		if (socket_fd == -1) {
			//cannot bind to this address, try next
		}
		
		int yes = 1; // true int for setsocket opt
		res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		
		if (res == -1) {
			printf("Could not set socketopt, exiting \n");
			return -1;
		}	
		
		res = bind(socket_fd, server_connect->ai_addr, server_connect->ai_addrlen);
		if (res == -1) {
			close(socket_fd);
			printf("Error binding, trying again \n");
			continue;
		}
		break; // we bound successfuly
	}
	
	//free the memory that was allocated
	free(server_info);
	//free(server_connect); 
	
	return socket_fd;

}

/** Listens for clients 
	@param socket_fd The descriptor of the socket to listen on
	
*/

void listen_for_clients(int socket_fd) {

	int res = listen(socket_fd, SERVER_BACKLOG);

	//TODO: Add a way for the server to gracefully exit? Maybe?
	while (1) {
		printf("Waiting for a client to connect"); //debug statement mostly
		
		socklen_t sin_size;
		struct sockaddr_storage client_addr;
		int client_socket_fd; // socket fd for client
		
		client_socket_fd = accept(socket_fd, (struct sockaddr*) &client_addr,  &sin_size);
		
		if (client_socket_fd == -1) {
			printf("Error occured while trying to accept a client...\n");
			continue; // couldnt accept client, continue on
		}
		
		printf("Client has connected \n");
		
		//TODO: Start a thread that will handle the connected client
		
	}

}

void print_usage() {
	printf("Usage: \n");
	printf("\t ./name_server");
}
