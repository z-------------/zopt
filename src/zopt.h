typedef enum ZOPT_KIND {
    ZOPT_NOTPRESENT = 0, // returned by zopt_get if the specified option is not present
    ZOPT_BOOL   = 1<<0,
    ZOPT_STR    = 1<<1
} ZOPT_KIND;

typedef struct ZOPT_DEF_PAIR {
    const char *name;
    ZOPT_KIND kind;
} ZOPT_DEF_PAIR;

typedef struct ZOPT_VAL_PAIR {
    const char *name;
    ZOPT_KIND kind;
    const char *value;
} ZOPT_VAL_PAIR;

int zopt_parse(int argc, const char *argv[], ZOPT_DEF_PAIR opt_defs[], int opt_defs_count, ZOPT_VAL_PAIR *opts_out[]);

ZOPT_KIND zopt_get(ZOPT_VAL_PAIR opts[], int opts_count, const char *name, char *value_out, int value_out_size);

char zopt_get_bool(ZOPT_VAL_PAIR opts[], int opts_count, const char *name);
