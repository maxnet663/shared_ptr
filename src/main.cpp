#include <iostream>
#include "shared_ptr.hpp"
#include "item.h"

int main() {
    int x = 25;
    SharedPtr<int> a = make_shared<int>(x);
    SharedPtr<int> b(a);
    b = b;
    SharedPtr<int> f = make_shared<int>(1);
    SharedPtr<item> c = make_shared<item>(x, *f);
    SharedPtr<item> t = make_shared<item>(-1, 22);
    SharedPtr<item> d = c;
    c.release();
    c = make_shared<item>(33, 44);
}
