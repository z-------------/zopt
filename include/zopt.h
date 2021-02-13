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

/*
 * Parse command line options.
 * 
 * argc - The number of items in argv.
 * argv - The argument vector.
 * opt_defs - The option definitions. See ZOPT_DEF above.
 * opt_defs_count - The number of option definitions.
 * 
 * Returns an options container; see ZOPT_OPTS above.
 */
ZOPT_OPTS zopt_parse(int argc, const char *argv[], ZOPT_DEF opt_defs[], int opt_defs_count);

/*
 * Get the value and kind of a specified option.
 * 
 * opts - The options container.
 * name - The name of the option whose value to get.
 * value_out - The address of the buffer to which to write the option's value.
 * value_out_size - The size of the buffer.
 * 
 * Returns the kind of the specified option, or ZOPT_NOTPRESENT if it is not present.
 */
ZOPT_KIND zopt_get(ZOPT_OPTS opts, const char *name, char *value_out, int value_out_size);

/*
 * Get the value of a specified boolean option.
 * 
 * opts - The options container.
 * name - The name of the option whose value to get.
 * default_val - The value to return if the specified option is not present.
 * 
 * Returns the value of the specified option, or default_val if it is not present.
 */
char zopt_get_bool(ZOPT_OPTS opts, const char *name, char default_val);
