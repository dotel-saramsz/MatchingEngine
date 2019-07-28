/*
    Implementing custom std::map() with some minor tweaks!
    Uses Red-Black Tree which can accommodate duplicate key entries
*/

#ifndef ORDERMATCHING_MAPTABLE_H
#define ORDERMATCHING_MAPTABLE_H

#include "OrderTable.h"
#include "TableRow.h"

enum Color{
    RED,
    BLACK
};

class MapTable: public OrderTable {
private:

public:
    void insert(OrderPoint*) override;

    void rotateLeft(Node *&);

    void rotateRight(Node *&);

    int getColor(Node *&);

    void setColor(Node *&, int);

    void fixInsertViolation(Node *&); //color correction and rotation

    Node* insert(Node* root, Node* parent, OrderPoint* data);

    long forwardparse() override;

    long reverseparse() override;

    MapTable(); //constructor

    void categorizeOrder() override;

};


#endif //ORDERMATCHING_MAPTABLE_H
