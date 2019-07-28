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

    Node* insert(Node* root, Node* parent, OrderPoint* data);

    long forwardparse() override;

    long reverseparse() override;

    long forwardparse(Node *, long);

    long reverseparse(Node *, long);

    int getHeight(Node* node);

    //AVL Rotations:
    Node* rotateRR(Node* node);

    Node* rotateLL(Node* node);

    Node* rotateRL(Node* node);

    Node* rotateLR(Node* node);

    AVLTable();

    void categorizeOrder() override;

    void categorizeOrder(Node* node);

};


#endif //ORDERMATCHING_AVLTABLE_H
