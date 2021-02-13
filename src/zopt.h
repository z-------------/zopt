typedef enum ZOPT_KIND {
    ZOPT_NOTPRESENT = 0, // returned by zopt_get if the specified option is not present
    ZOPT_BOOL   = 1<<0,
    ZOPT_STR    = 1<<1
} ZOPT_KIND;

typedef struct ZOPT_DEF {
    const char *name;
    ZOPT_KIND kind;
} ZOPT_DEF;

typedef struct ZOPT_VAL {
    const char *name;
    ZOPT_KIND kind;
    const char *value;
} ZOPT_VAL;

typedef struct ZOPT_OPTS {
    ZOPT_VAL *opts;
    int count;
    const char **args;
    int args_count;
} ZOPT_OPTS;

ZOPT_OPTS zopt_parse(int argc, const char *argv[], ZOPT_DEF opt_defs[], int opt_defs_count);

ZOPT_KIND zopt_get(ZOPT_OPTS opts, const char *name, char *value_out, int value_out_size);

char zopt_get_bool(ZOPT_OPTS opts, const char *name, char default_val);
