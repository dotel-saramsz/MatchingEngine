//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_AVLTABLE_H
#define ORDERMATCHING_AVLTABLE_H

#include "OrderTable.h"
#include "TableRow.h"


class AVLTable: public OrderTable {
private:
    //Here, the class that represents the tree node is to be defined
    class Node {
    public:
        Node* left;
        Node* right;
        TableRow* data; //Pointer because copying a complex object can have overhead
        //CHECK: bad referencing due to out of scope?
        int height;
        Node* parent;
    };
    Node* treeRoot;

public:
    void insert(OrderPoint*) override;

    void remove(OrderPoint*) override;

    void lookup(OrderPoint*) override;

    void calculateEQprice() override;

    Node* insert(Node* root, Node* parent, OrderPoint* data);

    Node* lookup(TableRow& data);

    long forwardparse(Node *, long);

    long reverseparse(Node *, long);

    int getHeight(Node* node);

    //AVL Rotations:
    Node* rotateRR(Node* node);

    Node* rotateLL(Node* node);

    Node* rotateRL(Node* node);

    Node* rotateLR(Node* node);

    AVLTable();

    void matchPreOpen(BuyOrderBook *pendingBuy, SellOrderBook *pendingSell) override;

    void categorizeOrder(Node* node);

};


#endif //ORDERMATCHING_AVLTABLE_H
