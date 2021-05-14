/*
	socks5.c

	support for socks5h
*/
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/un.h>

int socks_not_defined = 0;
char env_addr[15] = { 0 }, env_port[6] = { 0 };
uint32_t socks_addr = 0;
uint16_t socks_port = 0;

int status_s = -1;
char status_path[] = "/tmp/libmesode+tor.socket";

long socks5_parse_long(const char *str, int *success)
{
	int base = 0;
	char *end_ptr = NULL;
	long val = 0;

	errno = 0;
	val = strtol(str, &end_ptr, base);

	*success = 0;
	if (str == end_ptr)
		return 0;
	
	if (errno == ERANGE)
		return 0;
	
	if (*end_ptr)
		return 0;
	
	*success = 1;
	return val;
}

int socks5_check(void)
{
	char *env_proxy = NULL;
	char *s = NULL, *d = NULL;
	int c = 0, success = 0;
	
	if (socks_not_defined)
		return 0;
	
	if (socks_addr && socks_port)
		return 1;
	
	socks_not_defined = 1;
	if ((env_proxy = getenv("SOCKS_PROXY")) == NULL)
		return 0;
	
	if (*(s = env_proxy))
	{
		d = env_addr;
		while(c++ < 15 && *s && *s != ':')
			*d++ = *s++;
		
		while(*s && *s != ':')
			s++;
		
		if (*s)
		{
			s++;
			d = env_port;
			c = 0;
			while(c++ < 5 && *s)
				*d++ = *s++;
		}
	}
	
	if (!inet_pton(AF_INET, env_addr, &socks_addr))
		return 0;
	
	socks_port = htons((uint16_t)socks5_parse_long(env_port, &success));
	if (!success)
		return 0;
	
	socks_not_defined = 0;
	return 1;
}

int socks5_establish(int s)
{
	unsigned char request[3] = { 5, 1, 0 }, reply[2] = { 0, 0 };
	ssize_t send_sz = 0, recv_sz = 0;
	
	if ((send_sz = send(s, request, 3, 0)) != -1)
		if ((recv_sz = recv(s, reply, 2, 0)) != -1)
			if (reply[0] == 5 && !reply[1])
				return 1;

	errno = ECONNREFUSED;
	return 0;
}

int socks5_cmd_connect(int s, const char *host, unsigned short port)
{
	unsigned char request[262] = { 5, 1, 0, 3 }, reply[22] = { 5, 5 };
	ssize_t request_len = 0;
	ssize_t send_sz = 0, recv_sz = 0;
	
	request[4] = (unsigned char)strlen(host);
	strncpy((char*)&request[5], host, 255);
	*((uint16_t*)&request[4 + request[4] + 1]) = htons(port);
	request_len = 7 + request[4];
	
	if ((send_sz = send(s, request, request_len, 0)) != -1)
		if ((recv_sz = recv(s, reply, 22, 0)) != -1)
			if (reply[0] == 5 && !reply[1])
				return 1;

	switch(reply[1])
	{
		case 3: errno = ENETUNREACH; break;
		case 4: errno = EHOSTUNREACH; break;
		case 6: errno = ETIMEDOUT; break;
		case 7: errno = EADDRNOTAVAIL; break;
		default: errno = ECONNREFUSED; break;
	}

	return 0;
}

int socks5_connect(const char *host, unsigned short port)
{
	int proxy_s = -1;
	struct sockaddr_in proxy_addr;
	socklen_t proxy_addrlen = 0;
	
	if ((proxy_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return -1;
	
	memset(&proxy_addr, 0, sizeof(struct sockaddr_in));
	proxy_addrlen = sizeof(struct sockaddr_in);
	proxy_addr.sin_family = AF_INET;
	memcpy(&proxy_addr.sin_addr, &socks_addr, 4);
	memcpy(&proxy_addr.sin_port, &socks_port, 2);
	
	if (!connect(proxy_s, (struct sockaddr*)&proxy_addr, proxy_addrlen))
		if (socks5_establish(proxy_s))
			if (socks5_cmd_connect(proxy_s, host, port))
				return proxy_s;
	
	return -1;
}
