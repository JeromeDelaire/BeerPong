/*
 * Author : Jérôme Delaire
 * Date : 12/05/1995
 * Description :
 * Communication with java client program via a mkfifo file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*-------------------------------------------------------------------------------*/
/*-----GLOBAL CONSTANTS----------------------------------------------------------*/

/* Input buffer size */
#define BUFFER_SIZE 4096

/* Fifo output file */
static const char* FIFO_OUTPUT = "dev/FIFO_C_TO_CPP" ;
static const char* FIFO_INPUT = "dev/FIFO_CPP_TO_C" ;

/*-------------------------------------------------------------------------------*/
/*-----FUNCTIONS DECLARATION-----------------------------------------------------*/

/* Listening thread */
void *listeningThread(void *arg);
/* Write str in output fifo (c to java) */
int writeFifo(char str[]);

/*-------------------------------------------------------------------------------*/
/*-----MAIN FUNCTION-------------------------------------------------------------*/

int main(int argc, const char** argv){
         /* Socket init */
	int sockPrim = socket(AF_INET, SOCK_STREAM, 0); //Socket TCP	
	struct sockaddr_in servAddr, cliAddr ;
	int addrLength=sizeof(struct sockaddr_in);
        int sockWork; // descripteur de la socket de travail
        char buffer[1024];
        int nbRecus = 1 ;

	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // association sur toutes les adresses
	servAddr.sin_family= AF_INET; // famille d’adresse IPv4
        servAddr.sin_port = htons(8090);
        bind (sockPrim, (struct sockaddr *) & servAddr, addrLength); // attribution du nom
        listen(sockPrim,10); // socket principale en mode écoute

	while(1){
	        sockWork = accept(sockPrim, (struct sockaddr *)& cliAddr,&addrLength);
		printf("accepted %d\n", sockWork);
		nbRecus = 1 ;

		while(nbRecus!=0){
			nbRecus = recv(sockWork, buffer, 1023, 0);
	                if (nbRecus ==-1) { perror("erreur bind()"); exit(1);}
	                buffer[nbRecus]='\0';
		        printf("%d : %s\n", nbRecus, buffer);
			writeFifo(buffer);
		}
	}

	return EXIT_SUCCESS ;
}

/*-------------------------------------------------------------------------------*/
/*-----FUNCTION DEFINITION-------------------------------------------------------*/

/* Listen input fifo (java to c) thread */
void *listeningThread(void *arg){

	FILE* fp_in ;
	char read_buf [BUFFER_SIZE];

	(void) arg ;
	
	while(1){
		printf("** C side **\nNOTE: Waiting to read data on java to c fifo...\n");

		 /* Open fifo for reading and check if fifo is open */
	        fp_in = fopen(FIFO_INPUT, "r");
        	if(fp_in!=NULL)
	                printf("** C side **\nNOTE: Fifo input opened successfully\n");
	        /* Else print errno */
        	else{
                	printf("** C side **\nERROR: %s\n", strerror(errno));
	                pthread_exit(EXIT_FAILURE);
		}
	
		memset(read_buf, 0, sizeof(read_buf)); // Put 0 in buf
		int oct = fread(read_buf, sizeof(read_buf), 1, fp_in);
		
		if(oct ==-1)
			printf("** C side **\nERROR: %s\n : %d\n", strerror(errno), fp_in);
		else
			printf("** C side **\nNOTE: Fifo Java to C reveived \"%s\"\n", read_buf);

	}
	fclose(fp_in);
	pthread_exit(0);
}

/* Write str in output fifo (c to java) */
int writeFifo(char str[]){

	FILE* fp_out ;

	printf("** C side **\nNOTE: Fifo c to java must be open by the java client, waiting...\n");

	/* Open fifo for writing and check if fifo is open */
	if((fp_out = fopen(FIFO_OUTPUT, "wb"))!=NULL) // Block until someone open this fifo on the other side
		printf("** C side **\nNOTE: Fifo c to java opened successfully\n");

	/* Else print errno */
	else{
		printf("** C side **\nERROR: %s\n", strerror(errno));
	}

	/* Write str in fifo */
	if(fwrite(str, 1, strlen(str), fp_out)!=strlen(str)){
		printf("** C side **\nERROR: %s\n", strerror(errno));
		fclose(fp_out);
		return -1 ;
	}

	else{
		fclose(fp_out);
		printf("** C side **\nNOTE: writing \"%s\" success\n", str);
		return 0 ;
	}

}


