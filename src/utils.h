/**
 * @return number of args read in.
 */
int readin_args(WORD_LIST **l, int argc, const char *argv[])
{
    int i = 0;
    for (; i != argc && (*l) != NULL; ++i) {
        argv[i] = (*l)->word->word;
        (*l) = (*l)->next;
    }
    return i;
}

/**
 * @return number of optional arg read in on success, -1 if not enough/too many arguments.
 */
int to_argv_opt(WORD_LIST *l, int argc, int opt_argc, const char *argv[])
{
    if (readin_args(&l, argc, argv) < argc) {
        builtin_usage();
        return -1;
    }

    int i = readin_args(&l, opt_argc, argv + argc);
    if (l != NULL) {
        builtin_usage();
        return -1;
    }
    return i;
}
/**
 * @return 0 on success, -1 if not enough/too many arguments.
 */
int to_argv(WORD_LIST *l, int argc, const char *argv[])
{
    return to_argv_opt(l, argc, 0, argv);
}

/**
 * @return -1 if failed, 0 if succeeds.
 *
 * This function call will also reset_internal_getopt and set list = loptend
 * for you.
 */
int check_no_options(WORD_LIST **list)
{
    reset_internal_getopt();
    if (no_options(*list)) // If options present
        return -1;
    *list = loptend;
    return 0;
}
