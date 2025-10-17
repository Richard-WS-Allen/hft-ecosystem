#pragma once

#include <cstring>
#include <iostream>

#define ASSERT(cond, msg) \
  do { \
    if (!(cond)) [[unlikely]] { \
      std::cerr << "ASSERT FAILED: " << msg << std::endl; \
      exit(EXIT_FAILURE); \
    } \
  } while (0)
