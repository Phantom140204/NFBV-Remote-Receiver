#pragma once

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

int comms_init(void);
int receive_packet(String *buffer);
void send_command(const command_ids command, String data);
