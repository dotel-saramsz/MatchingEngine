//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_ORDERTABLE_H
#define ORDERMATCHING_ORDERTABLE_H

// This class is the abstract class that represents the order table for preorder session. It is implementation
// independent, i.e. for its implementation, it is inherited by the respective class.

class OrderTable {
protected:
    //Common members for any implementation of the order table
public:
    virtual void insert() = 0;
    virtual void remove() = 0;
    virtual void lookup() = 0;
    virtual void forwardparse() = 0;
    virtual void reverseparse() = 0;

    OrderTable();
};


#endif //ORDERMATCHING_ORDERTABLE_H
