#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/zopt.h"

#define CHECK_EQ(actual, expected) \
    if (actual != expected) { fprintf(stderr, "Check failed: %s is %d, expected %d.\n", #actual, actual, expected); exit(1); }
#define CHECK_STR_EQ(actual, expected) \
    if (strcmp(actual, expected)) { fprintf(stderr, "Check failed: %s is \"%s\", expected \"%s\".\n", #actual, actual, expected); exit(1); }

int main() {
    const char *argv[] = { "_", "foo", "-mu", "alice", "-p", "bar", "--foo=bar", "baz", "-abc", "qux", "--bar", "BAR!", "quux", "quuux" };
    int argc = sizeof(argv) / sizeof(const char *);

    ZOPT_DEF opt_defs[] = {
        { .name = "a", .kind = ZOPT_BOOL },
        { .name = "b", .kind = ZOPT_BOOL },
        { .name = "c", .kind = ZOPT_BOOL },
        { .name = "m", .kind = ZOPT_BOOL },
        { .name = "p", .kind = ZOPT_BOOL },
        { .name = "q", .kind = ZOPT_BOOL },
        { .name = "u", .kind = ZOPT_STR },
        { .name = "foo", .kind = ZOPT_STR },
        { .name = "bar", .kind = ZOPT_STR }
    };
    ZOPT_OPTS opts = zopt_parse(argc, argv, opt_defs, sizeof(opt_defs) / sizeof(ZOPT_DEF));

    CHECK_EQ(opts.count, 8);
    CHECK_EQ(opts.args_count, 6);

    CHECK_EQ(zopt_get_bool(opts, "m", 0), 1);
    CHECK_EQ(zopt_get_bool(opts, "p", 0), 1);

    CHECK_EQ(zopt_get_bool(opts, "a", 0), 1);
    CHECK_EQ(zopt_get_bool(opts, "b", 0), 1);
    CHECK_EQ(zopt_get_bool(opts, "c", 0), 1);

    CHECK_EQ(zopt_get_bool(opts, "q", 0), 0);
    CHECK_EQ(zopt_get_bool(opts, "q", 1), 1);

    CHECK_EQ(zopt_get_bool(opts, "x", 0), 0);
    CHECK_EQ(zopt_get_bool(opts, "x", 1), 1);

    CHECK_STR_EQ(zopt_get_str(opts, "u"), "alice");
    CHECK_STR_EQ(zopt_get_str(opts, "foo"), "bar");
    CHECK_STR_EQ(zopt_get_str(opts, "bar"), "BAR!");
    CHECK_EQ(zopt_get_str(opts, "NONEXISTENT"), NULL);

    CHECK_STR_EQ(opts.args[0], "foo");
    CHECK_STR_EQ(opts.args[1], "bar");
    CHECK_STR_EQ(opts.args[2], "baz");
    CHECK_STR_EQ(opts.args[3], "qux");
    CHECK_STR_EQ(opts.args[4], "quux");
    CHECK_STR_EQ(opts.args[5], "quuux");
}
