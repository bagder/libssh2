/* Copyright (C) The libssh2 project and its contributors. */

#include "runner.h"

int test(LIBSSH2_SESSION *session)
{
#if LIBSSH2_RSA_SHA1
    /* set in Dockerfile */
    return test_auth_pubkey(session, 0,
                            "libssh2",
                            NULL,
                            "key_rsa.pub",
                            "key_rsa");
#else
    (void)session;
    return 0;
#endif
}
