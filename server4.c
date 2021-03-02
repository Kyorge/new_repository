#include "myserver.h"

#define PORT_NUM 23445
#define ANSW_MSG "Hello!"

void praddr(char *msg, struct sockaddr *addr) {

    struct sockaddr_in* addr1 = ( struct sockaddr_in* ) addr;
    int port = ntohs ( addr1->sin_port );
    char* ip = inet_ntoa ( addr1->sin_addr );
    printf("%s: %s:%d\n", msg, ip, port);

}

/*int create_useless_server ( int new_port ) {

    int sk;

    sk = socket ( AF_INET, SOCK_DGRAM, 0 );
    if ( sk < 0 ) {

        perror ( "Unable to create socket" );
        return 1;

    }

    name.sin_family = AF_INET;
    name.sin_port = htons ( PORT_NUM + new_port );
    name.sin_addr.s_addr = htonl ( INADDR_LOOPBACK );

}*/

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

    int sk, ret, client_num = 0, yes = 1;
    char command;
    struct sockaddr_in name = {0};
    struct sockaddr from = {0};
    socklen_t from_len = sizeof ( from );
    sk = socket ( AF_INET, SOCK_DGRAM, 0 );
    if ( sk < 0 ) {

	perror ( "Unable to create socket" );
        return 1;

    }

    name.sin_family = AF_INET;
    name.sin_port = htons ( PORT_NUM );
    name.sin_addr.s_addr = htonl ( INADDR_ANY );
//    setsockopt  ( sk, SOL_SOCKET, SO_BROADCAST, &yes, sizeof ( yes ) );
    ret = bind ( sk, ( struct sockaddr* ) &name, sizeof ( name ) );
    if ( ret < 0 ) {

        perror ( "Unable to bind socket with address" );
	close ( sk );
	return 1;

    }

    char buffer[BUFSZ] = {0};

    while ( 1 ) {

	//create_useless_server ( client_num++ );
	printf ( "Press e + ENTER if you want to listen next client or press f to pay respect)\n" );

        command = getchar ();

        if ( command == 'f' ) {

            close ( sk );
            printf ( "Server was closed( Try to call later! DO SVYAZI\n" );
            return 0;

        }

        command = getchar ();



	ret = recvfrom ( sk, buffer, sizeof ( buffer ), 0, &from, &from_len );
	if ( ( ret < 0 ) || ( ret >= BUFSZ ) ) {

	    printf ( "Unexpected read error or overflow %d\n", ret );
	    exit ( 1 );

	}

	if ( !strcmp ( buffer, "YA VSO, DO SVYAZY" ) )

	    break;

    	if ( ret == -1 ) {

            perror ( "Unable to write" );
            exit ( 1 );

        }

	buffer [ strlen ( buffer ) - 1 ] = '\0';

        praddr ( buffer, &from);

	ret = look_for_commands ( buffer );

	if ( ret == 0 )

	    printf ( "%s\n", buffer );

        ret = sendto ( sk, ANSW_MSG, 30, 0, &from, from_len );
        if ( ret == -1 ) {

            perror ( "Unable to write" );
            exit ( 1 );

        }

    }

}
