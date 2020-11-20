#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>

int main ( int argc, char** argv ) {

    int in_init, cur_watch, cur_ret;
    int mask = IN_CREATE | IN_DELETE | IN_MOVE_SELF;
    char init_buf[4096];
    struct inotify_event* event;

    in_init = inotify_init();
    
    if ( in_init < 0 ) {

        printf ( "ERROR: in_init!\n" );
        return -1;

    }
    
    cur_watch = inotify_add_watch ( in_init, argv[1], mask );
    
    if ( cur_watch < 0 ) {

        printf ( "ERROR: inotify_add_watch!\n" );
        return -2;

    }
    
    while (1) {

        cur_ret = read ( in_init, init_buf, 4096 );
    
        if ( cur_ret > 0 ) {
    
            event = init_buf;
            printf ( "path:%s   ", event->name );
        
            if ( event->mask & IN_CREATE )
            
                printf ( "event: create.\n");
            
            if ( event->mask & IN_DELETE )
            
                printf ( "event: delete.\n");

            if ( event->mask & IN_MOVE_SELF )
            
                printf ( "event: move self.\n");    

        }
    
        else {
    
            printf ( "ERROR: read!\n" );
            return -3;

        }
    
    }
    
    close ( in_init );
    return 0;
    
}
