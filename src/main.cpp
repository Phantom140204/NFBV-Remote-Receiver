#include <Arduino.h>
#include <LoRa.h>

#define SPI_MOSI_PIN 27
#define SPI_MISO_PIN 19
#define SPI_SCK_PIN 5
#define SPI_CS_PIN 18

#define LORA_RST_PIN 14
#define LORA_IRQ_PIN 26
#define LORA_BAND 866E6

String buffer;
unsigned long prev_ticks = millis();
int mode = 0;

// Communication Protocol -

// Send a string in the following format -
// <Mode>:<Data>, where
// Mode 0 = Return to Home           , data = float, float (lat, long)
// Mode 1 = Abort mission            , data = NULL
// Mode 2 = Up button                , data = NULL
// Mode 3 = Down button              , data = NULL
// Mode 4 = Left button              , data = NULL
// Mode 5 = Right button             , data = NULL
// Mode 6 = AUV battery              , data = int (battery %)
// Mode 7 = AUV internal pressure    , data = uint32 (raw sensor data)
// Mode 8 = AUV internal temperature , data = int (temperature in celsius)
// Mode 9 = AUV GPS location         , data = float, float (lat, long)

typedef enum
{
  RETURN_TO_HOME,
  ABORT_MISSION,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  AUV_BATTERY,
  AUV_INTERNAL_PRESSURE,
  AUV_INTERNAL_TEMPERATURE,
  AUV_GPS,
} command_ids;

int comms_init(void)
{
  Serial.begin(115200);

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

void send_command(const command_ids command, String data)
{
  String message = command + data;
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}

void parse_received_data()
{
  // String manipulation for getting mode and data
  int mode = (int)buffer[0] - (int)'0';
  String data = "";
  for (int i = 1; i < buffer.length(); i++)
  {
    data += buffer[i];
  }

  // Print the received data
  switch (mode)
  {
  case RETURN_TO_HOME:
    Serial.println("RETURN TO HOME with GPS Coordinates: " + data);
    break;

  case ABORT_MISSION:
    Serial.println("ABORT MISSION");
    break;

  case UP:
    Serial.println("UP");
    break;

  case DOWN:
    Serial.println("DOWN");
    break;

  case LEFT:
    Serial.println("LEFT");
    break;

  case RIGHT:
    Serial.println("RIGHT");
    break;

  default:
    break;
  }
}

void setup()
{
  comms_init();
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    while (LoRa.available())
    {
      buffer = LoRa.readString();
    }
    parse_received_data();
  }

  if (millis() - prev_ticks > 1000)
  {
    switch (mode)
    {
    case AUV_BATTERY:
      send_command(AUV_BATTERY, "42");
      break;

    case AUV_INTERNAL_PRESSURE:
      send_command(AUV_INTERNAL_PRESSURE, "1003420");
      break;

    case AUV_INTERNAL_TEMPERATURE:
      send_command(AUV_INTERNAL_TEMPERATURE, "32");
      break;

    case AUV_GPS:
      send_command(AUV_GPS, "22.5386511, 88.2965371");
      break;

    default:
      break;
    }

    mode++;
    if (mode < AUV_BATTERY)
    {
      mode = AUV_BATTERY;
    }
    else if (mode > AUV_GPS)
    {
      mode = AUV_BATTERY;
    }

    prev_ticks = millis();
  }
}
