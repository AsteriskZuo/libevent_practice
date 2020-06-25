//
//  test_common.cpp
//  test_libevent
//
//  Created by AsteriskZuo on 2020/6/22.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "test_common.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>

static void test_xor();
static void test_xor2();
void test_common()
{
    test_xor2();
}
static void test_xor()
{
    static const char* content = "key";
    std::size_t content_size = std::strlen(content);
    static std::uint8_t confuse_key[] = { 0x67 };
    char* input = (char*)std::malloc(content_size + 1);
    char* output = (char*)std::malloc(content_size + 1);
    std::memcpy(input, content, content_size);
    
    for (int i = 0 ; i < content_size + 1; ++i) {
        output[i] = input[i] ^ confuse_key[0];
    }
    
    for (int i = 0 ; i < content_size + 1; ++i) {
        output[i] = output[i] ^ confuse_key[0];
    }
    
    int ret = std::strncmp(input, output, content_size + 1);
    
    int a = ret;
}
static void test_xor2()
{
    static const char* content = "key";
    std::size_t content_size = std::strlen(content);
    static std::uint8_t confuse_key[] = { 0x67, 0x8d, 0xef };
    static std::size_t confuse_key_size = sizeof(confuse_key) / sizeof(std::uint8_t);
    char* input = (char*)std::malloc(content_size + 1);
    char* output = (char*)std::malloc(content_size + 1);
    std::memcpy(input, content, content_size);
    
    for (int i = 0 ; i < content_size + 1; ++i) {
        output[i] = input[i];
        for (int j = 0; j < confuse_key_size; ++j) {
            output[i] ^= confuse_key[j];
        }
    }
    
    for (int i = 0 ; i < content_size + 1; ++i) {
        for (int j = 0; j < confuse_key_size; ++j) {
            output[i] ^= confuse_key[j];
        }
    }
    
    int ret = std::strncmp(input, output, content_size + 1);
    
    int a = ret;
}
