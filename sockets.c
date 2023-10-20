#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 1024
#define PORT 2210



int main(){
    printf("Listen Test %d:\n", PORT);
    printf("    netcat -u 127.0.0.1 %d\n", PORT);


    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(struct sockaddr_in));

    while(1) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN];
        int bytesRx = recvfrom(socketDescriptor,
            messageRx, MAX_LEN, 0, (struct sockaddr*) &sinRemote, &sin_len);

        int terminalIdx = (bytesRx < MAX_LEN) ? bytesRx: MAX_LEN -1;
        messageRx[terminalIdx] = 0;
        printf("message received (%d bytes): \n\n '%s'\n", bytesRx, messageRx);

        int incMe = atoi(messageRx);

        char messageTx[MAX_LEN];
        sprintf(messageTx, "Math %d + 1 = %d\n", incMe, incMe+1);

        sin_len = sizeof(sinRemote);
        sendto( socketDescriptor,
            messageTx, strlen(messageTx), 0,
            (struct sockaddr*) &sinRemote, sin_len);
    }
    close(socketDescriptor);

}






