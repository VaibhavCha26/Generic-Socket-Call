#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

/* macros */ 
#define USR 1
#define ROOT 0

/* escalation options and their available sockets for calling basically */
typedef enum usr_sockets_options{
  usr_exp
}socket_options;

typedef enum root_sockets_options{
  root_exp
}root_sockets_options;

// =================================== ADDR TO SEND TO 

typedef struct sockaddr_in{
  // destination_ip
  // destination_port
}sockaddr_in;

// Not needed for UDP ? i guess but why ---- shooting in the wild anyway lol.

// ===================================


// here we define the sockets individually.
struct usr_exp{
  char usr_exp_buffer[1024*32];
  int socket_flags;
  // how to add domain wtf ? 
};

struct root_exp{
  char root_exp_buffer[1024*36];
  int socket_flags;
};


/* Definitions */ 
typedef struct socket_metadata{
  //enum sockets_options sockets_available;
  int escalation;
  void* file_location;
  int socket_type;

  int domain;
  int type;
  int protocol;
  
}socket_metadata;



//struct socket_calls socket_calling(char* argv[], int* data_fd){
//  enum sockets_options sockets_available;
// switch (*argv[1]) { case ROOT : 
//      return (struct socket_calls){
//        .escalation =  0,
//        .fd = *data_fd,
//      };
//    
//    case USR : 
//      return (struct socket_calls){
//        .escalation = 1,
//        .fd = *data_fd,
//
//};
//    
//
//    default : break; // change this later;
//  };
//};

int socket_handling(struct socket_metadata);

int data_handling(char* file,int flags,char* buffer, struct socket_metadata curr_sock_meta);
void data_push(char* buffer, struct socket_metadata curr_sock_meta);



// ex --> [name] esc socket_type file_location addr;
int main(int argc,char* argv[]){
  struct socket_metadata curr_sock_meta;

  curr_sock_meta.file_location = argv[3];
  curr_sock_meta.escalation = *argv[1];
  curr_sock_meta.socket_type = *argv[2];

  switch(argc){
    case 6 : break;
    default : perror("Missed something - refer to docs: "); exit(1);
  }
  

  socket_handling(curr_sock_meta);

}



int socket_handling(struct socket_metadata curr_sock_meta){
  switch (curr_sock_meta.escalation) {
    case 0 : {

      switch (curr_sock_meta.socket_type) {

        case 0 : {
          struct usr_exp usr_exp;
          
          usr_exp.socket_flags = O_RDONLY;
          curr_sock_meta.domain = AF_INET;
          curr_sock_meta.type =  SOCK_STREAM;
          curr_sock_meta.protocol = 0;

          int socket_fd = socket(AF_INET,SOCK_STREAM,0);
          if(socket_fd < 0){
            perror("error in opening socket btw: "); 
            exit(1);
          }

          int data_handling_status = data_handling(curr_sock_meta.file_location,
                        usr_exp.socket_flags,
                        usr_exp.usr_exp_buffer,
                        curr_sock_meta
                        );

          if(data_handling_status != 0){
            perror("data_handling error: ");
            exit(1);
          }

        }

        default : exit(1);
      }
      
    }


    case 1 : {

      switch (curr_sock_meta.socket_type) {

        case 0 : {
          struct root_exp root_exp;
          // root_exp.socket_flags = O_RDONLY | O_WRONLY
          data_handling(curr_sock_meta.file_location,
                        root_exp.socket_flags,
                        root_exp.root_exp_buffer,curr_sock_meta
                        );
        }

        default : exit(1);
      }
    }

    default : exit(1); // later bruh
  }


  return 0;
}

// TCP --- CONNECT a lot of stuff and bruh.
// UDP --> Skips connect;
int data_handling(char* file,int flags,char* buffer, struct socket_metadata curr_sock_meta)
{
  if(file == NULL){
    perror("location not defined");
    exit(1);
  }

  int file_fd = open(file,flags);
  if(file_fd == -1){
    perror("opening error");
    exit(1);
  }

  ssize_t total_read = 0;
  ssize_t size_read = 0;

  if( (size_read = read(file_fd,
                        buffer,sizeof(buffer) - 1 )) > 0 ) {

    total_read += size_read;
    
  }
  else {
    if(total_read > sizeof(buffer - 1)){
      // debug info T-T 
      buffer[sizeof(buffer)] = '\0';
      total_read = 0; // reset the total_read for the next set;
    }
    buffer[total_read] = '\0';
  }
  return 0;
}

// handling the difference between addr and handling stuff -- something like TCP AND UDP and stuff;
void data_push(char* buffer, struct socket_metadata curr_sock_meta)
{
  
}




