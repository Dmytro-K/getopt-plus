#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt-plus.h"

static arg_list_t *arg_list_put(arg_list_t *lst, const char *arg)
{
    arg_list_t *tmp = lst;
    if (NULL != tmp)
    {
        for (; NULL != tmp->next; tmp = tmp->next)
            ;
        tmp->next = malloc(sizeof(arg_list_t));
        tmp = tmp->next;
    }
    else
    {
        tmp = lst = malloc(sizeof(arg_list_t));
    }

    tmp->head = arg;
    tmp->next = NULL;

    return lst;
}

static void arg_list_free(arg_list_t *lst)
{
    for (arg_list_t *tmp = lst; NULL != lst; lst = tmp)
    {
        tmp = lst->next;
        free(lst);
    }
}

void args_cleanup(arg_t args[])
{
    for (size_t i = 0; ((NULL != args[i].c) || (NULL != args[i].opt.name)); ++i)
    {
        arg_list_free(args[i].arg);
    }
}

int args_parse(int argc, char **argv, arg_t args[], const char *posargs_names[],
               const char *posargs[])
{
    size_t short_count = 0;
    size_t short_size = 0;
    size_t long_count = 0;
    size_t count;
    bool err = false;
    int help = 0;
    for (count = 0;; ++count)
    {
        if ((NULL == args[count].c) && (NULL == args[count].opt.name))
        {
            break;
        }
        if (NULL != args[count].c)
        {
            ++short_count;
            short_size += strlen(args[count].c);
        }
        if (NULL != args[count].opt.name)
        {
            long_count++;
        }
    }

    struct option *long_options = malloc(sizeof(struct option) * (long_count + 2));
    for (size_t i = 0, j = 0; i < long_count; ++i, ++j)
    {
        while (NULL == args[j].opt.name)
        {
            ++j;
        }
        long_options[i] = args[j].opt;
        long_options[i].val = (1 << 24) + (int)j;
    }

    long_options[long_count].name = "help";
    long_options[long_count].has_arg = no_argument;
    long_options[long_count].flag = &help;
    long_options[long_count].val = 1;

    memset(&long_options[long_count + 1], 0, sizeof(long_options[long_count + 1]));

    char *short_options = malloc(short_size + 2);
    short_options[0] = '\0';
    for (size_t i = 0, j = 0; i < short_count; ++i, ++j)
    {
        while (NULL == args[j].c)
        {
            ++j;
        }
        strcat(short_options, args[j].c);
    }

    short_options[short_size] = 'h';
    short_options[short_size + 1] = '\0';

    while (1)
    {
        int option_index = 0;

        int c = getopt_long(argc, argv, short_options, long_options, &option_index);

        if (-1 == c)
        {
            break;
        }
        else if ('h' == c)
        {
            help = 1;
            break;
        }
        else if ('?' == c)
        {
            err = true;
            break;
        }
        else
        {
            if (c < (1 << 24))
            {
                for (size_t i = 0; i < count; ++i)
                {
                    if ((NULL != args[i].c) && (c == args[i].c[0]))
                    {
                        ++args[i].count;
                        if (optarg != NULL)
                        {
                            args[i].arg = arg_list_put(args[i].arg, optarg);
                        }
                    }
                }
            }
            else
            {
                size_t i = (size_t)c - (1 << 24);
                ++args[i].count;
                if (NULL != optarg)
                {
                    args[i].arg = arg_list_put(args[i].arg, optarg);
                }
            }
        }

        for (size_t i = 0; i < count; ++i)
        {
            if ((NULL != args[i].opt.name) && (NULL != args[i].opt.flag))
            {
                if (args[i].opt.flag[0] == ((int)i + (1 << 24)))
                {
                    args[i].opt.flag[0] = args[i].opt.val;
                    ++args[i].count;
                    if (NULL != optarg)
                    {
                        args[i].arg = arg_list_put(args[i].arg, optarg);
                    }
                }
            }
        }
    }

    free(long_options);
    free(short_options);

    int posargs_count = 0;
    if ((NULL != posargs_names) && (NULL != posargs))
    {
        if (argc > optind)
        {
            int poscount = argc - optind;
            for (int i = 0; (i < poscount) && (NULL != posargs_names[i]); ++i)
            {
                posargs[i] = argv[optind + i];
                ++posargs_count;
            }
        }
    }

    if ((0 != help) || (false != err))
    {
        printf("Usage: %s [OPTIONS]", argv[0]);
        if ((NULL != posargs_names) && (NULL != posargs))
        {
            for (size_t i = 0; NULL != posargs_names[i]; ++i)
            {
                printf(" %s", posargs_names[i]);
            }
        }
        printf("\n");
        printf("Options:\n");
        for (size_t i = 0; i < count; ++i)
        {
            printf("  ");

            if (args[i].c != NULL)
            {
                printf("-%c", args[i].c[0]);
            }
            else
            {
                printf("  ");
            }

            if (NULL != args[i].opt.name)
            {
                printf("%c --%-10s", ((NULL == args[i].c) ? ' ' : ','), args[i].opt.name);
            }
            else
            {
                printf("    %-10s", "");
            }
            if (NULL != args[i].help)
            {
                printf("\t%s", args[i].help);
            }
            printf("\n");
        }
        printf("  -h, --%-10s\tPrint this help\n", "help");
        return -1;
    }

    if (false == err)
    {
        return posargs_count;
    }
    else
    {
        return -2;
    }
}

void args_dbg(const arg_t args[])
{
    for (size_t i = 0; ((NULL != args[i].c) || (NULL != args[i].opt.name)); ++i)
    {
        printf("%s - ", (NULL != args[i].c ? args[i].c : ""));
        printf("%s: ", (NULL != args[i].opt.name ? args[i].opt.name : ""));
        printf("count: %u ", args[i].count);
        printf("args: ");
        for (arg_list_t *lst = args[i].arg; NULL != lst; lst = lst->next)
        {
            printf("%p(%s) ", lst->head, lst->head);
        }
        printf("\n");
    }
}
