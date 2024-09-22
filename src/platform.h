/**
 * @file platform.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide various platforming tools for Leto. This merges
 * some platform macros, defines new ones, and identifies display servers.
 * @date 2024-09-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This file is under the GNU Affero General Public License v3.0. That
 * means it is fully legal to distribute this source code and/or any
 * modified copies of this source code in both private and commercial
 * contexts. However, you must note copyright and state any changes you've
 * made. Source disclosure is also necessary. For more information, see the
 * @file LICENSE.md file that should have been distributed alongside this
 * source code.
 */

#ifndef LETO__PLATFORM
#define LETO__PLATFORM

// Utility macros like STRINGIFY.
#include <utilities/macros.h>

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

/**
 * @brief The major version of the game (X.x.x). This is only
 * changed for huge new releases, and should @b never go backward.
 */
#define LETO_VERSION_MAJOR 0

/**
 * @brief The minor version of the game (x.X.x). This value is
 * changed for minor releases, like UI changes or major overhauls
 * to a section of the code. This rolls over to zero when a new
 * major release is deployed.
 */
#define LETO_VERSION_MINOR 0

/**
 * @brief The tweak version of the game (x.x.X). This value is
 * changed for every commit to the code base. This rolls back to
 * zero when a new major or minor release is deployed.
 */
#define LETO_VERSION_TWEAK 8

/**
 * @brief A denotion of the current build state. This will be one
 * of four things. For pre-release builds, there are two;
 * "pre-alpha" and "alpha". Pre-alpha builds are extremely
 * unpolished, barely functional builds, and alpha builds are
 * little better. For release builds, there are also two: "beta"
 * and "stable". Beta releases are nearly feature-complete but need
 * a lot of polishing, and stable builds are fully ready for
 * production. This value will @b never go down, only further
 * towards stable.
 */
#define LETO_VERSION_IDENTIFIER "pre-alpha"

/**
 * @brief A verison string containing all the major, minor, tweak,
 * and cycle identifier of the current build.
 */
#define LETO_VERSION                                                      \
    LETO_STRINGIFY(LETO_VERSION_MAJOR)                                    \
    "." LETO_STRINGIFY(LETO_VERSION_MINOR) "." LETO_STRINGIFY(            \
        LETO_VERSION_TWEAK) "_" LETO_VERSION_IDENTIFIER

/**
 * @brief A @b full version string that contains the operating
 * system and CPU architecture, along with all digits in @ref
 * LETO_VERSION.
 */
#define LETO_VERSION_FULL LETO_PLATFORM "-" LETO_ARCH "-" LETO_VERSION

#endif // LETO__PLATFORM
