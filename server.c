#include "myserver.h"

int main() {

    int sk, ret;
    char command;
    struct sockaddr_un name = {0};

    sk = socket ( AF_UNIX, SOCK_STREAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
        return 1;

    }

    name.sun_family = AF_UNIX;
    strncpy ( name.sun_path, PATH, sizeof ( PATH ) );

    ret = bind ( sk, ( struct sockaddr* )&name, sizeof ( name ) );
    if ( ret < 0 ) {

        perror ( "Unable to bind socket with address" );
	close ( sk );
	return 1;

    }

    ret = listen ( sk, 20 );
    if ( ret ) {

	perror ( "Unable to listen" );
	close ( sk );
	return 1;

    }

    while ( 1 ) {

	int client_sk;
	char buffer[BUFSZ] = {0};
	client_sk = accept ( sk, NULL, NULL );
	if ( client_sk < 0 ) {

	    perror ( "Unable to accept" );
	    exit ( 1 );

	}

	while ( 1 ) {

	    ret = read ( client_sk, buffer, BUFSZ );
	    if ( ( ret < 0 ) || ( ret >= BUFSZ ) ) {

	        printf ( "Unexpected read error or overflow %d\n", ret );
	        exit ( 1 );

	    }

	    if ( !strcmp ( buffer, "YA VSO, DO SVYAZY" ) )

		break;

	    printf ( "%s\n", buffer );
	    printf ( "Press e + ENTER if you want to listen next client or press f to pay respect)\n" );

	    command = getchar ();

	    if ( command == 'f' ) {

	        unlink ( PATH );
    	        close ( sk );
	        printf ( "Server was closed( Try to call later! DO SVYAZI\n" );
	        return 0;

	    }

	    command = getchar ();

	}

	close ( client_sk );

    }

}
