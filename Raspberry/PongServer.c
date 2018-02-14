/*
 * Author : Jérôme Delaire
 * Date : 14/02/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wiringPiI2C.h>

/*-------------------------------------------------------------------------------*/
/*-----GLOBAL CONSTANTS----------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
/*-----FUNCTIONS DECLARATION-----------------------------------------------------*/

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
	int i2c_fd;

	/* Setup I2C */
	if((i2c_fd = wiringPiI2CSetup(0x00))==-1){
		printf("ERROR: I2C setup \n");
		exit(1);
	}
	printf("no\n");
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
			
			char toSend[8] ;
                        sprintf(toSend, "%08X", atoi(buffer));
                        printf("toSend %s\n", toSend);
                        char r[2], g[2], b[2];
                        sprintf(r, "%c%c", toSend[2], toSend[3]);
                        sprintf(g, "%c%c", toSend[4], toSend[5]);
			sprintf(b, "%c%c", toSend[6], toSend[7]);
                        printf("r : %s\n", r);
                        printf("g : %s\n", g);
                        printf("b : %s\n", b);
                        if(strtol(r, NULL, 16)!=0x00 || strtol(g, NULL, 16)!=0x00 || strtol(b, NULL, 16)!=0x00){
                                wiringPiI2CWrite(i2c_fd, strtol(r, NULL, 16));
                                wiringPiI2CWrite(i2c_fd, strtol(g, NULL, 16));
                                wiringPiI2CWrite(i2c_fd, strtol(b, NULL, 16));
                        }
		}
	}

	return EXIT_SUCCESS ;
}

/*-------------------------------------------------------------------------------*/
/*-----FUNCTION DEFINITION-------------------------------------------------------*/
