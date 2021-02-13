#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/zopt.h"

typedef char BOOL;

#define TRUE    (BOOL)1
#define FALSE   (BOOL)0

#define DUMP_STR(name)  printf("%s = \"%s\"\n", #name, name)
#define DUMP_INT(name)  printf("%s = %d\n", #name, name)

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

void _zopt_parse_handle_opt(ZOPT_OPTS *result, const char *value, ZOPT_DEF opt_defs[], int opt_defs_count, const char *cur_opt_name, BOOL *is_waiting_for_val, ZOPT_DEF **cur_def) {
    *cur_def = find_def(opt_defs, opt_defs_count, cur_opt_name);
    if (!*cur_def) {
        fprintf(stderr, "Unknown option '%s'\n", cur_opt_name);
        return;
    }
    *is_waiting_for_val = FALSE;
    switch ((*cur_def)->kind) {
        case ZOPT_BOOL:
            result->opts = add_opt(result->opts, result->count++, *cur_def, "");
            break;
        case ZOPT_STR:
            if (value) {
                result->opts = add_opt(result->opts, result->count++, *cur_def, value);
            } else {
                *is_waiting_for_val = TRUE;
            }
            break;
    }
}

ZOPT_OPTS zopt_parse(int argc, const char *argv[], ZOPT_DEF opt_defs[], int opt_defs_count) {
    ZOPT_OPTS result;
    result.opts = NULL;
    result.count = 0;
    result.args = NULL;
    result.args_count = 0;

    BOOL is_waiting_for_val = FALSE;
    ZOPT_DEF *cur_def;
    for (int i = 1; i < argc; ++i) {
        if (is_waiting_for_val) {
            result.opts = add_opt(result.opts, result.count++, cur_def, argv[i]);
            is_waiting_for_val = FALSE;
            continue;
        }
        
        int len;
        if (argv[i][0] != '-' || (len = strlen(argv[i])) < 2) {
            ++result.args_count;
            result.args = (const char **)realloc(result.args, result.args_count * sizeof(const char *));
            result.args[result.args_count - 1] = argv[i];
            continue;
        }

        const char *cur_opt_name;
        BOOL is_long_opt = argv[i][1] == '-'; // whole thing starts with '--'
        if (is_long_opt) {
            cur_opt_name = &argv[i][2];
            char *value = NULL;
            char *eq_sign;
            if (eq_sign = strchr(argv[i], '=')) {
                value = eq_sign + 1;

                int opt_name_size = eq_sign - cur_opt_name + 1;
                char *opt_name = (char *)malloc(opt_name_size * sizeof(char));
                strncpy(opt_name, cur_opt_name, opt_name_size);
                opt_name[opt_name_size-1] = 0;
                cur_opt_name = opt_name;
            }
            _zopt_parse_handle_opt(&result, value, opt_defs, opt_defs_count, cur_opt_name, &is_waiting_for_val, &cur_def);
        } else {
            for (int j = 1; j < len; ++j) {
                char opt_name[2];
                opt_name[0] = argv[i][j];
                opt_name[1] = 0;
                cur_opt_name = opt_name;
                _zopt_parse_handle_opt(&result, NULL, opt_defs, opt_defs_count, cur_opt_name, &is_waiting_for_val, &cur_def);
            }
        }
    }
    
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
    strncpy(value_out, "", value_out_size);
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
        { .name = "u", .kind = ZOPT_STR },
        { .name = "foo", .kind = ZOPT_STR }
    };
    ZOPT_OPTS opts = zopt_parse(argc, argv, opt_defs, sizeof(opt_defs) / sizeof(ZOPT_DEF));
    printf("parsed %d opts and %d args\n", opts.count, opts.args_count);

    printf("-m = %d\n", zopt_get_bool(opts, "m", FALSE));
    printf("-p = %d\n", zopt_get_bool(opts, "p", FALSE));
    char u_val[128];
    if (zopt_get(opts, "u", u_val, sizeof(u_val)))
        printf("-u = \"%s\"\n", u_val);
    char foo_val[128];
    if (zopt_get(opts, "foo", foo_val, sizeof(foo_val)))
        printf("--foo = \"%s\"\n", foo_val);

    for (int i = 0; i < opts.args_count; ++i) {
        printf("arg: \"%s\"\n", opts.args[i]);
    }
}
