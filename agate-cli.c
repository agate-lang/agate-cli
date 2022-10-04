// SPDX-License-Identifier: MIT
// Copyright (c) 2022 Julien Bernard
#include <stdio.h>
#include <stdlib.h>

#include "agate.h"
#include "support.h"

#include "config.h"

static void print(AgateVM *vm, const char* text) {
  fputs(text, stdout);
}

static void write(AgateVM *vm, uint8_t byte) {
  fputc(byte, stdout);
}

static void error(AgateVM *vm, AgateErrorKind kind, const char *unit_name, int line, const char *message) {
  switch (kind) {
    case AGATE_ERROR_COMPILE:
      printf("[%s:%d]: error: %s\n", unit_name, line, message);
      break;
    case AGATE_ERROR_RUNTIME:
      printf("Error: %s\n", message);
      break;
    case AGATE_ERROR_STACKTRACE:
      printf("from [%s:%d]: %s\n", unit_name, line, message);
      break;
  }
}

static void input(AgateVM *vm, char *buffer, size_t size) {
  fgets(buffer, size, stdin);
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: agate <script> [...]\n");
    return EXIT_FAILURE;
  }

  AgateConfig config;
  agateConfigInitialize(&config);

  config.unit_handler = agateExUnitHandler;

  config.print = print;
  config.write = write;
  config.error = error;
  config.input = input;

  AgateVM *vm = agateExNewVM(&config);

  agateExUnitAddIncludePath(vm, AGATE_UNIT_DIRECTORY);
  agateExUnitAddIncludePath(vm, ".");

  const char *source = agateExUnitLoad(vm, argv[1]);
  int return_code = EXIT_SUCCESS;

  if (source != NULL) {
    agateSetArgs(vm, argc - 1, argv + 1);
    AgateStatus status = agateCallString(vm, argv[1], source);
    agateExUnitRelease(vm, source);

    if (status != AGATE_STATUS_OK) {
      return_code = EXIT_FAILURE;
    }
  } else {
    fprintf(stderr, "Unknown unit: '%s'\n", argv[1]);
    return_code = EXIT_FAILURE;
  }

  agateDeleteVM(vm);
  return return_code;
}
