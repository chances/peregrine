#include <stdio.h>
#include "minunit.h"

int tests_run = 0;

#include "../src/script.h"

static char * test_runtimeInit() {
  Runtime* runtime;
  mu_assert(runtime = runtimeInit(), "error, Wren runtime not initialized");
  wrenFreeVM(runtime->vm);
  free(runtime);
  return 0;
}

static char* runTests() {
  mu_run_test(test_runtimeInit);
  return 0;
}

#pragma region Bootstrap tests
int main() {
  char *result = runTests();
  if (result != 0) printf("%s\n", result);
  else printf("ALL TESTS PASSED\n");
  printf("Tests passed: %d\n", tests_run);

  return result != 0;
}
#pragma endregion
