#include "headers.h"

int isVowel(char letter){
    if (letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u'){
        return 1;
    }
    return 0;
}



char * Tadbalik(char tadbalik[], char client_message[], int len){
    printf("function\n");
    int temp = len;
    // char syll[50];
    int cnt = 0;
    
    for(int i=len-1; i>0; i--){
        
        printf("%c\n", client_message[i]);
        if(isVowel(client_message[i])){
            //check if element on its left is a consonant
            if(!isVowel(client_message[i-1])){
                //i-1 to i+1 using sub.str
                //getting last syllable
                for(int j=i-1; j<len; j++){
                    printf("%d\n",j);
                    tadbalik[cnt++] = client_message[j];
                    
                }
                temp = i-1;
                printf("gg\n");
                //additing it to at the start of the word (process of baliktad)
                for(int j=0; j<temp; j++){
                    printf("%d\n",j);
                    tadbalik[cnt++] = client_message[j];
                }

                return tadbalik;
                temp = i-1;

                i=i-2;
            }
        }
    }

    return tadbalik;
}

int main(void)
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    /* Create a socket that returns a socket descriptor; this will be used
       to refer to the socket later on in the code. The server-side code
       keeps the address information of both the server and the client in
       a variable of type sockaddr_in, which is a struct.
    */
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Initialize the server address by the port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind the socket descriptor to the server address (the port and IP):
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    
    // Turn on the socket to listen for incoming connections:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");
    
    /* Store the client’s address and socket descriptor by accepting an
       incoming connection. The server-side code stops and waits at accept()
       until a client calls connect().
    */
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, (socklen_t *)&client_size);
    
    if (client_sock < 0){
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    while (1){
        // Receive client's message:
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }

        client_message[strcspn(client_message,"\n")] =0;
        

        printf("Msg from client: %s\n", client_message);
        
        if(strcmp(client_message,"EXIT") == 0){
            printf("Closing the socket");
            // Closing the socket:
            close(client_sock);
            close(socket_desc);
            return 0;
        
        }
        int len = strlen(client_message);

        char tadbalik[len+1]; 
        
        Tadbalik(tadbalik, client_message, len);


        // Respond to client:
        strcpy(server_message, tadbalik);

        
        if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
        }
        
        // Clean buffers:
        memset(server_message, '\0', sizeof(server_message));
        memset(tadbalik, '\0', sizeof(tadbalik));
        memset(client_message, '\0', sizeof(client_message));
    }
    
    
    return 0;
}
