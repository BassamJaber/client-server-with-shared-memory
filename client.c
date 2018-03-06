#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>          /* hostent struct, gethostbyname() */
#include <arpa/inet.h>      /* inet_ntoa() to format IP address */
#include <netinet/in.h>     /* in_addr structure */
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#define BUFFSIZE 32
void endClient(int);
void Die(char *mess) { perror(mess); exit(1); }
char lastModifiedDate[80];
typedef struct Memory{
  char message[50];
}memory;
memory sharedmem;
int main(int argc, char *argv[]) {
  strcpy(lastModifiedDate,"1992-07-18 12:30:00");

int sock;
struct sockaddr_in echoserver;
char buffer[BUFFSIZE];
unsigned int echolen;
int received = 0;

if (argc != 4) {
	fprintf(stderr, "USAGE: TCPecho <server_ip> <word> <port>\n");
        exit(1);
 }
  /* Create the TCP socket */
  /*The arguments to the socket() call decide the type of socket:
  * PF_INET just means it uses IP (which you always will);
  * SOCK_STREAM and IPPROTO_TCP go together for a TCP socket. 
  */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
     Die("Failed to create socket");
   }

/*The value returned is a socket handle, which is similar to a file handle; 
*specifically, if the socket creation fails, it will return -1
* rather than a positive numbered handle. */

  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(argv[1]);  /* IP address */
  echoserver.sin_port = htons(atoi(argv[3]));       /* server port */
  /* Establish connection */
   if (connect(sock,(struct sockaddr *) &echoserver,sizeof(echoserver)) < 0) {
              Die("Failed to connect with server");
    }

char option,serverTime[80],sr;
 time_t time1,time2;   
/* Send the word to the server */


/* Receive the word back from the server */
   while(1){
   printf("Enter 0 to read or 1 to write to shared memory or 2 to end\n");
   //fflush(stdin);
   //fflush(stdout);
   scanf(" %c",&option);
   printf("before write..\n");
   write(sock,&option,1);
   //printf("write..");
   if(option== '0'){
   //read last modified date if read
     //printf("before read");
     read(sock,&serverTime,80);
     //printf("after read");
      printf("server time%s\n",serverTime);
     printf("client time%s\n",lastModifiedDate);
     if(strcmp(serverTime,lastModifiedDate)>0){
       printf("getting data from server\n");
       sr='1';
       write(sock,&sr,1);
       read(sock,&(sharedmem.message),50);
       strcpy(lastModifiedDate,serverTime);
       printf("%s\n",sharedmem.message);
      }
     else{
       printf("getting data from local copy\n");
       strcpy(lastModifiedDate,serverTime);
       printf("%s\n",sharedmem.message);
     }
   }
   else if (option=='1'){
   printf("send message\n");
    scanf("%s",sharedmem.message);
   write(sock,&(sharedmem.message),50);
   }
   else{
     printf("end");    
 endClient(sock);
   }
   }
return 0;
}
void endClient(int sock){
close(sock);
   exit(0);
}       
