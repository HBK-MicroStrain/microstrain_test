#pragma once

#include <string>

#include <doctest/doctest.h>

// -----------------------------------------------------------------------------------------------------------
// Automatic test registration and discovery
// -----------------------------------------------------------------------------------------------------------

#define MICROSTRAIN_TEST_CASE_IMPL(suite_name, test_name) \
    TEST_CASE("[" suite_name "] " test_name)

// -----------------------------------------------------------------------------------------------------------
// Assertions
// -----------------------------------------------------------------------------------------------------------

// TODO: Can add wrapper macros for the doctest parts of this and make it framework-independent
//     ---> In this case, we could rename the file common.hpp or implementation.hpp
/// @brief Internal, DON'T USE DIRECTLY!
#define CSTR_EQ_IMPL(actual, expected, level)                                                       \
    do                                                                                                        \
    {                                                                                                         \
        INFO("Actual:   " << std::string(actual));                                                            \
        INFO("Expected: " << std::string(expected));                                                          \
                                                                                                              \
        bool terminated = actual[strlen(expected)] == '\0';                                                   \
        if (!terminated)                                                                                      \
        {                                                                                                     \
            level##_MESSAGE(terminated, "The actual string is not terminated when the expected string is.");  \
            break;                                                                                            \
        }                                                                                                     \
                                                                                                              \
        level##_EQ(strcmp(actual, expected), 0);                                                              \
    } while(0)

/// @brief Displays a warning, but does not fail the test when the given C strings are not equal.
///
/// This assertion is pseudo-safe if the expected string is passed as the second argument.
/// It will fail early if the actual string isn't null terminated at the same position as
/// the expected string.
///
/// It is reasonable to assume that the expected string is null terminated, since it is
/// almost always hard-coded by a test.
#define WARN_CSTR_EQ(actual, expected) CSTR_EQ_IMPL(actual, expected, WARN)

/// @brief Fails the test when the given C strings are not equal, but allows the test to keep running.
///
/// This assertion is pseudo-safe if the expected string is passed as the second argument.
/// It will fail early if the actual string isn't null terminated at the same position as
/// the expected string.
///
/// It is reasonable to assume that the expected string is null terminated, since it is
/// almost always hard-coded by a test.
#define CHECK_CSTR_EQ(actual, expected) CSTR_EQ_IMPL(actual, expected, CHECK)

/// @brief Fails and exits the test immediately when the given C strings are not equal.
///
/// This assertion is pseudo-safe if the expected string is passed as the second argument.
/// It will fail early if the actual string isn't null terminated at the same position as
/// the expected string.
///
/// It is reasonable to assume that the expected string is null terminated, since it is
/// almost always hard-coded by a test.
#define REQUIRE_CSTR_EQ(actual, expected) CSTR_EQ_IMPL(actual, expected, REQUIRE)


inline std::string microstrain_doctest_format_hex(const uint8_t* buffer, size_t size)
{
    static constexpr char hex_chars[] = "0123456789ABCDEF";

    std::string result;
    result.resize(size * 2);

    for (size_t i=0; i<size; i++)
    {
        const uint8_t b = buffer[i];
        result[2 * i]     = hex_chars[b >> 4];
        result[2 * i + 1] = hex_chars[b & 0x0F];
    }

    return result;
}

/// @brief Internal, DON'T USE DIRECTLY!
#define BUFFER_EQ_IMPL(actual, expected, size, level)                          \
    do                                                                         \
    {                                                                          \
        INFO("Actual:   " << microstrain_doctest_format_hex(actual, size));    \
        INFO("Expected: " << microstrain_doctest_format_hex(expected, size));  \
                                                                               \
        level##_EQ(memcmp(actual, expected, size), 0);                         \
    } while(0)

/// @brief Displays a warning, but does not fail the test when the given u8 buffers are not equal.
#define WARN_BUFFER_EQ(actual, expected, size)    BUFFER_EQ_IMPL(actual, expected, size, WARN)

/// @brief Fails the test when the given u8 buffers are not equal, but allows the test to keep running.
#define CHECK_BUFFER_EQ(actual, expected, size)   BUFFER_EQ_IMPL(actual, expected, size, CHECK)

/// @brief Fails and exits the test immediately when the given u8 buffers are not equal.
#define REQUIRE_BUFFER_EQ(actual, expected, size) BUFFER_EQ_IMPL(actual, expected, size, REQUIRE)
