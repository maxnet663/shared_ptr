
#ifndef SHARED_PTR_ITEM_H
#define SHARED_PTR_ITEM_H

struct item {
    int x;
    double y;
    item() : x(1) , y(3.14) {}
    item(int int_x, double int_y) : x(int_x), y(int_y) {}
};

#endif //SHARED_PTR_ITEM_H
