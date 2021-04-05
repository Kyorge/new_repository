#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#pragma warning (disable:4996)
int main()
{
    FILE* out;

    out = fopen("input40.txt", "w");
    int num_cache = -1, num_pages = -1, page = -1, i;
    srand(time(0));

    num_cache = rand() % 1000;
    num_pages = rand() % 100000;

    fprintf ( out, "%d ", num_cache );
    fprintf ( out, "%d ", num_pages );

    for (i = 0; i < num_pages; ++i) {
        page = rand() % num_pages/4;
        fprintf ( out, "%d ", page );
    }
    fclose(out);
    return 0;
}

