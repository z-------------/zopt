#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zopt.h"

#define TRUE    1
#define FALSE   0

#define DUMP_STR(name)  printf("%s = \"%s\"\n", #name, name)
#define DUMP_INT(name)  printf("%s = %d\n", #name, name)

typedef char BOOL;

ZOPT_VAL_PAIR *add_opt(ZOPT_VAL_PAIR opts[], int opts_count, ZOPT_DEF_PAIR *def, const char *value) {
    opts = (ZOPT_VAL_PAIR *)realloc(opts, (opts_count+1) * sizeof(ZOPT_VAL_PAIR));
    opts[opts_count].name = def->name;
    opts[opts_count].kind = def->kind;
    opts[opts_count].value = value;
    return opts;
}

ZOPT_DEF_PAIR *find_def(ZOPT_DEF_PAIR opt_defs[], int opt_defs_count, const char *name) {
    // printf("find_def\n");
    // DUMP_STR(name);
    for (int i = 0; i < opt_defs_count; ++i) {
        // DUMP_STR(opt_defs[i].name);
        if (!strcmp(opt_defs[i].name, name)) {
            return &opt_defs[i];
        }
    }
    return 0;
}

int zopt_parse(int argc, const char *argv[], ZOPT_DEF_PAIR opt_defs[], int opt_defs_count, ZOPT_VAL_PAIR **opts_out) {
    int count = 0;

    ZOPT_VAL_PAIR *opts = 0;
    BOOL is_waiting_for_val = 0;
    ZOPT_DEF_PAIR *cur_def;
    for (int i = 1; i < argc; ++i) {
        // printf("loop top\n");
        if (is_waiting_for_val) {
            // printf("got val we were waiting for.\n");
            opts = add_opt(opts, count++, cur_def, argv[i]);
        } else {
            if (argv[i][0] != '-') continue;
            const char *cur_opt_name = &argv[i][1];
            // DUMP_STR(cur_opt_name);
            cur_def = find_def(opt_defs, opt_defs_count, cur_opt_name);
            if (!cur_def) {
                fprintf(stderr, "Unknown option '%s'\n", cur_opt_name);
                continue;
            }
            // DUMP_STR(cur_def->name);
            // DUMP_INT(cur_def->kind);
            switch (cur_def->kind) {
                case ZOPT_BOOL:
                    opts = add_opt(opts, count++, cur_def, ""); break;
                case ZOPT_STR:
                    is_waiting_for_val = TRUE;
            }
        }
        // printf("loop bottom\n");
    }
    // printf("done\n");
    
    *opts_out = opts;
    return count;
}

ZOPT_KIND zopt_get(ZOPT_VAL_PAIR opts[], int opts_count, const char *name, char *value_out, int value_out_size) {
    for (int i = 0; i < opts_count; ++i) {
        if (!strcmp(opts[i].name, name)) {
            strncpy(value_out, opts[i].value, value_out_size);
            return opts[i].kind;
        }
    }
    return ZOPT_NOTPRESENT;
}

char zopt_get_bool(ZOPT_VAL_PAIR opts[], int opts_count, const char *name) {
    // DUMP_STR(name);
    for (int i = 0; i < opts_count; ++i) {
        // DUMP_STR(opts[i].name);
        if (!strcmp(opts[i].name, name)) {
            return TRUE;
        }
    }
    return FALSE;
}

/* main */

int main(int argc, const char *argv[]) {
    ZOPT_DEF_PAIR opt_defs[] = {
        { .name = "m", .kind = ZOPT_BOOL },
        { .name = "p", .kind = ZOPT_BOOL },
        { .name = "u", .kind = ZOPT_STR }
    };
    ZOPT_VAL_PAIR *opts = 0;
    int opts_count = zopt_parse(argc, argv, opt_defs, sizeof(opt_defs) / sizeof(ZOPT_DEF_PAIR), &opts);
    printf("parsed %d opts.\n", opts_count);

    printf("-m = %d\n", zopt_get_bool(opts, opts_count, "m"));
    printf("-p = %d\n", zopt_get_bool(opts, opts_count, "p"));

    char u_val[128];
    zopt_get(opts, opts_count, "u", u_val, sizeof(u_val));
    printf("-u = \"%s\"\n", u_val);
}
