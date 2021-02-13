#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zopt.h"

#define TRUE    1
#define FALSE   0

#define DUMP_STR(name)  printf("%s = \"%s\"\n", #name, name)
#define DUMP_INT(name)  printf("%s = %d\n", #name, name)

typedef char BOOL;

ZOPT_VAL *add_opt(ZOPT_VAL opts[], int opts_count, ZOPT_DEF *def, const char *value) {
    opts = (ZOPT_VAL *)realloc(opts, (opts_count+1) * sizeof(ZOPT_VAL));
    opts[opts_count].name = def->name;
    opts[opts_count].kind = def->kind;
    opts[opts_count].value = value;
    return opts;
}

ZOPT_DEF *find_def(ZOPT_DEF opt_defs[], int opt_defs_count, const char *name) {
    for (int i = 0; i < opt_defs_count; ++i) {
        if (!strcmp(opt_defs[i].name, name)) {
            return &opt_defs[i];
        }
    }
    return 0;
}

ZOPT_OPTS zopt_parse(int argc, const char *argv[], ZOPT_DEF opt_defs[], int opt_defs_count) {
    ZOPT_OPTS result;
    result.count = 0;

    ZOPT_VAL *opts = 0;
    BOOL is_waiting_for_val = 0;
    ZOPT_DEF *cur_def;
    for (int i = 1; i < argc; ++i) {
        if (is_waiting_for_val) {
            opts = add_opt(opts, result.count++, cur_def, argv[i]);
        } else {
            if (argv[i][0] != '-') continue;
            const char *cur_opt_name = &argv[i][1];
            cur_def = find_def(opt_defs, opt_defs_count, cur_opt_name);
            if (!cur_def) {
                fprintf(stderr, "Unknown option '%s'\n", cur_opt_name);
                continue;
            }
            switch (cur_def->kind) {
                case ZOPT_BOOL:
                    opts = add_opt(opts, result.count++, cur_def, ""); break;
                case ZOPT_STR:
                    is_waiting_for_val = TRUE;
            }
        }
    }
    
    result.opts = opts;
    return result;
}

ZOPT_KIND zopt_get(ZOPT_OPTS opts, const char *name, char *value_out, int value_out_size) {
    for (int i = 0; i < opts.count; ++i) {
        ZOPT_VAL *opt = &opts.opts[i];
        if (!strcmp(opt->name, name)) {
            strncpy(value_out, opt->value, value_out_size);
            return opt->kind;
        }
    }
    return ZOPT_NOTPRESENT;
}

char zopt_get_bool(ZOPT_OPTS opts, const char *name, char default_val) {
    for (int i = 0; i < opts.count; ++i) {
        if (!strcmp(opts.opts[i].name, name)) {
            return TRUE;
        }
    }
    return default_val;
}

/* main */

int main(int argc, const char *argv[]) {
    ZOPT_DEF opt_defs[] = {
        { .name = "m", .kind = ZOPT_BOOL },
        { .name = "p", .kind = ZOPT_BOOL },
        { .name = "u", .kind = ZOPT_STR }
    };
    ZOPT_OPTS opts = zopt_parse(argc, argv, opt_defs, sizeof(opt_defs) / sizeof(ZOPT_DEF));
    printf("parsed %d opts.\n", opts.count);

    printf("-m = %d\n", zopt_get_bool(opts, "m", FALSE));
    printf("-p = %d\n", zopt_get_bool(opts, "p", FALSE));

    char u_val[128];
    zopt_get(opts, "u", u_val, sizeof(u_val));
    printf("-u = \"%s\"\n", u_val);
}
