#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>

using namespace std;

struct B {
    B() { b = new int[4]; }

    int * b;
    ~B() { delete [] b; }
    B(const B &) = delete;
    void operator= (const B & ) = delete;
};

int main(){
    auto deleter=[] (B * b){
            delete[] b;
    };
    unique_ptr<B, decltype(deleter)> pb(new B[2],deleter);
    return 0;



}
