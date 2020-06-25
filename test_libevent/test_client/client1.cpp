//
//  client1.cpp
//  test_client
//
//  Created by AsteriskZuo on 2020/6/23.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "client1.hpp"

#include <iostream>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#define MAX_LINE 16384

static void bufferevent_data_read_cb_func(struct bufferevent *bev, void *ctx)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
//    event_base_loopexit(bufferevent_get_base(bev), nullptr);
}
static void bufferevent_data_write_cb_func(struct bufferevent *bev, void *ctx)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
}
static void bufferevent_event_cb_func(struct bufferevent *bev, short what, void *ctx)
{
    std::cout << __func__ << ":" << __LINE__ << ":what=" << what << std::endl;
}

void test_send_hello_world()
{
    evutil_socket_t sock;
    struct sockaddr_in sin;
    struct event_base *base;
    struct bufferevent* bev;
    
    base = event_base_new();
    if (!base)
        return; /*XXXerr*/
    
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0x7f000001);
    sin.sin_port = htons(9995);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(sock);
    
    bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
        return;
    
    bufferevent_setcb(bev, bufferevent_data_read_cb_func, bufferevent_data_write_cb_func, bufferevent_event_cb_func, NULL);
    
    bufferevent_setwatermark(bev, EV_READ | EV_WRITE, 0, MAX_LINE);
    
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    int ret = bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin));
    
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    event_base_dispatch(base);
    
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    bufferevent_free(bev);
    
    event_base_free(base);
}
