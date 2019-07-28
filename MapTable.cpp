#include "MapTable.h"
#include <iostream>
#include "TransactionLog.h"
#include "Transaction.h"

extern long matchStartTime;

using namespace std;

//constructor
MapTable::MapTable() {

}

//get node color
int MapTable::getColor(OrderTable::Node *&node) {
    if(node== nullptr)
        return BLACK;

    return node->color;
}


//set the color of node
void MapTable::setColor(OrderTable::Node *&node, int color) {
    if(node== nullptr)
        return;

    node->color=color;
}

void MapTable::insert(OrderPoint* orderPoint) {
//  insert orders
    treeRoot = insert(treeRoot,nullptr,orderPoint);
    fixInsertViolation(treeRoot);
    cout<<"[MAP] Inserted/Updated treeNode with Price= "<<orderPoint->price<<endl;
}

//left rotation : same as AVL
void MapTable::rotateLeft(OrderTable::Node *&node) {
    Node *right_child = node->right;
    node->right = right_child->left;

    if (node->right != nullptr)
        node->right->parent = node;

    right_child->parent = node->parent;

    if (node->parent == nullptr)
        treeRoot = right_child;
    else if (node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;

    right_child->left = node;
    node->parent = right_child;
}

//right rotation: same as AVL
void MapTable::rotateRight(OrderTable::Node *&node) {
    Node *left_child=node->left;
    node->left=left_child->right;

    if(node->left!=nullptr){
        node->left->parent=node;
    }
    left_child->parent = node->parent;

    if (node->parent == nullptr)
        treeRoot = left_child;
    else if (node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;

    left_child->right = node;
    node->parent = left_child;
}


OrderTable::Node* MapTable::insert(OrderTable::Node *root, OrderTable::Node *parent, OrderPoint *data) {
    //insert nodes here

    //case 1: if tree is empty
    if(root==nullptr) {
        root = new Node();
        TableRow* row = new TableRow(data->price, data, data->shareQty * (1-data->type), data->shareQty * data->type);
        root->left = nullptr;
        root->right = nullptr;
        root->parent = parent;
        root->data = row;
        root->color=BLACK;
        cout<<"[MAP] New TableRow inserted at price: "<<data->price<<endl;
    }
        //case2: if the orderprice is less than price in root's data element
    else if(data->price < root->data->price) {
        root->left = insert(root->left,root,data);
        root->left->parent=root;
    }
        //case3: if the orderprice is more than price in root's data element
    else if(data->price > root->data->price) {
        root->right = insert(root->right,root,data);
        root->right->parent=root;
     }
        //case4: if the orderprice is equal to the price in root's data element, i.e CHAINING case
    else if(data->price == root->data->price) {
        root->data->addOrder(data);
        root->data->buyQty += data->shareQty * (1-data->type);
        root->data->sellQty += data->shareQty * data->type;
        cout<<"[MAP] Chaining of order at TableRow, orderID: "<<data->orderID<<endl;
    }


    return root;
}

/*
 * In Red-Black Tree, Recoloring and Rotation are used for balancing tree.
 *
 * This method "fixInsertViolation(Node *&)" performs recoloring first and if it doesn't work,it goes for rotation.
 * 1. If uncle is RED, recoloring
 * 2. If uncle is BLACK, rotation and/or recoloring
 */
void MapTable::fixInsertViolation(OrderTable::Node *&node) {

    Node *parent = nullptr;
    Node *grandparent = nullptr;
    while (node!= treeRoot && getColor(node) == RED && getColor(node->parent) == RED) {
        parent = node->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left) {
            Node *uncle = grandparent->right;
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            } else {
                if (node== parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateRight(grandparent);
                swap(parent->color, grandparent->color);
                node = parent;
            }
        } else {
            Node *uncle = grandparent->left;
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            } else {
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                node = parent;
            }
        }
    }
    setColor(treeRoot, BLACK);

}

long MapTable::forwardparse() {
    return OrderTable::forwardparse(treeRoot, 0);
}

long MapTable::reverseparse() {
    return OrderTable::reverseparse(treeRoot, 0);
}

void MapTable::categorizeOrder() {
    OrderTable::categorizeOrder(treeRoot);
}
