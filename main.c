#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt-plus/getopt-plus.h"

int main(int argc, char **argv)
{

    int long_arg = 0;
    // int help = 0;
    arg_t options[] = {
        /* These options set a flag. */
        {{"longarg", optional_argument, &long_arg, 1}, "Some long argument", 0, NULL, NULL},
        {{}, "ARG 1", 0, "a::", NULL},
        {{"bar", optional_argument, NULL, 1}, "ARG 2", 0, "b::", NULL},
        {{0, 0, 0, 0}, NULL, 0, 0, NULL},
    };

    const char *posargs[2] = {0};

    int ret = args_parse(argc, argv, options, (const char *[]){"INPUT", "OUTPUT", NULL}, posargs);
    if (ret < 0)
    {
        args_cleanup(options);
        return ret;
    }

    printf("longarg: %d\n", long_arg);
    args_dbg(options);
    printf("posargs:");
    for (size_t i = 0; i < 2; ++i)
    {
        if (posargs[i] == NULL)
        {
            break;
        }
        printf(" %s", posargs[i]);
    }
    printf("\n");

    args_cleanup(options);
    return 0;
}
