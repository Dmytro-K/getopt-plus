#pragma once

#ifndef GETOPT_PLUS_H
#define GETOPT_PLUS_H

#include <stdint.h>

#include <getopt.h>

typedef struct arg_list_t
{
    const char *head;
    struct arg_list_t *next;
} arg_list_t;

typedef struct
{
    struct option opt;
    const char *help;
    uint32_t count;
    const char *c;
    arg_list_t *arg;
} arg_t;

int args_parse(int argc, char **argv, arg_t args[], const char *posargs_names[],
               const char *posargs[]);

void args_dbg(const arg_t args[]);

void args_cleanup(arg_t args[]);

#endif  // GETOPT_PLUS_H
