#pragma once

#ifndef GETOPT_PLUS_H
#define GETOPT_PLUS_H

#include <stddef.h>
#include <stdint.h>

#include <getopt.h>

/** Singly linked list of argument strings collected for an option. */
typedef struct arg_list_t
{
    /** Argument string stored by this node. */
    const char *head;

    /** Next node, or NULL at the end of the list. */
    struct arg_list_t *next;
} arg_list_t;

/** Definition and parse state for one command-line option. */
typedef struct
{
    /** GNU getopt_long() definition for the long option. */
    struct option opt;

    /** Human-readable description displayed in usage output. */
    const char *help;

    /** Number of times the option occurred on the command line. */
    uint32_t count;

    /** Short-option specification, or NULL when no short form exists. */
    const char *c;

    /** Arguments collected from occurrences of this option. */
    arg_list_t *arg;
} arg_t;

/**
 * Parse long options, short options, and positional arguments.
 *
 * Both @p args and @p posargs_names use a zeroed or NULL sentinel entry. Parsed
 * option counts and option arguments are stored back into @p args.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument vector.
 * @param args Mutable, sentinel-terminated option definitions.
 * @param posargs_names Sentinel-terminated positional argument names, or NULL.
 * @param posargs Destination array for positional argument strings, or NULL.
 * @param posargs_min Minimum number of required positional arguments.
 * @return Number of positional arguments on success, or a negative value when
 *         help was requested or parsing failed.
 */
int args_parse(int argc, char **argv, arg_t args[], const char *posargs_names[],
               const char *posargs[], int posargs_min);

/**
 * Print command usage and all configured options to standard output.
 *
 * @param prog Program name displayed in the usage prefix.
 * @param args Sentinel-terminated option definitions.
 * @param posargs_names Sentinel-terminated positional argument names, or NULL.
 */
void args_print_usage(const char *prog, const arg_t args[], const char *posargs_names[]);

/**
 * Print parsed option counts and arguments to standard output.
 *
 * @param args Sentinel-terminated parsed option definitions.
 */
void args_dbg(const arg_t args[]);

/**
 * Release option-argument list nodes allocated by args_parse().
 *
 * @param args Sentinel-terminated parsed option definitions.
 */
void args_cleanup(arg_t args[]);

#endif  // GETOPT_PLUS_H
