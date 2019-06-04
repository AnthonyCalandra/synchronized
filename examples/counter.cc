#include <iostream>
#include <thread>
#include "../synchronized.hpp"

#define ITERATIONS 1000000

using namespace synchronized;

void foo(synchronized<unsigned>& syncUnsigned) {
    for (unsigned i = 0; i < ITERATIONS; i++) {
        ++syncUnsigned.value();
    }
}

void bar(synchronized<unsigned>& syncUnsigned) {
    for (unsigned i = 0; i < ITERATIONS; i++) {
        syncUnsigned.value()++;
    }
}

int main() {
    auto syncUnsigned = make_synchronized<unsigned>(0);
    std::thread helper1 {foo, std::ref(syncUnsigned)};
    std::thread helper2 {bar, std::ref(syncUnsigned)};
    helper1.join();
    helper2.join();
    std::cout << syncUnsigned.value() << std::endl;
    return 0;
}
