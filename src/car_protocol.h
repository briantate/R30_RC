#ifndef CAR_PROTOCOL_H_
#define CAR_PROTOCOL_H_

#include <stdint.h>

//button values vor various uses
#define BUTTON_RELEASED (0)
#define BUTTON_PRESSED (1)

//drive direction 
#define DIRECTION_NEGATIVE (0) //left or down
#define DIRECTION_POSITIVE (1) //right or up

//command types
#define CMD_DRIVE (0x01)
#define CMD_KEEPALIVE (0x02)

#define SIZE_DRIVE_PAYLOAD (5)
#define SIZE_KEEPALIVE_PAYLOAD (5)


typedef struct{
    uint8_t type;
    uint8_t payload_length;
}__attribute__((packed)) car_protocol_header_t;

typedef struct{
    union{
        uint8_t reg;
        struct{
            uint8_t rsvd: 2;
            uint8_t r_button: 1;
            uint8_t l_button: 1;
            uint8_t r_y_direction: 1;
            uint8_t r_x_direction: 1;
            uint8_t l_x_direction: 1;
            uint8_t l_y_direction: 1;
        } bits;
    };
}polarity_buttons_t;

typedef struct{
    car_protocol_header_t header;
    uint8_t left_x_speed;
    uint8_t left_y_speed;
    uint8_t right_x_speed;
    uint8_t right_y_speed;
    polarity_buttons_t polarity_buttons;
}__attribute__((packed)) car_drive_packet_t;

#endif /* CAR_PROTOCOL_H_ */