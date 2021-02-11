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

void write_to_server ( char* buff, int sk ) {

    int ret;

    printf ( "Now text your message:\n" );

    scanf ( "%s", buff );

    ret = write ( sk, buff, sizeof ( buff ) );
    if ( ret < 0 ) {

        perror ( "Unable to write" );
        exit ( 1 );

    }

}

void exit_from_server ( int sk ) {

    int ret;

    printf ( "You was disconnected! DO SVYAZI\n" );
    ret = write ( sk, EXIT_MESSAGE, sizeof ( EXIT_MESSAGE ) );
    if ( ret < 0 ) {

        perror ( "Unable to write" );
        exit ( 1 );

    }

    close ( sk );

}

int main() {

    struct sockaddr_in name = {0};
    char command[6], buff[50000];
    int port;
    char ipaddres[100];
    int sk, ret, com;
    ret = scanf ( "%d %s", &port, ipaddres );

    if ( ret < 0 ) {

	perror ( "Wrong input" );
	exit ( 1 );

    }

    sk = socket ( AF_INET, SOCK_STREAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
	exit ( 1 );

    }

    name.sin_family = AF_INET;
    name.sin_port = htons ( port );
    ret = inet_aton ( ipaddres, &(name.sin_addr) );

    if ( ret == 0 ) {

        perror ( "Wrong ip addres" );
        exit ( 1 );

    }

    //strncpy ( name.sun_path, PATH, sizeof ( name ) );

    ret = connect ( sk, (struct sockaddr* ) &name, sizeof ( name ) );
    if ( ret ) {

	perror ( "Unable to connect to socket" );
	exit ( 1 );

    }

    while (1) {

	printf ( "Please input command:\n" );
	com = waiting_for_command ( command );
        while ( !com ) {

	    printf ( "Unknown command(\n" );
            printf ( "Please input command:\n" );
	    com = waiting_for_command ( command );

        }

        switch ( com ) {

	    case 1: write_to_server ( buff, sk );
		    break;

	    case 2: exit_from_server ( sk );
		    exit ( 1 );
	            break;

	}

    }

}
