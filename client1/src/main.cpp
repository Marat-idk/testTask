#include <iostream>
#include <cstring>
#include <cstdlib>  // для функций rand() и srand()
#include <ctime>    // для функции time()
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    // Равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

int main(){

    srand(static_cast<unsigned int>(time(0)));

    uint port = 11005;
    sockaddr_in addr;

    addr.sin_family = AF_INET;                    // задаём семейство адресов
    addr.sin_addr.s_addr = htonl(INADDR_ANY);       // задаём IP-адресс
    addr.sin_port = htons(port);                    // задвём порт

    int sockfd;

    if(( sockfd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0){
        puts("Can't create socket");
        return -1;
    }

    sockaddr_in clientAdrr;
    int clientLen = sizeof(clientAdrr);


    int array[] = {0, 0};
    for(int i = 0; i < 100; ++i){
        array[0]++;
        array[1] = getRandomNumber(0, 1);
        // отправяляем порядковый номер сообщения и 0 или 1
        sendto(sockfd, (const void *)array, sizeof(array), MSG_CONFIRM, (struct sockaddr *) &addr, sizeof(addr));

        printf("Recieved %d number is %d\n", array[0], array[1]);
        // отправляем с перерывами в 10мс
        usleep(10000);
    }


    close(sockfd);

    return 0;
}