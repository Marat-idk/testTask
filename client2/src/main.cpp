#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <kafka/KafkaProducer.h>

int main(){
    using namespace kafka::clients;
    // связь 1 клиента со 2

    uint port = 11005;
    sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;                      // задаём семейство адресов
    addr.sin_addr.s_addr = htonl(INADDR_ANY);       // задаём IP-адресс
    addr.sin_port = htons(port);                    // задвём порт

    int sockfd;

    if(( sockfd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0){
        puts("Can't create socket");
        return -1;
    }

    if( bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0){
        puts("Can't bind socket with port");
        return -1;
    }

    sockaddr_in clientAdrr;
    int clientLen = sizeof(clientAdrr);
    bzero(&clientAdrr, clientLen);
    int number;
    int i;
    int bytesRecvied = 0;

    int buffer[16];


    std::string brokers = "localhost:9092";
    kafka::Topic topic  = "testTask";

    try {

        // задаём объект с конфигурацией
        kafka::Properties props({
            {"bootstrap.servers",  brokers},
            {"enable.idempotence", "true"},
        });

        KafkaProducer producer(props);

        std::string msgForClient3;


        do{
            // получаю порядковый номер и число
            bytesRecvied = recvfrom(sockfd, (int *)buffer, sizeof(buffer), MSG_WAITALL, (sockaddr *)&clientAdrr, (socklen_t *)&clientLen);

            msgForClient3 = std::to_string(buffer[0]) + " " + std::to_string(buffer[1]); 
            auto record = producer::ProducerRecord(topic,
                                                    kafka::NullKey,
                                                    kafka::Value(msgForClient3.c_str(), msgForClient3.size()));

                // Send the message.
                try {
                    producer::RecordMetadata metadata = producer.syncSend(record);
                    std::cout << "Message delivered: " << metadata.toString() << std::endl;
                } catch (const kafka::KafkaException& e) {
                    std::cerr << "Message delivery failed: " << e.error().message() << std::endl;
                }
            msgForClient3.clear();
        }while(bytesRecvied != 0);

    } catch (const kafka::KafkaException& e) {
        std::cerr << "Something went wrong: " << e.what() << std::endl;
    }

    return 0;
}
