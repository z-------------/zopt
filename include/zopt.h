typedef enum ZOPT_KIND {
    ZOPT_NOTPRESENT = 0,
    ZOPT_BOOL   = 1<<0,
    ZOPT_STR    = 1<<1
} ZOPT_KIND;

typedef struct ZOPT_DEF {
    char *name;
    ZOPT_KIND kind;
} ZOPT_DEF;

typedef struct ZOPT_VAL {
    char *name;
    ZOPT_KIND kind;
    char *value;
} ZOPT_VAL;

typedef struct ZOPT_OPTS {
    ZOPT_VAL *opts;
    int count;
    char **args;
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
ZOPT_OPTS zopt_parse(int argc, char *argv[], ZOPT_DEF opt_defs[], int opt_defs_count);

/*
 * Get a specified option.
 * 
 * opts - The options container.
 * name - The name of the option whose value to get.
 * 
 * Returns a pointer to the specified option.
 */
ZOPT_VAL *zopt_get(ZOPT_OPTS opts, char *name);

/*
 * Get the value a specified string option.
 * 
 * opts - The options container.
 * name - The name of the option whose value to get.
 * 
 * Returns a pointer to the string value of the specified option.
 */
char *zopt_get_str(ZOPT_OPTS opts, char *name);

/*
 * Get the value of a specified boolean option.
 * 
 * opts - The options container.
 * name - The name of the option whose value to get.
 * default_val - The value to return if the specified option is not present.
 * 
 * Returns the boolean value of the specified option, or default_val if it is not present.
 */
char zopt_get_bool(ZOPT_OPTS opts, char *name, char default_val);
