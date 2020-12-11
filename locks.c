#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
 
void write_locks ( struct flock* lock, int num ) {
 
	printf ( "Lock number: %d\n", num);
	printf( "\tl_type: %d\n\tl_whence: %d\n\tl_len: %ld\n\tl_pid: %d\n", lock->l_type, lock->l_whence, lock->l_len, lock->l_pid);
 
}
 
int main ( int argc, char** argv ) {
 
	int fd, ret, num = 1;
	struct flock f1 = {0};
 
	if ( argc < 2 ) {
 
		printf ( "Not enough arguments!" );
		exit(1);
 
	}
 
	fd = open ( argv[1], O_RDWR );
 
	if ( fd < 0 ) {
 
		perror ( "Unable to open file!");
		exit(1);
 
	}
 
	while ( fcntl ( fd, F_GETLK, &f1 ) >= 0 ) {
 
		write_locks ( &f1, num );
		num++;
 
	}
 
	return 0;
 
}
