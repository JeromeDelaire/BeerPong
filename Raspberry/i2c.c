#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringPiI2C.h>

/* Input buffer size */
#define BUFFER_SIZE 4096

static const char* FIFO_INPUT = "dev/FIFO_C_TO_CPP" ;

void *listeningThread(void *arg);


int main()
{

	pthread_t listening_thread ;

	if(pthread_create(&listening_thread, NULL, listeningThread, NULL) == -1){
		perror("pthread_create\n");
		return EXIT_FAILURE ;
	}


   	if (pthread_join(listening_thread, NULL)) {
   		perror("pthread_join");
    		return EXIT_FAILURE;
    	}

	return EXIT_SUCCESS ;

}

/* Listen input fifo (java to c) thread */
void *listeningThread(void *arg){

        FILE* fp_in ;
        char read_buf [BUFFER_SIZE];
	int i2c_fd, result ;
	int color ;

	i2c_fd = wiringPiI2CSetup(0x00);

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
                else{
                        printf("** C side **\nNOTE: Fifo Java to C reveived %d oct \"%6X\"\n", oct, atoi(read_buf));
			char toSend[8] ;
			sprintf(toSend, "%08X", atoi(read_buf));
			printf("toSend %s\n", toSend);
			char r[2], g[2], b[2];
			sprintf(r, "%c%c", toSend[2], toSend[3]);
			sprintf(g, "%c%c", toSend[4], toSend[5]);
			sprintf(b, "%c%c", toSend[6], toSend[7]);

			printf("r : %s\n", r);
			printf("g : %s\n", g);
			printf("b : %s\n", b);
			if(strtol(r, NULL, 16)!=0x00 || strtol(g, NULL, 16)!=0x00 || strtol(b, NULL, 16)!=0x00){
				result = wiringPiI2CWrite(i2c_fd, strtol(r, NULL, 16));
				result = wiringPiI2CWrite(i2c_fd, strtol(g, NULL, 16));
				result = wiringPiI2CWrite(i2c_fd, strtol(b, NULL, 16));
			}
		}

        }
        fclose(fp_in);
        pthread_exit(0);
}
