#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <linux/dirent.h>
#include <linux/unistd.h>

int main ( int argc, char** argv ) {

  DIR* cur_dir = opendir ( argv[1] );

  if ( cur_dir != NULL ) {

    struct dirent* cur_dirent = ( struct dirent* ) calloc ( 1, sizeof ( struct dirent ) );
    cur_dirent = readdir ( DIR* cur_dir );

    while ( cur_dirent != NULL ) {

        printf ( "%s\n", cur_dirent->d_name );
        cur_dirent = readdir ( DIR* cur_dir );

    }  
    
  }
  
  else {

      printf ("ERROR: can't read directory!");
      return -1;

  }

}
