#include "runner.h"

static const char *USERNAME = "I'm the wrong username";
static const char *PASSWORD = "my test password"; /* set in Dockerfile */

int test(LIBSSH2_SESSION *session)
{
    int rc;

    const char *userauth_list =
        libssh2_userauth_list(session, USERNAME,
                              (unsigned int)strlen(USERNAME));
    if(!userauth_list) {
        print_last_session_error("libssh2_userauth_list");
        return 1;
    }

    if(!strstr(userauth_list, "password")) {
        fprintf(stderr, "'password' was expected in userauth list: %s\n",
                userauth_list);
        return 1;
    }

    rc = libssh2_userauth_password_ex(session, USERNAME,
                                      (unsigned int)strlen(USERNAME),
                                      PASSWORD,
                                      (unsigned int)strlen(PASSWORD),
                                      NULL);
    if(rc == 0) {
        fprintf(stderr, "Password auth succeeded with wrong username\n");
        return 1;
    }

    return 0;
}
