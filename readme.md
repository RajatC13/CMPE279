# CMPE 279 Assignments


## Assignment 1

We achieve privilege separation in server by forking and dropping privileges. The parent process maintains the root privilege; while the child process drops the privileges and talks on the network with reduced privileges.

### Running The Server
`$ gcc Myserver.c -o server`
`$ sud0 ./server`

### Running The Client
`$ gcc client.c -o client`
`$ ./server`

## Assignment 2

This assignment is based on the assignment 2. We achieve address space randomization in server by exec'ing the same server code after forking and dropping privileges. The exec'ed server then uses the file descriptor of the socket passed-on to it to talk to the client.

### Running The Server
`$ gcc server.c -o server`
`$ sud0 ./server`

### Running The Client
`$ gcc client.c -o client`
`$ ./server`
