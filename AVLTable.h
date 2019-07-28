//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_AVLTABLE_H
#define ORDERMATCHING_AVLTABLE_H

#include "OrderTable.h"
#include "TableRow.h"


class AVLTable: public OrderTable {
private:

public:
    void insert(OrderPoint*) override;

    Node* insert(Node* root, Node* parent, OrderPoint* data);

    long forwardparse() override;

    long reverseparse() override;

    int getHeight(Node* node);

    //AVL Rotations:
    Node* rotateRR(Node* node);

    Node* rotateLL(Node* node);

    Node* rotateRL(Node* node);

    Node* rotateLR(Node* node);

    AVLTable();

    void categorizeOrder() override;

};


#endif //ORDERMATCHING_AVLTABLE_H
