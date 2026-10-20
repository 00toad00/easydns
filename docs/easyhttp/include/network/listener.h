#pragma once


#ifdef __cplusplus
extern "C" {
#endif
int create_tcp_listener_v4(const char *ip, unsigned short port, int backlog, int id);
#ifdef __cplusplus
}
#endif