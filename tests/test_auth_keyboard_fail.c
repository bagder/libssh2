/* Copyright (C) The libssh2 project and its contributors. */

#include "runner.h"

int test(LIBSSH2_SESSION *session)
{
    return test_auth_keyboard(session, TEST_AUTH_SHOULDFAIL,
                              "libssh2", /* set in Dockerfile */
                              "I am the wrong password");
}
