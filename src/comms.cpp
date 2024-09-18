#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#include "comms.h"

#define SPI_MOSI_PIN 27
#define SPI_MISO_PIN 19
#define SPI_SCK_PIN 5
#define SPI_CS_PIN 18

#define LORA_RST_PIN 14
#define LORA_IRQ_PIN 26
#define LORA_BAND 866E6

int comms_init(void)
{
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN);

    LoRa.setPins(SPI_CS_PIN, LORA_RST_PIN, LORA_IRQ_PIN);

    int status = LoRa.begin(LORA_BAND); // Returns 1 if successful
    if (status != 1)
    {
        return 1;
    }

    LoRa.setTxPower(20); // 20 sets Tx to the highest power

    return 0;
}

int receive_packet(String *buffer)
{
    int packet_size = LoRa.parsePacket();

    if (packet_size > 0)
    {
        *buffer = LoRa.readString();
        return 0;
    }
    else
    {
        return 1;
    }
}

void send_command(const command_ids command, String data)
{
    String message = command + data;

    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
}