#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>

using namespace std;

struct A {
    int data;

};

void foo(unique_ptr<A> p) {
    cout << p->data << endl;
}

void foo2(shared_ptr<A> p) {
    cout << p->data << endl;
}

int main(){
    unique_ptr<A> pa(new A {4} );
    cout << pa -> data<<endl;
    foo(move(pa));

    shared_ptr<A> sa(new A{5});
    cout<< sa->data <<endl;
    foo2(sa);
    //foo2(move(sa));

    weak_ptr<A> wa=sa;
    auto wp=wa.lock();
    cout<< wp->data<<endl;
    return 0;
}
