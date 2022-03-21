#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include "kafka/KafkaConsumer.h"


int main() {

    std::string brokers = "localhost:9092";
    kafka::Topic topic  = "testTask";

    try {

        // создаем объект конфигурации
        kafka::Properties props ({
            {"bootstrap.servers", brokers},
        });

        kafka::clients::KafkaConsumer consumer(props);

        consumer.subscribe({topic});

        auto lastTimeCommitted = std::chrono::steady_clock::now();

        // читаем сообщения из топика
        std::cout << "Reading messages from topic: " << topic << std::endl;
        bool running = true;
        std::string received;
        while (running) {
            auto records = consumer.poll(std::chrono::milliseconds(10));
            for (const auto& record: records) {
                // если получили пустое сообщение, то выходим
                if (record.value().size() == 0) {
                    running = false;
                    break;
                }

                if (!record.error()) {
                    received = record.value().toString();

                    std::cout << "Recieved " << received.substr(0, received.find(" ")) << " "
                    << "number is " << received.substr(received.find(" "), received.size()) << std::endl;

                } else {
                    std::cerr << record.toString() << std::endl;
                }
                received.erase();
            }
        }

    } catch (const kafka::KafkaException& e) {
        std::cerr << "Something went wrong: " << e.what() << std::endl;
    }

    return 0;
}