/* sock.c
** strophe XMPP client library -- socket abstraction implementation
**
** Copyright (C) 2005-2009 Collecta, Inc.
**
**  This software is provided AS-IS with no warranty, either express
**  or implied.
**
** This program is dual licensed under the MIT and GPLv3 licenses.
*/

/** @file
 *  Socket abstraction.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>

#include "sock.h"
#include "snprintf.h"

#include "socks5.h"

int sock_error(void)
{
    return errno;
}

static int _in_progress(int error)
{
    return (errno == EINPROGRESS);
}

sock_t sock_connect(const char * const host, const unsigned short port)
{
    sock_t sock;
    char service[6];
    struct addrinfo *res, *ainfo, hints;
    int err;

	if (socks5_check())
		return socks5_connect(host, port);

    xmpp_snprintf(service, 6, "%u", port);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif /* AI_ADDRCONFIG */
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;

    err = getaddrinfo(host, service, &hints, &res);
    if (err != 0)
        return -1;

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        sock = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);
        if (sock < 0)
            continue;

        err = sock_set_nonblocking(sock);
        if (err == 0) {
            err = connect(sock, ainfo->ai_addr, ainfo->ai_addrlen);
            if (err == 0 || _in_progress(sock_error()))
                break;
        }
        sock_close(sock);
    }
    freeaddrinfo(res);
    sock = ainfo == NULL ? -1 : sock;

    return sock;
}

int sock_set_keepalive(const sock_t sock, int timeout, int interval)
{
    int ret;
    int optval = (timeout && interval) ? 1 : 0;

    /* This function doesn't change maximum number of keepalive probes */

    ret = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
    if (ret < 0)
        return ret;

    if (optval) {
#ifdef TCP_KEEPIDLE
        ret = setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &timeout, sizeof(timeout));
#elif defined(TCP_KEEPALIVE)
        /* QNX receives `struct timeval' as argument, but it seems OSX does int */
        ret = setsockopt(sock, IPPROTO_TCP, TCP_KEEPALIVE, &timeout, sizeof(timeout));
#endif /* TCP_KEEPIDLE */
        if (ret < 0)
            return ret;
#ifdef TCP_KEEPINTVL
        ret = setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
        if (ret < 0)
            return ret;
#endif /* TCP_KEEPINTVL */
    }

    return ret;
}

int sock_close(const sock_t sock)
{
    return close(sock);
}

int sock_set_blocking(const sock_t sock)
{
    int rc;

    rc = fcntl(sock, F_GETFL, NULL);
    if (rc >= 0) {
        rc = fcntl(sock, F_SETFL, rc & (~O_NONBLOCK));
    }
    return rc;
}

int sock_set_nonblocking(const sock_t sock)
{
    int rc;

    rc = fcntl(sock, F_GETFL, NULL);
    if (rc >= 0) {
        rc = fcntl(sock, F_SETFL, rc | O_NONBLOCK);
    }
    return rc;
}

int sock_read(const sock_t sock, void * const buff, const size_t len)
{
    return recv(sock, buff, len, 0);
}

int sock_write(const sock_t sock, const void * const buff, const size_t len)
{
    return send(sock, buff, len, 0);
}

int sock_is_recoverable(const int error)
{
    return (error == EAGAIN || error == EINTR);
}

int sock_connect_error(const sock_t sock)
{
    struct sockaddr_storage ss;
    struct sockaddr *sa = (struct sockaddr *)&ss;
    socklen_t len;
    char temp;

    memset(&ss, 0, sizeof(ss));
    len = sizeof(ss);
    sa->sa_family = AF_UNSPEC;

    /* we don't actually care about the peer name, we're just checking if
     * we're connected or not */
    if (getpeername(sock, sa, &len) == 0) {
        return 0;
    }

    /* it's possible that the error wasn't ENOTCONN, so if it wasn't,
     * return that */
    if (sock_error() != ENOTCONN) return sock_error();

    /* load the correct error into errno through error slippage */
    recv(sock, &temp, 1, 0);

    return sock_error();
}
