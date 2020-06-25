//
//  main.cpp
//  test_libevent
//
//  Created by zuoyu on 2020/6/8.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include <iostream>
#include <thread>

#include "preface.hpp"
#include "preface-libevent.hpp"
#include "preface-libevent2.hpp"
#include "test_common.hpp"
#include "hello-world.hpp"

//#include "ref01_rot13_server_bufferevent.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
//    test_example_01(0, 0);
//    test_common();
//    test_ref_01_rot13_server_buffer();
    test_hello_world();
    std::cout << "hello world~" << std::endl;
    return 0;
}
