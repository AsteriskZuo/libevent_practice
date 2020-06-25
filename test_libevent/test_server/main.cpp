//
//  main.cpp
//  test_server
//
//  Created by AsteriskZuo on 2020/6/23.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include <iostream>

#include "ref01_rot13_server_bufferevent.hpp"
#include "server2.hpp"
#include "../test_libevent/echo_server.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
//    test_ref_01_rot13_server_buffer();
//    test_server2();
    test_echo_server();
    std::cout << "Hello, World!\n";
    return 0;
}
