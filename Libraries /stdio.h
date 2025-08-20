/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Explanation:
 *  - SPDX is a standard license identifier system.
 *  - This line specifies that the source code is owned by Espressif Systems (the creators of ESP32).
 *  - License: Apache-2.0 (a permissive open-source license, allowing usage, modification, and redistribution).
 */

#pragma once
// Ensures this header file is only included once during compilation.
// Prevents redefinition errors. Equivalent to:
//   #ifndef _FILENAME_
//   #define _FILENAME_
//   ...
//   #endif

#include "sdkconfig.h"
// Includes ESP-IDF’s sdkconfig header, which contains build-time configuration
// options selected using `menuconfig` (like enabling/disabling libraries).
// This helps in conditional compilation (features enabled/disabled depending on settings).

#include_next <stdio.h>
// This is special: instead of including <stdio.h> from the *same* folder,
// it includes the *next* stdio.h in the include search path.
// This allows ESP-IDF to "wrap" or extend the standard C library `stdio.h`
// without fully replacing it, so you still get the normal functions (printf, fopen, etc.).

#if CONFIG_LIBC_PICOLIBC
// This block is only compiled if ESP-IDF is configured to use PicoLibC.
// PicoLibC = a lightweight C standard library designed for embedded systems.
// It saves memory compared to Newlib (the default C library).

// Declare flockfile and funlockfile, which are normally part of stdio.h
// but may be missing in PicoLibC. They are used for thread-safe locking of FILE streams.
void flockfile(FILE *);    // Lock a FILE stream (for multithreaded safety)
void funlockfile(FILE *);  // Unlock a FILE stream

// open_memstream is a GNU extension.
// It creates a FILE* that writes to a dynamically allocated memory buffer instead of a file.
// Example: you can use fprintf() to write into memory instead of an actual file.
FILE *open_memstream(char **, size_t *);
#endif
// End of conditional block for PicoLibC
