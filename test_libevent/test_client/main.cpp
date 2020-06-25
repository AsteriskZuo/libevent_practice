//
//  main.cpp
//  test_client
//
//  Created by AsteriskZuo on 2020/6/23.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include <iostream>
#include "R6_http_client.hpp"
#include "client1.hpp"
#include "client2.hpp"
#include "../test_libevent/echo_client.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
//    test_http_client();
//    test_send_hello_world();
//    test_client();
    test_echo_client();
    std::cout << "Hello, World!\n";
    return 0;
}
