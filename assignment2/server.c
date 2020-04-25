// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 8080
int main(int argc, char const *argv[])
{

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int status;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hola from server";
    pid_t child_pid;
    uid_t no_uid = 65534;

    if(argc > 1 && strcmp(argv[1],"exec")==0)
    {

      printf("|----->INSIDE EXEC\n");
      int socket = (int) *argv[2];
      valread = read( socket , buffer, 1024);
      printf("        %s\n",buffer );
      send(socket , hello , strlen(hello) , 0 );
      printf("        Hello message sent\n");
      printf("|----->EXITING EXEC\n");
      return 0;
    }
    printf("|-->SERVER RUNNING..\n");


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    if ((child_pid =fork()) == 0) {
        printf("|---->INSIDE CHILD\n");

        // status = setuid(no_uid);
        // printf("%d \n", status);
        // printf("%d \n", getuid());
        if (status = setuid(no_uid) < 0) {
          perror("Couldn't drop privilege");
          exit(EXIT_FAILURE);
        }
        char *args[] = {"./server", "exec", (char *)&new_socket, NULL};
        execv(args[0], args);
        printf("|---->EXITING CHILD\n");
    }
    else {
        int return_status;
        waitpid(child_pid, &return_status, 0);
        if(return_status == 0)
        {
            printf("|---->Child process terminated normally\n");
        } else
        {
            printf("Child process terminated with an error\n");
        }
        printf("|-------------------\n");
        return 0;
    }

}
