#include "myserver.h"

int look_for_commands ( char* buffer ) {

    if ( ( buffer[0] == 'c' ) && ( buffer[1] == 'd' ) ) {

	int ret;

	if ( buffer[2] == ' ' )

	    ret = chdir ( buffer + 3 );

	else

	    if ( buffer[2] == '\0' )

		ret = chdir ( "/" );

	    else

		ret = -1;

	if ( ret == -1 ) {

	    printf ( "Unexpected ERROR\n" );
	    return 0;

	}

	else

	    return 1;

    }

    if ( strcmp ( buffer, "ls" ) == 0 ) {

	pid_t pid = fork();
	if ( pid == 0 ) {

	    execlp ( buffer, buffer, NULL );
	    fflush ( stdout );
	    exit(1);

	}

	else {

	    wait(0);
	    return 1;

	}

    }

    return 0;

}

int main() {

    int sk, ret;
    char command;
    struct sockaddr_in name = {0};

    sk = socket ( AF_INET, SOCK_DGRAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
        return 1;

    }

    name.sin_family = AF_INET;
    name.sin_port = htons ( 23458 );
    name.sin_addr.s_addr = htonl ( INADDR_LOOPBACK );

    ret = bind ( sk, ( struct sockaddr* ) &name, sizeof ( name ) );
    if ( ret < 0 ) {

        perror ( "Unable to bind socket with address" );
	close ( sk );
	return 1;

    }

    /*ret = listen ( sk, 20 );
    if ( ret ) {

	perror ( "Unable to listen" );
	close ( sk );
	return 1;

    }*/

    //while ( 1 ) {

	//int client_sk;
	char buffer[BUFSZ] = {0};
	/*client_sk = accept ( sk, NULL, NULL );
	if ( client_sk < 0 ) {

	    perror ( "Unable to accept" );
	    exit ( 1 );

	}*/

	while ( 1 ) {

	    ret = recvfrom ( sk, buffer, BUFSZ, 0, NULL, NULL );
	    if ( ( ret < 0 ) || ( ret >= BUFSZ ) ) {

	        printf ( "Unexpected read error or overflow %d\n", ret );
	        exit ( 1 );

	    }

	    if ( !strcmp ( buffer, "YA VSO, DO SVYAZY" ) )

		break;

	    buffer [ strlen ( buffer ) - 1 ] = '\0';

	    ret = look_for_commands ( buffer );

	    if ( ret == 0 )

		printf ( "%s\n", buffer );

	    printf ( "Press e + ENTER if you want to listen next client or press f to pay respect)\n" );

	    command = getchar ();

	    if ( command == 'f' ) {

	        //unlink ( PATH );
    	        close ( sk );
	        printf ( "Server was closed( Try to call later! DO SVYAZI\n" );
	        return 0;

	    }

	    command = getchar ();

	}

	//close ( sk );

    //}

}
