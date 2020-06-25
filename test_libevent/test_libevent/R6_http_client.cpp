//
//  R6_http_client.cpp
//  test_client
//
//  Created by AsteriskZuo on 2020/6/24.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "R6_http_client.hpp"

/* Don't actually copy this code: it is a poor way to implement an
   HTTP client.  Have a look at evhttp instead.
*/
#include <event2/dns.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>

#include <stdio.h>

static int R6_http_client(int argc, char **argv);
static int R6_http_client2(int argc, char **argv);
void test_http_client()
{
    int argc = 3;
//    char* argv[] = { "test", "www.leiphone.com", "news/202006/0UhH6XClGWU6a9Fq.html" };
//    char* argv[] = { "test", "www.leiphone.com", "" };
    char* argv[] = { "test", "www.baidu.com", "HTTP/1.1" };
    R6_http_client(argc, argv);
}


static void readcb(struct bufferevent *bev, void *ptr)
{
    char buf[1024];
    int n;
    struct evbuffer *input = bufferevent_get_input(bev);
    while ((n = evbuffer_remove(input, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, n, stdout);
    }
}

static void writecb(struct bufferevent *bev, void *ptr)
{
    printf("writecb okay.\n");
//    const char* query = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n";
//    bufferevent_write(bev, query, sizeof(query));
}

static void eventcb(struct bufferevent *bev, short events, void *ptr)
{
    if (events & BEV_EVENT_CONNECTED) {
         printf("Connect okay.\n");
//        const char* query = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n";
//        bufferevent_write(bev, query, sizeof(query));
    } else if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {
         struct event_base *base = (event_base *)ptr;
         if (events & BEV_EVENT_ERROR) {
         int err = bufferevent_socket_get_dns_error(bev);
         if (err)
             printf("DNS error: %s\n", evutil_gai_strerror(err));
         }
         printf("Closing\n");
         bufferevent_free(bev);
         event_base_loopexit(base, NULL);
    }
}

static int R6_http_client(int argc, char **argv)
{
    struct event_base *base;
    struct evdns_base *dns_base;
    struct bufferevent *bev;

    if (argc != 3) {
        printf("Trivial HTTP 0.x client\n"
               "Syntax: %s [hostname] [resource]\n"
               "Example: %s www.google.com /\n",argv[0],argv[0]);
        return 1;
    }

    base = event_base_new();
    dns_base = evdns_base_new(base, 1);

    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, readcb, NULL, eventcb, base);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    evbuffer_add_printf(bufferevent_get_output(bev), "GET / %s\r\nHost: %s\r\n\r\n", argv[2], argv[1]);
//    evbuffer_add_printf(bufferevent_get_output(bev), "GET / %s\r\n", argv[2]);// error
    bufferevent_socket_connect_hostname(
        bev, dns_base, AF_UNSPEC, argv[1], 80);
    event_base_dispatch(base);
    return 0;
}

static int R6_http_client2(int argc, char **argv)
{
    struct event_base *base;
    struct evdns_base *dns_base;
    struct bufferevent *bev;

    if (argc != 3) {
        printf("Trivial HTTP 0.x client\n"
               "Syntax: %s [hostname] [resource]\n"
               "Example: %s www.google.com /\n",argv[0],argv[0]);
        return 1;
    }

    base = event_base_new();
    dns_base = evdns_base_new(base, 1);

    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, readcb, writecb, eventcb, base);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
//    evbuffer_add_printf(bufferevent_get_output(bev), "GET / %s\r\nHost: %s\r\n\r\n", argv[2], argv[1]);
//    evbuffer_add_printf(bufferevent_get_output(bev), "GET / %s\r\n", argv[2]);// error
    bufferevent_socket_connect_hostname(
        bev, dns_base, AF_UNSPEC, argv[1], 80);
    event_base_dispatch(base);
    return 0;
}
