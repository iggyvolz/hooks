PHP_ARG_ENABLE([hooks],
  [whether to enable hooks support],
  [AS_HELP_STRING([--enable-hooks],
    [Enable hooks support])],
  [yes])

if test "$PHP_HOOKS" != "no"; then
  AC_DEFINE(HAVE_HOOKS, 1, [ Have hooks support ])
  PHP_NEW_EXTENSION(hooks, hooks.c, $ext_shared)
fi
