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
    //define a Red-Black Tree Node
    class Node {
    public:
        int color;
        Node* left;
        Node* right;
        Node* parent;
        TableRow* data; //Pointer because copying a complex object can have overhead
        //CHECK: bad referencing due to out of scope?
        int height;

    };
    Node* treeRoot;

public:
    void insert(OrderPoint*) override;

    void remove(OrderPoint*) override;

    void lookup(OrderPoint*) override;

    void calculateEQprice() override;

    void rotateLeft(Node *&);

    void rotateRight(Node *&);

    int getColor(Node *&);

    void setColor(Node *&, int);

    void fixInsertViolation(Node *&); //color correction and rotation

    Node* insert(Node* root, Node* parent, OrderPoint* data);

    long forwardparse(Node *, long);

    long reverseparse(Node *, long);

    MapTable(); //constructor

    void matchPreOpen(BuyOrderBook *pendingBuy, SellOrderBook *pendingSell) override;
};


#endif //ORDERMATCHING_MAPTABLE_H
