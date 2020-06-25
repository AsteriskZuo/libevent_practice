//
//  echo_server.cpp
//  test_server
//
//  Created by AsteriskZuo on 2020/6/25.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "echo_server.hpp"
#include <iostream>
#include <string.h>

static const int PORT = 9999;

static bool echo_server_main();
void test_echo_server()
{
    echo_server_main();
}

#include <event.h>
#include <event2/listener.h>
#include <sys/signal.h>

static void evconnlistener_cb_func(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr * sockaddr, int socklen, void *arg);
static void signal_cb_func(evutil_socket_t fd, short what, void *arg);

static void bufferevent_read_cb_func(struct bufferevent *bev, void *ctx);
static void bufferevent_write_cb_func(struct bufferevent *bev, void *ctx);
static void bufferevent_event_cb_func(struct bufferevent *bev, short what, void *ctx);

static bool echo_server_main()
{
    bool ret = false;
    do {
        event_base* evbase = event_base_new();
        if (NULL == evbase) {
            std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
            break;
        }
        
        struct sockaddr_in sin = {0};
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);
        evconnlistener* evlistener = evconnlistener_new_bind(evbase, evconnlistener_cb_func, (void*)evbase, LEV_OPT_CLOSE_ON_FREE, -1, (sockaddr*)&sin, sizeof(sin));
        if (NULL == evlistener) {
            std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
            break;
        }
        
        event* evsig = event_new(evbase, SIGINT, EV_SIGNAL|EV_PERSIST, signal_cb_func, (void*)evbase);
        if (NULL == evsig) {
            std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
            break;
        }
        if (0 > event_add(evsig, NULL)) {
            std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
            break;
        }
        
        event_base_dispatch(evbase);
        
        evconnlistener_free(evlistener);
        
        event_free(evsig);
        
        event_base_free(evbase);
        
        ret = true;
        
        std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
        
    } while (false);
    
    return ret;
}

static void evconnlistener_cb_func(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr * sockaddr, int socklen, void *arg)
{
    bool ret = false;
    do {
        if (-1 == evutil_make_socket_nonblocking(fd)) {
            break;
        }
        event_base* base = (event_base*)arg;
        if (NULL == base) {
            break;
        }
        bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if (NULL == bev) {
            break;
        }
        bufferevent_setcb(bev, bufferevent_read_cb_func, bufferevent_write_cb_func, bufferevent_event_cb_func, arg);
        bufferevent_enable(bev, EV_READ | EV_WRITE);
        ret = true;
    } while (false);
//    return ret;
}

static void signal_cb_func(evutil_socket_t sig, short what, void *arg)
{
    //EV_TIMEOUT
    std::cout << "echo_server_" << __func__ << ":" << __LINE__ << ":evflag=" << what << std::endl;
    event_base_loopexit((event_base*)arg, NULL);
}

static void bufferevent_read_cb_func(struct bufferevent *bev, void *ctx)
{
    std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
    static const size_t buffer_size = 1024;
    static char* buffer[buffer_size + 1] = {0};
    memset(buffer, 0, buffer_size);
    bufferevent_read(bev, buffer, buffer_size);
    size_t data_size = strlen((char*)buffer);
    bufferevent_write(bev, buffer, data_size);
}
static void bufferevent_write_cb_func(struct bufferevent *bev, void *ctx)
{
    std::cout << "echo_server_" << __func__ << ":" << __LINE__ << std::endl;
}
static void bufferevent_event_cb_func(struct bufferevent *bev, short what, void *ctx)
{
    std::cout << "echo_server_" << __func__ << ":" << __LINE__ << ":evflag=" << what << std::endl;
}


//#define BEV_EVENT_READING    0x01    /**< error encountered while reading */
//#define BEV_EVENT_WRITING    0x02    /**< error encountered while writing */
//#define BEV_EVENT_EOF        0x10    /**< eof file reached */
//#define BEV_EVENT_ERROR        0x20    /**< unrecoverable error encountered */
//#define BEV_EVENT_TIMEOUT    0x40    /**< user-specified timeout reached */
//#define BEV_EVENT_CONNECTED    0x80    /**< connect operation finished. */

