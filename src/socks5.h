/*
	socks5.h

	support for socks5h
*/
#ifndef __SOCKS5_H__
#define __SOCKS5_H__

int socks5_check(void);
int socks5_connect(const char *host, unsigned short port);

#endif /* __SOCKS5_H__ */
