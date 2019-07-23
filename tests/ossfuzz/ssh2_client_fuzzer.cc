#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <libssh2.h>
#include "testinput.h"

#define FUZZ_ASSERT(COND)                                                     \
        if(!(COND))                                                           \
        {                                                                     \
          fprintf(stderr, "Assertion failed: " #COND "\n%s",                  \
                  strerror(errno));                                           \
          assert((COND));                                                     \
        }

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  int socket_fds[2] = {-1, -1};
  ssize_t written;
  int rc;
  LIBSSH2_SESSION *session = NULL;
  int handshake_completed = 0;

  rc = libssh2_init(0);

  if(rc != 0) {
    fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
    goto EXIT_LABEL;
  }

  // Create a socket pair so data can be sent in.
  rc = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_fds);
  FUZZ_ASSERT(rc == 0);

  written = send(socket_fds[1], data, size, 0);
  FUZZ_ASSERT(written == size);

  rc = shutdown(socket_fds[1], SHUT_WR);
  FUZZ_ASSERT(rc == 0);

  // Create a session and start the handshake using the fuzz data passed in.
  session = libssh2_session_init();

  if(libssh2_session_handshake(session, socket_fds[0])) {
    goto EXIT_LABEL;
  }

  // If we get here the handshake actually completed.
  handshake_completed = 1;

EXIT_LABEL:

  if (session != NULL)
  {
    if (handshake_completed)
    {
      libssh2_session_disconnect(session,
                                 "Normal Shutdown, Thank you for playing");
    }

    libssh2_session_free(session);
  }

  libssh2_exit();

  close(socket_fds[0]);
  close(socket_fds[1]);

  return 0;
}