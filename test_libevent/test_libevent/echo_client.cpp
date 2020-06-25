//
//  echo_client.cpp
//  test_client
//
//  Created by AsteriskZuo on 2020/6/25.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "echo_client.hpp"

#include <event.h>
#include <sys/signal.h>
#include <string.h>
#include <iostream>


static bool echo_client();
void test_echo_client()
{
    echo_client();
}

static void bufferevent_read_cb_func(struct bufferevent *bev, void *ctx);
static void bufferevent_write_cb_func(struct bufferevent *bev, void *ctx);
static void bufferevent_event_cb_func(struct bufferevent *bev, short what, void *ctx);

static bool echo_client()
{
    bool ret = false;
    do {
        event_base* evbase = event_base_new();
        if (NULL == evbase) {
            break;
        }
        
        struct sockaddr_in sin = {0};
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(0x7f000001);
        sin.sin_port = htons(9999);
        
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sock) {
            break;
        }
        
        if (-1 == evutil_make_socket_nonblocking(sock)) {
            break;
        }
        
        bufferevent* bev = bufferevent_socket_new(evbase, sock, BEV_OPT_CLOSE_ON_FREE);
        if (NULL == bev) {
            break;
        }
        
        if (-1 == bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin))) {
            break;
        }
        
        bufferevent_setcb(bev, bufferevent_read_cb_func, bufferevent_write_cb_func, bufferevent_event_cb_func, (void*)evbase);
        bufferevent_enable(bev, EV_READ | EV_WRITE);
        
        event_base_dispatch(evbase);
        
        bufferevent_free(bev);
        
        event_base_free(evbase);
        
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        
        ret = true;
    } while (false);
    
    return ret;
}

static void bufferevent_read_cb_func(struct bufferevent *bev, void *ctx)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    static const size_t buffer_size = 1024;
    static char* buffer[buffer_size + 1] = {0};
    memset(buffer, 0, buffer_size);
    size_t data_size = 0;
    while (static_cast<void>(data_size = bufferevent_read(bev, buffer, buffer_size)), 0 < data_size) {
        std::cout << __func__ << ":" << __LINE__ << ":data=" << (char*)buffer << std::endl;
    }
    event_base_loopexit((event_base*)ctx, NULL);
}
static void bufferevent_write_cb_func(struct bufferevent *bev, void *ctx)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
}
static void bufferevent_event_cb_func(struct bufferevent *bev, short what, void *ctx)
{
    std::cout << __func__ << ":" << __LINE__ << ":flag=" << what << std::endl;
    if (BEV_EVENT_CONNECTED & what) {
        static const char* data = "connect success.";
        static const size_t data_size = strlen(data);
        if (-1 == bufferevent_write(bev, data, data_size)) {
            std::cout << __func__ << ":" << __LINE__ << ":send_message_fail" << what << std::endl;
        }
    }
}
