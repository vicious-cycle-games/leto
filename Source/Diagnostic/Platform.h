/**
 * @file Platform.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Defines some various platform-related macros and strings.
 * @date 2024-10-05
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the @file LICENSE.md file.
 */

#ifndef LETO__PLATFORM_H
#define LETO__PLATFORM_H

// Figure out what operating system we're compiling for. If it's not
// supported, we throw a compiler error. Note that if on an Apple
// system, this brings in a (trivial) header file to check WHICH Apple
// system we're on.
#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN32__)
    #define LETO_WINDOWS
    #define LETO_PLATFORM "windows"
#elif defined(linux) || defined(__linux__)
    #define LETO_LINUX
    #define LETO_PLATFORM "linux"
    // Define the display server of the application, as some window
    // creation hints that are required on X11 are not on Wayland.
    #if DISPLAY_SERVER == 0
        #define LETO_WAYLAND
        #define LETO_DISPLAY_SERVER "wayland"
    #elif DISPLAY_SERVER == 1
        #define LETO_X11
        #define LETO_DISPLAY_SERVER "x11"
    #else
        #error "Unsupported display server."
    #endif
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_MAX)
        #define LETO_APPLE
        #define LETO_PLATFORM "apple"
    #else
        #error "Unsupported Apple system."
    #endif
#else
    #error "Unsupported operating system."
#endif

// Make sure we're running on a CPU architecture that we support.
// I'd really rather not run on x32 processors because of registry
// pressure, but if the user wants it, we'll let it happen.
#if defined(__x86_64__) || defined(_M_X64)
    #define LETO_ARCH "x86_64"
    #define LETO_X86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) ||            \
    defined(_M_IX86)
    #define LETO_ARCH "x86_32"
    #define LETO_X86_32
    #warning "Leto will probably run strange on 32 bit systems."
#elif defined(__aarch64__) || defined(_M_ARM64) ||                        \
    defined(__AARCH64EL__) || defined(__arm64)
    #define LETO_ARCH "arm_x64"
    #define LETO_ARM_X64
#else
    #error "Unsupported CPU architecture."
#endif

// Define any debug state-specific macros.
#if !defined(LETO_DEBUG)
    #define LETO_BUILD_STATE "release"
    #define NDEBUG
#else
    #define LETO_BUILD_STATE "debug"
#endif

/**
 * @brief A string representing the full platform string of the currently
 * running Leto binaries. Note that this does @b not contain Leto's display
 * server even if on Linux, that has to be queried via the @ref
 * LETO_DISPLAY_SERVER macro.
 */
#define LETO_PLATFORM_STRING                                              \
    LETO_PLATFORM "-" LETO_ARCH "-" LETO_BUILD_STATE

#endif // LETO__PLATFORM_H
