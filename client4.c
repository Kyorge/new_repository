#include "myserver.h"

#define EXIT_MESSAGE "YA VSO, DO SVYAZY"
#define PORT_NUM 23460

int waiting_for_command ( char* command ) {

    int s;

    s = scanf ( "%s", command );
    if ( ( s < 0 ) || ( s > 5 ) ) {

	perror ( "Too long command" );
	exit ( 1 );

    }

    if ( !strcmp ( command, "WRITE" ) )

	return 1;

    else if ( !strcmp ( command, "EXIT" ) )

	     return 2;

    	 else

	     return 0;

}

void praddr(char *msg, struct sockaddr *addr) {

    struct sockaddr_in* addr1 = ( struct sockaddr_in* ) addr;
    int port = ntohs ( addr1->sin_port );
    char* ip = inet_ntoa ( addr1->sin_addr );
    printf("%s: %s:%d\n", msg, ip, port);

}

void write_to_server ( char* buff, int sk, struct sockaddr* server, struct sockaddr* from, socklen_t from_len ) {

    int ret;

    printf ( "Now text your message:\n" );
    getchar ();
    //scanf ( "%s", buff );
    fgets ( buff, sizeof( buff ), stdin );

    ret = sendto ( sk, buff, sizeof ( buff ), 0, ( struct sockaddr* ) server, sizeof ( struct sockaddr_in ) );
    if ( ret == -1 ) {

        perror ( "Unable to write" );
        exit ( 1 );

    }

    ret = recvfrom ( sk, buff, sizeof ( buff ), 0, from, &from_len );
    if ( ( ret < 0 ) || ( ret >= BUFSZ ) ) {

        printf ( "Unexpected read error or overflow %d\n", ret );
        exit ( 1 );

    }

    praddr ( buff, from );

}

void exit_from_server ( int sk ) {

    int ret;

    printf ( "You was disconnected! DO SVYAZI\n" );
    ret = write ( sk, EXIT_MESSAGE, sizeof ( EXIT_MESSAGE ) );
    if ( ret < 0 ) {

        printf ( "Server is already closed!\n" );

    }

    close ( sk );

}

int main() {

    struct sockaddr_in server = {0}, server2 = {0};
    struct sockaddr_in from;
    socklen_t from_len = sizeof ( from );
    char command[6], buff[50000];
    //int port;
    //char ipaddres[100];
    int sk, ret, com, yes = 1;
    /*ret = scanf ( "%d %s", &port, ipaddres );

    if ( ret < 0 ) {

	perror ( "Wrong input" );
	exit ( 1 );

    }*/

    sk = socket ( AF_INET, SOCK_DGRAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
	exit ( 1 );

    }

    server.sin_family = AF_INET;
    server.sin_port = htons ( PORT_NUM );
    server2.sin_family = AF_INET;
    server2.sin_port = htons ( PORT_NUM - 15 );
    server.sin_addr.s_addr = htonl ( INADDR_ANY );
    server2.sin_addr.s_addr = htonl ( INADDR_ANY );
    setsockopt ( sk, SOL_SOCKET, SO_BROADCAST, &yes, sizeof ( yes ) );
    ret = bind ( sk, ( struct sockaddr* ) &server, sizeof ( server ) );
    if ( ret < 0 ) {

        perror ( "Unable to bind socket with address" );
        close ( sk );
        return 1;

    }

    /*ret = inet_aton ( ipaddres, &( server.sin_addr ) );

    if ( ret == 0 ) {

        perror ( "Wrong ip addres" );
        exit ( 1 );

    }*/

    /*
    ret = connect ( sk, (struct sockaddr* ) &name, sizeof ( name ) );
    if ( ret ) {

	perror ( "Unable to connect to socket" );
	exit ( 1 );

    } */

    while (1) {

	printf ( "Please input command:\n" );
	com = waiting_for_command ( command );
        while ( !com ) {

	    printf ( "Unknown command(\n" );
            printf ( "Please input command:\n" );
	    com = waiting_for_command ( command );

        }

        switch ( com ) {

	    case 1: write_to_server ( buff, sk, ( struct sockaddr* ) &server2, ( struct sockaddr* ) &from, from_len );
		    break;

	    case 2: exit_from_server ( sk );
		    exit ( 1 );
	            break;

	}

    }

}
