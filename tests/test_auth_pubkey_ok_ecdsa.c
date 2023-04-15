#include "runner.h"

int test(LIBSSH2_SESSION *session)
{
#if LIBSSH2_ECDSA
    /* configured in Dockerfile */
    return test_auth_pubkey(session, 0,
                            "libssh2",
                            NULL,
                            "key_ecdsa.pub",
                            "key_ecdsa");
#else
    (void)session;
    return 0;
#endif
}
