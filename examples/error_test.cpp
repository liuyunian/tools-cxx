#include <errno.h>

#include <tools_cxx/error.h>

int main(){
    err_msg("Failed to ...");

    // err_quit("Failed to ...");

    errno = 2;
    err_ret("Failed to ...");

    // err_sys("Failed to ...");

    err_dump("Failed to ...");

    return 0;
}