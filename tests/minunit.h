#pragma once

// See the MinUnit blog post: https://jera.com/techinfo/jtns/jtn002

#define mu_assert(test, message) \
  do                             \
  {                              \
    if (!(test))                 \
      return message;            \
  } while (0)

#define mu_run_test(test)   \
  do                        \
  {                         \
    char *message = test(); \
    if (message)            \
      return message;       \
    tests_run++;            \
  } while (0)

/// Total number of passing tests
extern int tests_run;
