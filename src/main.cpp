#include <iostream>
#include "shared_ptr.h"

int main() {
    int x = 25;
    SharedPtr<int> a(x);
    SharedPtr<int> b(a);
    SharedPtr<int> f = make_shared<int>(1);
//    SharedPtr<item> c = make_shared<item>(item(x, 2));
//    SharedPtr<item> d = make_shared<item>(c);
//    c.release();
//    c = make_shared<item>(new item(33, 44));
}
