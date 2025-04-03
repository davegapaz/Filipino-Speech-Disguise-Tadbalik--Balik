#include "headers.h"

int main(void)
{
    char ip[2000];

    //asks for ip address
    printf("Enter IP of the server to connect: ");
    fgets(ip,2000,stdin);
    ip[strcspn(ip,"\n")] =0;
        

    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    
    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    printf("Socket created successfully\n");
    
    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    // Send a connection request to the server, which is waiting at accept():
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");
    printf("bat ganon?\n");
    while(1){
        // Get input from the user:
        printf("Enter message: ");
        fgets(client_message,2000,stdin);
        
        //remove newline from client message
        client_message[strcspn(client_message,"\n")] =0;

        if(!strcmp(client_message,"stop")){
            printf("Goodbye.\n");
            // Close the socket:
            close(socket_desc);
            return 0;
        }

        // Send the message to server:
        if(send(socket_desc, client_message, strlen(client_message), 0) < 0){
            printf("Unable to send message\n");
            return -1;
        }
        
        // Receive the server's response:
        if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
            printf("Error while receiving server's msg\n");
            return -1;
        }
        
        
        printf("Server's response: %s\n",server_message);
        

        // Clean buffers:
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
    }
    
    return 0;
}