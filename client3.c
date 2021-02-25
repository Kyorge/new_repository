#include "myserver.h"

#define EXIT_MESSAGE "YA VSO, DO SVYAZY"

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

void write_to_server ( char* buff, int sk, struct sockaddr* server ) {

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

    struct sockaddr_in server = {0};
    char command[6], buff[50000];
    int port;
    char ipaddres[100];
    int sk, ret, com;
    ret = scanf ( "%d %s", &port, ipaddres );

    if ( ret < 0 ) {

	perror ( "Wrong input" );
	exit ( 1 );

    }

    sk = socket ( AF_INET, SOCK_DGRAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
	exit ( 1 );

    }

    server.sin_family = AF_INET;
    server.sin_port = htons ( port );
    ret = inet_aton ( ipaddres, &( server.sin_addr ) );

    if ( ret == 0 ) {

        perror ( "Wrong ip addres" );
        exit ( 1 );

    }

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

	    case 1: write_to_server ( buff, sk, ( struct sockaddr* ) &server );
		    break;

	    case 2: exit_from_server ( sk );
		    exit ( 1 );
	            break;

	}

    }

}
