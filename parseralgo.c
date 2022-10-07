#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> 
#include "parseralgostruct.h"

typedef enum 
{
	Passed,
    Error
} Msg_Check;

static const char *HEADER = "A3"; 	// means £
static const char *EOFF = "25"; 	//means %

static void wait_msg(char text);
static void wait_msg_complete(char *text);
static void parse_msg(Msg_Data *data);
static Msg_Check check_format(Msg_Data *data);

static char received_data[512];
static uint16_t received_index = 0;

//example data getting from USART1
//char text[] = "A3 CCDD  FE  00000012 01 0F  53 45 4C 4D 25";
unsigned char text[] = "A3CCDDFE00000012010F53454C4D25";


int main()
{
    //printf("main\n");
    
	int i;
    for (i = 0; i < strlen(text) + 1; i++) 
	{
        wait_msg(text[i]);
    }
    
    

    return 0;
}


//to simulate getting data from USART1 byte per byte
static void wait_msg(char text)
{
	//printf("waitmsg\n");
	
    received_data[received_index++] = text;

    if (received_index == 31) {
        wait_msg_complete(received_data);

        received_index = 0;      
    }
    
    
}


//to simulate completion of data stream from USART1 byte per byte
static void wait_msg_complete(char *text)
{
    
    //printf("waitmsgcomplete\n");
    
    Msg_Data data;
    
    /*
	memcpy(data.commandno, text[1], sizeof(data.commandno));
	memcpy(data.periphno, text[3], sizeof(data.periphno));
	memcpy(data.commandaddress, text[4], sizeof(data.commandaddress));
	*/
	
	
	//entry of message, end of message, data part of message is defined with unsigned char, strcpy is used parse char types
	
    strncpy(data.header, text, sizeof(data.header));
    strcpy(data.endof, &text[28]); 
    strncpy(data.data, &text[20], sizeof(data.data));


	//variables like command no, commandtype, commandaddress is defined with unsigned integer types
	//strcpy and strtol are used to parse message and convert hex strings to integer values

	
	char* temp = malloc(8 * sizeof(char));
    strncpy(temp, &text[2], 4);
    data.commandno = strtol(temp, NULL, 16);
    printf("\ndata.commandno  : %u \n\n", data.commandno );
    
	memset(temp, 0, sizeof temp);
    strncpy(temp, &text[6], 2);
    data.periphno = strtol(temp, NULL, 16);
    printf("data.periphno : %u\n\n", data.periphno );
    
    
	memset(temp, 0, sizeof temp);
    strncpy(temp, &text[8], 8);
    data.commandaddress = strtol(temp, NULL, 16);
    printf("data.commandaddress : %u\n\n", data.commandaddress );
    
    
	memset(temp, 0, sizeof temp);
    strncpy(temp, &text[16], 2);
    data.commandtype = strtol(temp, NULL, 16);
    printf("data.commandtype : %u \n\n", data.commandtype );
    
    
	memset(temp, 0, sizeof temp);
    strncpy(temp, &text[18], 2);
    data.datatype = strtol(temp, NULL, 16);
    printf("data.datatype  : %u  \n\n", data.datatype);
	
	
	
	parse_msg(&data);
}


//to parse message packet and to control if message is desired or not
static void parse_msg(Msg_Data *data)
{
    
    //printf("parsemsg\n");
    
    
    Msg_Check type = check_format(data);

    switch (type) {
    case Passed:
        printf("Meaningful message\r\n");
        break;
    case Error:
    	printf("Error\r\n");
    	break;
    default:
        printf("Error\r\n");
        break;
    }
}

static Msg_Check check_format(Msg_Data *data)
{
    //printf("checkformat\n");
    
    if ((strcmp(data->header, HEADER) == 0) && (strcmp(data->endof, EOFF) == 0)) 
	{
        return Error;
    }
    else
    {
    	return Passed;
	}
}
