#include <iostream>
#include "shared_ptr.h"
#include "item.h"

int main() {
    int x = 25;
    SharedPtr<int> a = make_shared<int>(x);
    SharedPtr<int> b(a);
    SharedPtr<int> f = make_shared<int>(1);
    SharedPtr<item> c = make_shared<item>(x, *f);
    SharedPtr<item> d = c;
    c.release();
    c = make_shared<item>(33, 44);
}
