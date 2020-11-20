#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main ( int argc, char** argv ) {

  if ( argc == 1 )

    printf ( "ERROR: too few arguments!\n" );

  else {    

    DIR* cur_dir = opendir ( argv[1] );

    if ( cur_dir != NULL ) {

        struct dirent* cur_dirent;
        cur_dirent = readdir ( cur_dirent );

        while ( cur_dirent != NULL ) {

            printf ( "%s\n", cur_dirent->d_name );
            cur_dirent = readdir ( cur_dirent );

        }  
    
     }
  
    else {

        printf ("ERROR: can't read directory!");
          return -1;

    }

  }

}
