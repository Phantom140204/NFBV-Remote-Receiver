#include <Arduino.h>

#include "comms.h"

String buffer;

void parse_received_lora_packet();
void parse_received_serial_data();

void setup()
{
  Serial.begin(115200);

  comms_init();

  Serial.println("Initialised");
}

void loop()
{
  if (receive_packet(&buffer) == 0)
  {
    parse_received_lora_packet();
  }

  parse_received_serial_data();
}

void parse_received_lora_packet()
{
  // String manipulation for getting mode and data
  int mode = (int)buffer[0] - (int)'0';
  String data = "";
  for (int i = 1; i < buffer.length(); i++)
  {
    data += buffer[i];
  }

  Serial.println(buffer);

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

void parse_received_serial_data()
{
  String serial_data, data;
  int mode = 0;

  if (Serial.available())
  {
    serial_data = Serial.readString();

    // String manipulation for getting mode and data
    mode = (int)serial_data[0] - (int)'0';
    data = "";
    for (int i = 1; i < serial_data.length(); i++)
    {
      data += serial_data[i];
    }

    Serial.print("Sent packet - ");
    Serial.println(serial_data);
  }

  switch (mode)
  {
  case AUV_BATTERY:
    send_command(AUV_BATTERY, data);
    break;

  case AUV_INTERNAL_PRESSURE:
    send_command(AUV_INTERNAL_PRESSURE, data);
    break;

  case AUV_INTERNAL_TEMPERATURE:
    send_command(AUV_INTERNAL_TEMPERATURE, data);
    break;

  case AUV_GPS:
    send_command(AUV_GPS, data);
    break;

  default:
    break;
  }
}
