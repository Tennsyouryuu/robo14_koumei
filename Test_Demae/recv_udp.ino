#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


/*****************Netwok Settings*****************/

int tire = 0, cata = 1, updown = 2;
byte mac[] = { 0x80, 0xA2, 0xDA, 0x0F, 0x88, 0x32 };
IPAddress ip( 172, 16, 14, 200 );
unsigned int localPort = 4000;

char packetBuffer[ UDP_TX_PACKET_MAX_SIZE ];
EthernetUDP Udp;

/*****************Netword Settings End************/
/*****************Network Functions***************/

void UDP_setup() {
    Ethernet.begin( mac, ip );
    Udp.begin( localPort );
}

void get_command( int *command ) {
    if( Udp.parsePacket() ) {
        Udp.read( packetBuffer, UDP_TX_PACKET_MAX_SIZE );
        convert_command( packetBuffer, command );
    }
}

void convert_command( char *packetBuffer, int *command ) {
    int VALUE_ID = 0;
    int VALUE_LEFT_FRON = 2;
    int VALUE_LEFT_REAR = 4;
    int VALUE_RIGT_FRON = 6;
    int VALUE_RIGT_REAR = 8;
    int VALUE_BUTTON_A = 1;
    int VALUE_BUTTON_B = 2;
    int COM_ID = 0;
    int COM_LEFT_FRON = 1;
    int COM_LEFT_REAR = 2;
    int COM_RIGT_FRON = 3;
    int COM_RIGT_REAR = 4;
    int COM_BUTTON_A = 1;
    int COM_BUTTON_B = 2;

    switch( packetBuffer[ VALUE_ID ] ) {
        case '$':
            command[ COM_ID ] = tire;    break;
        case '%':
            command[ COM_ID ] = cata;    break;
        case '&':
            command[ COM_ID ] = updown;  break;
    }

    if ( command[ COM_ID ] == tire || command[ COM_ID ] == cata ) {

        char temp_left_fron[ 2 ];
        char temp_left_rear[ 2 ];
        char temp_rigt_fron[ 2 ];
        char temp_rigt_rear[ 2 ];
        temp_left_fron[ 0 ] = packetBuffer[ VALUE_LEFT_FRON ];
        temp_left_rear[ 0 ] = packetBuffer[ VALUE_LEFT_REAR ];
        temp_rigt_fron[ 0 ] = packetBuffer[ VALUE_RIGT_FRON ];
        temp_rigt_rear[ 0 ] = packetBuffer[ VALUE_RIGT_REAR ];
        temp_left_fron[ 1 ] = temp_left_rear[ 1 ] = temp_rigt_fron[ 1 ] = temp_rigt_rear[ 1 ] = '\0';

        command[ COM_LEFT_FRON ] = atoi( temp_left_fron );
        command[ COM_LEFT_REAR ] = atoi( temp_left_rear );
        command[ COM_RIGT_FRON ] = atoi( temp_rigt_fron );
        command[ COM_RIGT_REAR ] = atoi( temp_rigt_rear );

        if( packetBuffer[ VALUE_LEFT_FRON - 1 ] == 'B' ) {
            command[ COM_LEFT_FRON ] *= -1;
        }
        if( packetBuffer[ VALUE_LEFT_REAR - 1 ] == 'B' ) {
            command[ COM_LEFT_REAR ] *= -1;
        }
        if( packetBuffer[ VALUE_RIGT_FRON - 1 ] == 'B' ) {
            command[ COM_RIGT_FRON ] *= -1;
        }
        if( packetBuffer[ VALUE_RIGT_REAR - 1 ] == 'B' ) {
            command[ COM_RIGT_REAR ] *= -1;
        }
    }
    else if ( command[ COM_ID ] == updown ) {
        char temp_button_a[ 2 ];
        char temp_button_b[ 2 ];
        temp_button_a[ 0 ] = packetBuffer[ VALUE_BUTTON_A ];
        temp_button_b[ 0 ] = packetBuffer[ VALUE_BUTTON_B ];
        temp_button_a[ 1 ] = temp_button_b[ 1 ] = '\0';

        command[ COM_BUTTON_A ] = atoi( temp_button_a );
        command[ COM_BUTTON_B ] = atoi( temp_button_b );
    }
}

/******************Netword Functions End***********/


/*
void setup() {
    UDP_setup();
    Serial.begin(9600);
}

void loop() {
    int command[] = { tire, 0, 0 };
    get_command( command );

    Serial.print( command[ 0 ] );
    Serial.print( command[ 1 ] );
    Serial.print( command[ 2 ] );
    Serial.print( "\n" );
}
*/
