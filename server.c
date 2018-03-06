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
pthread_mutex_t writer;
int readerCount=0;
 #define MAXPENDING 5    /* Max connection requests */
 #define BUFFSIZE 32
 void Die(char *mess) { perror(mess); exit(1); }

/*The handler for echo connections is straightforward.
* All it does is receive any initial bytes available,
* then cycle through sending back data and receiving more data*/
#define SHMSZ 27;
struct MEMORY{
  char message[50];
};
struct MEMORY *memptr;
char lastModifiedDate[80];
void *HandleClient(int *);
int main(int argc, char *argv[]) {

static struct  MEMORY memory;
  char   *shmptr;
  int shmid;
   
  
  /*
   * Create, attach and initialize the memory segment
   */
  if ((shmid = shmget((int) getpid(), sizeof(memory),
              IPC_CREAT | 0600)) != -1 ) {
     
    if ( (shmptr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
      perror("shmptr -- parent -- attach");
      exit(1);
    }
    memcpy(shmptr, (char *) &memory, sizeof(memory));
    memptr = (struct MEMORY *) shmptr;
  }
  else {
    perror("shmid -- parent -- creation");
    exit(2);
  }
  int i;
for(i=0;i<100;i++)
{
    memptr->message[i] = '\0';
    }
 
 


/* Current Time*/
     time_t     now;
    struct tm *ts;
 
    /* Get the current time */
    now = time(NULL);
 
    /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
    ts = localtime(&now);
    strftime(lastModifiedDate, sizeof(lastModifiedDate), "%Y-%m-%d %H:%M:%S", ts);

  int serversock, clientsock;
  struct sockaddr_in echoserver, echoclient;

  if (argc != 2) {
      fprintf(stderr, "USAGE: echoserver <port>\n");
      exit(1);
   }
   /* Create the TCP socket */
   if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      Die("Failed to create socket");
    }
   /* Construct the server sockaddr_in structure */
   memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
   echoserver.sin_family = AF_INET;                  /* Internet/IP */
   echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
   echoserver.sin_port = htons(atoi(argv[1]));       /* server port */

   /* Bind the server socket */
   if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
       Die("Failed to bind the server socket");
    }
    /* Listen on the server socket */
   if (listen(serversock, MAXPENDING) < 0) {
       Die("Failed to listen on server socket");
    }

   /* Run until cancelled */
    while (1) {
    unsigned int clientlen = sizeof(echoclient);
    /* Wait for client connection */
    if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
        Die("Failed to accept client connection");
     }
     fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
     pthread_t thread;
     int *cls;
     cls=(int*)malloc(sizeof(int));
     *cls=clientsock;
     pthread_create(&thread,NULL,(void *)HandleClient,cls);
   }
}


void printHostIpfromName(char * siteName);



/*Provide with Site Name , Return the IP address of teh Site 
 *Using DNS Server to request IP for Site Name
 */
void printHostIpfromName(char * siteName){
  struct hostent *host;     /* host information */
  struct in_addr h_addr;    /* internet address */
  if ((host = gethostbyname(siteName)) == NULL) {
        fprintf(stderr, "(mini) nslookup failed on '%s'\n", siteName);
        exit(1);
     }

     h_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
     fprintf(stdout, "%s\n", inet_ntoa(h_addr));

}
void *HandleClient(int *cls) {
  int  sock=*cls;
  char buffer[BUFFSIZE];
  int received = -1;
  char option,sr;
  do{ 
 read(sock,&option,1);
 printf("%c\n",option);
 if( option =='0'){
   if(readerCount==0){
     pthread_mutex_lock(&writer);
   }
   readerCount++;
   write(sock,lastModifiedDate,80);
   read(sock,&sr,1);
   if(sr=='1'){ 
     write(sock,memptr->message,50);
   }
   readerCount--;
   if(readerCount==0)
     pthread_mutex_unlock(&writer);
 }
 else if (option=='1') {
   pthread_mutex_lock(&writer);
   printf("in option\n");
   read(sock,memptr->message,50);
   pthread_mutex_unlock(&writer);
   printf("Data printed%s\n",memptr->message);
    time_t     now;
    struct tm *ts; 
    /* Get the current time */
    now = time(NULL);
 
    /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
    ts = localtime(&now);
    strftime(lastModifiedDate, sizeof(lastModifiedDate), "%Y-%m-%d %H:%M:%S", ts);

 }
 else{
   printf("end\n");
   close(sock);
   pthread_exit(0);
}
 }while(1);
 
}
