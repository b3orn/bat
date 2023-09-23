#ifndef BAT_VERSION_H
#define BAT_VERSION_H

#ifndef BAT_VERSION
#define BAT_VERSION "unknown"
#endif

#ifndef BAT_BUILD_VERSION
#define BAT_BUILD_VERSION 0
#endif

#ifndef BAT_BUILD_DATE
#define BAT_BUILD_DATE "unknown"
#endif

#if defined BAT_OS_MACOS
#define BAT_OS "macos"
#elif defined BAT_OS_LINUX
#define BAT_OS "linux"
#elif defined BAT_OS_WINDOWS
#define BAT_OS "windows"
#else
#define BAT_OS "unknown"
#endif

#if defined BAT_ARCH_AMD64
#define BAT_ARCH "amd64"
#elif defined BAT_ARCH_X86
#define BAT_ARCH "x86"
#elif defined BAT_ARCH_ARM64
#define BAT_ARCH "arm64"
#elif defined BAT_ARCH_ARM32
#define BAT_ARCH "arm32"
#else
#define BAT_ARCH "unknown"
#endif

#endif
