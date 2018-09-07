#include <iostream>
#include <thread>
#include "../synchronized.hpp"

#define ITERATIONS 1000000

void foo(synchronized::unique_synchronized<unsigned>& syncUnsigned) {
    for (unsigned i = 0; i < ITERATIONS; i++) {
        ++syncUnsigned();
    }
}

void bar(synchronized::unique_synchronized<unsigned>& syncUnsigned) {
    for (unsigned i = 0; i < ITERATIONS; i++) {
        syncUnsigned()++;
    }
}

int main() {
    auto syncUnsigned = synchronized::make_synchronized<unsigned>(0);
    std::thread helper1 {foo, std::ref(syncUnsigned)};
    std::thread helper2 {bar, std::ref(syncUnsigned)};
    helper1.join();
    helper2.join();
    std::cout << syncUnsigned() << std::endl;
    return 0;
}
