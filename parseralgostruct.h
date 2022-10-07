#ifndef PARSEALGOSTRUCT_H
#define PARSEALGOSTRUCT_H


typedef struct Msg_Data
{
unsigned char header[2], endof[2];
uint16_t commandno;
uint8_t periphno;
uint32_t commandaddress;
uint8_t commandtype;
uint8_t datatype;
unsigned char data[8];
}Msg_Data;

#endif
