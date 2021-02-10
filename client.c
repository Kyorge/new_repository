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

    struct sockaddr_un name = {0};
    char command[6], buff[50000];
    int sk, ret, com;

    sk = socket ( AF_UNIX, SOCK_STREAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
	exit ( 1 );

    }

    name.sun_family = AF_UNIX;
    strncpy ( name.sun_path, PATH, sizeof ( name ) );

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
