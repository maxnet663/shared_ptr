#include <iostream>
#include "../include/shared_ptr.h"
#include "../include/item.h"

int main() {
    int x = 25;
    SharedPtr<int> a(x);
    SharedPtr<int> b(a);
    SharedPtr<item> c = make_shared<item>(x, 2);
    SharedPtr<item> d = make_shared<item>(c);
}
