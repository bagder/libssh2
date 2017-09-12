#include "session_fixture.h"

#include <libssh2.h>

#include <stdio.h>

const char *EXPECTED_RSA_HOSTKEY =
    "AAAAB3NzaC1yc2EAAAABIwAAAQEArrr/JuJmaZligyfS8vcNur+mWR2ddDQtVdhHzdKU"
    "UoR6/Om6cvxpe61H1YZO1xCpLUBXmkki4HoNtYOpPB2W4V+8U4BDeVBD5crypEOE1+7B"
    "Am99fnEDxYIOZq2/jTP0yQmzCpWYS3COyFmkOL7sfX1wQMeW5zQT2WKcxC6FSWbhDqrB"
    "eNEGi687hJJoJ7YXgY/IdiYW5NcOuqRSWljjGS3dAJsHHWk4nJbhjEDXbPaeduMAwQU9"
    "i6ELfP3r+q6wdu0P4jWaoo3De1aYxnToV/ldXykpipON4NPamsb6Ph2qlJQKypq7J4iQ"
    "gkIIbCU1A31+4ExvcIVoxLQw/aTSbw==";

const char *EXPECTED_ECDSA_HOSTKEY =
    "AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBG3+G5vVjF0tfzSY8"
    "FQ5cDrbFX5VXc09bRKPU7DfBhimxgEvLpjyxXRogiTSC+gy1SxFAo4aI1pUY5jlC6xG4Lk=";

int test(LIBSSH2_SESSION *session)
{
    int rc;
    size_t len;
    int type;
    unsigned int expected_len = 0;
    char *expected_hostkey = NULL;

    const char *hostkey = libssh2_session_hostkey(session, &len, &type);
    if (hostkey == NULL) {
        print_last_session_error("libssh2_session_hostkey");
        return 1;
    }

    if (type == LIBSSH2_HOSTKEY_TYPE_ECDSA) {
        rc = libssh2_base64_decode(session, &expected_hostkey, &expected_len,
                                   EXPECTED_ECDSA_HOSTKEY, strlen(EXPECTED_ECDSA_HOSTKEY));

        char *encoded = NULL;
        _libssh2_base64_encode(session, expected_hostkey, expected_len, &encoded);
        fprintf(stderr, "ECDSA key %s\n", encoded);
    }
    else if (type == LIBSSH2_HOSTKEY_TYPE_RSA) {
        rc = libssh2_base64_decode(session, &expected_hostkey, &expected_len,
                                   EXPECTED_RSA_HOSTKEY, strlen(EXPECTED_RSA_HOSTKEY));
    }
    else {
        fprintf(stderr, "Unexpected type of hostkey: %i\n", type);
        return 1;
    }

    if (rc != 0) {
        print_last_session_error("libssh2_base64_decode");
        return 1;
    }

    if (len != expected_len) {
        fprintf(stderr, "Hostkey does not have the expected length %ld != %d\n",
                len, expected_len);
        return 1;
    }

    if (memcmp(hostkey, expected_hostkey, len) != 0) {
        fprintf(stderr, "Hostkeys do not match\n");
        return 1;
    }

    return 0;
}
