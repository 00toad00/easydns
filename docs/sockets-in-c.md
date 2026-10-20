# Sockets in C: TCP, UDP and epoll

Simple examples for TCP, UDP and epoll on Linux. Each one is a single `main()` with no helper functions, so it's easy to copy, change and extend. They use the same setup steps as `docs/easyhttp/src/network/listener.c`.

Build any example with:

```bash
clang -std=c11 -Wall -Wextra -o tcp_server tcp_server.c
```

---

## The basics

A socket is a file descriptor for network communication. You create it with `socket()` and release it with `close()`.

| | TCP | UDP |
|---|---|---|
| Socket type | `SOCK_STREAM` | `SOCK_DGRAM` |
| Connection | yes: `connect()` / `accept()` | no |
| Reliable | yes | no, packets can get lost |
| Send / receive | `send()` / `recv()` | `sendto()` / `recvfrom()` |

Order of calls:

```
TCP server: socket -> setsockopt -> bind -> listen -> accept -> recv/send -> close
TCP client: socket -> connect -> send/recv -> close

UDP server: socket -> bind -> recvfrom/sendto -> close
UDP client: socket -> sendto/recvfrom -> close
```

Every example fills in the address the same way. Ports must be converted to network byte order with `htons()`, and IP strings are converted with `inet_pton()`:

```c
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port   = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
```

For IPv6, use `struct sockaddr_in6` and `AF_INET6` instead (`sin6_family`, `sin6_port`, `sin6_addr`).

---

## TCP server

Waits for a client, sends back everything it receives, and closes the connection when the client disconnects (`recv()` returns `0`).

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket creation failed: %m\n");
        return 1;
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "socket bind failed: %m\n");
        close(fd);
        return 1;
    }

    if (listen(fd, 16) < 0) {
        fprintf(stderr, "socket listen failed: %m\n");
        close(fd);
        return 1;
    }

    printf("listening on 127.0.0.1:8080\n");

    for (;;) {
        int client = accept(fd, NULL, NULL);
        if (client < 0) { continue; }

        char buf[1024];
        ssize_t n;
        while ((n = recv(client, buf, sizeof(buf), 0)) > 0) {
            send(client, buf, n, 0);
        }

        close(client);
    }

    close(fd);
    return 0;
}
```

## TCP client

Connects to the server, sends a message and prints the answer.

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket creation failed: %m\n");
        return 1;
    }

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "socket connect failed: %m\n");
        close(fd);
        return 1;
    }

    const char *msg = "hello tcp";
    send(fd, msg, strlen(msg), 0);

    char buf[1024];
    ssize_t n = recv(fd, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        buf[n] = '\0';
        printf("received: %s\n", buf);
    }

    close(fd);
    return 0;
}
```

```
$ ./tcp_server &
listening on 127.0.0.1:8080
$ ./tcp_client
received: hello tcp
```

---

## UDP server

UDP has no `listen()` or `accept()`. `recvfrom()` receives one packet and fills in the sender's address in `client`. `sendto()` sends the reply back to that address.

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8053);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket creation failed: %m\n");
        return 1;
    }

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "socket bind failed: %m\n");
        close(fd);
        return 1;
    }

    printf("listening on 127.0.0.1:8053\n");

    for (;;) {
        char buf[512];
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        ssize_t n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &len);
        if (n < 0) { continue; }

        sendto(fd, buf, n, 0, (struct sockaddr*)&client, len);
    }

    close(fd);
    return 0;
}
```

## UDP client

The client doesn't need `bind()`; it just sends to the server's address.

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8053);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket creation failed: %m\n");
        return 1;
    }

    const char *msg = "hello udp";
    sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr));

    char buf[512];
    ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0, NULL, NULL);
    if (n > 0) {
        buf[n] = '\0';
        printf("received: %s\n", buf);
    }

    close(fd);
    return 0;
}
```

```
$ ./udp_server &
listening on 127.0.0.1:8053
$ ./udp_client
received: hello udp
```

---

## epoll server

The TCP server above handles only one client at a time: while it is busy with one client, the others have to wait. With `epoll`, one loop can serve many clients. You put sockets into an epoll instance, and `epoll_wait()` tells you which of them have something to read.

| Call | Purpose |
|---|---|
| `epoll_create1(0)` | create the epoll instance |
| `epoll_ctl(ep, EPOLL_CTL_ADD, fd, &ev)` | add a socket (`EPOLL_CTL_DEL` removes it) |
| `epoll_wait(ep, events, 64, -1)` | wait until sockets are ready (`-1` = no timeout) and return how many |

`EPOLLIN` means "tell me when there's something to read". On the listening socket that means a new client is waiting. `ev.data.fd` stores which socket the event belongs to.

The setup is the same as in the TCP server. Only the loop is different:

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

int main() {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket creation failed: %m\n");
        return 1;
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "socket bind failed: %m\n");
        close(fd);
        return 1;
    }

    if (listen(fd, 16) < 0) {
        fprintf(stderr, "socket listen failed: %m\n");
        close(fd);
        return 1;
    }

    int ep = epoll_create1(0);
    if (ep < 0) {
        fprintf(stderr, "epoll creation failed: %m\n");
        close(fd);
        return 1;
    }

    struct epoll_event ev;
    ev.events  = EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(ep, EPOLL_CTL_ADD, fd, &ev);

    printf("listening on 127.0.0.1:8080\n");

    struct epoll_event events[64];
    for (;;) {
        int count = epoll_wait(ep, events, 64, -1);

        for (int i = 0; i < count; i++) {
            int cur = events[i].data.fd;

            if (cur == fd) {
                /* new client */
                int client = accept(fd, NULL, NULL);
                if (client < 0) { continue; }

                ev.events  = EPOLLIN;
                ev.data.fd = client;
                epoll_ctl(ep, EPOLL_CTL_ADD, client, &ev);
            } else {
                /* data from a client */
                char buf[1024];
                ssize_t n = recv(cur, buf, sizeof(buf), 0);
                if (n <= 0) {
                    epoll_ctl(ep, EPOLL_CTL_DEL, cur, NULL);
                    close(cur);
                    continue;
                }

                send(cur, buf, n, 0);
            }
        }
    }

    close(ep);
    close(fd);
    return 0;
}
```

Test it with several clients at once:

```
$ ./epoll_server &
listening on 127.0.0.1:8080
$ ./tcp_client & ./tcp_client & ./tcp_client
received: hello tcp
received: hello tcp
received: hello tcp
```

---

## Ideas for extending the examples

- **Another IP or port:** change `htons(8080)` and `"127.0.0.1"`. `"0.0.0.0"` accepts connections from all network interfaces.
- **Show who connected:** pass a `struct sockaddr_in` to `accept()` (like `recvfrom()` in the UDP server) and print it with `inet_ntop()` and `ntohs()`.
- **UDP in epoll:** create the UDP socket like in the UDP server, add it with `epoll_ctl()` and `EPOLLIN`, and call `recvfrom()`/`sendto()` when `cur` is that socket.
- **Timeout for the UDP client:** a lost packet makes `recvfrom()` wait forever. Set a timeout before receiving:
  ```c
  struct timeval tv = { .tv_sec = 2, .tv_usec = 0 };
  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  ```

## Good to know

- `recv()` returns `0` when the other side has closed the connection, and `-1` on errors.
- TCP is a byte stream. A message can arrive split over several `recv()` calls, or several messages can arrive in one. For bigger or binary data, put the message length in front (DNS over TCP uses 2 bytes for this) and keep calling `recv()` until everything has arrived.
- `send()` may send fewer bytes than you passed. Check the return value for large amounts of data.
- With UDP, one `sendto()` is exactly one `recvfrom()` on the other side.

Useful tools: `nc 127.0.0.1 8080` (TCP), `nc -u 127.0.0.1 8053` (UDP), `ss -tlnp` / `ss -ulnp` (open ports), and the man pages `socket(7)`, `tcp(7)`, `udp(7)` and `epoll(7)`.
