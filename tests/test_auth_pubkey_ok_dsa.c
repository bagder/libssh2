#include "runner.h"

int test(LIBSSH2_SESSION *session)
{
#if !defined(LIBSSH2_MBEDTLS)
    /* configured in Dockerfile */
    return test_auth_pubkey(session, 0,
                            "libssh2",
                            NULL,
                            "key_dsa.pub",
                            "key_dsa");
#else
    (void)session;
    return 0;
#endif
}
