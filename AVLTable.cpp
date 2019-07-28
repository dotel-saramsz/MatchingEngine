//
// Created by dotelsaramsha on 4/29/19.
//

#include "AVLTable.h"
#include <iostream>
#include "Transaction.h"
#include "TransactionLog.h"
#include <chrono>

extern long matchStartTime;

using namespace std;

void AVLTable::insert(OrderPoint* orderPoint) {
    treeRoot = insert(treeRoot,nullptr,orderPoint);
    cout<<"[AVL] Inserted Order: "<<orderPoint->type<<" | "<<orderPoint->orderID<<" | "<<orderPoint->price<<" | "<<orderPoint->shareQty<<endl;
}

OrderTable::Node* AVLTable::insert(Node* root, Node* parent,OrderPoint* data) {
    //Recursively traverse the tree to insert the data at the correct position
    //Here if the Node having data element (TableRow) has the same price as the data (OrderPoint),
    //add to the list of orders in that TableRow

    //case1: if its the entry position, i.e root=NULL
    if(root==nullptr) {
        root = new Node();
        TableRow* row = new TableRow(data->price, data, data->shareQty * (1-data->type), data->shareQty * data->type);
        root->left = nullptr;
        root->right = nullptr;
        root->parent = parent;
        root->data = row;
        root->height = 0;
//        cout<<"[AVL] New TableRow inserted at price: "<<data->price<<endl;
    }
    //case2: if the orderprice is less than price in root's data element
    else if(data->price < root->data->price) {
        root->left = insert(root->left,root,data);
        //AVL rotations
        if(getHeight(root->left) - getHeight(root->right) == 2) {
            if(data->price < root->left->data->price) {
                //LL rotation
                root = rotateLL(root);
            }
            else if(data->price > root->left->data->price) {
                //LR rotation
                root = rotateLR(root);
            }
        }
    }
    //case3: if the orderprice is more than price in root's data element
    else if(data->price > root->data->price) {
        root->right = insert(root->right,root,data);
        //AVL rotations
        if(getHeight(root->right) - getHeight(root->left) == 2) {
            if(data->price > root->right->data->price) {
                //RR rotation
                root = rotateRR(root);
            }
            else if(data->price < root->right->data->price) {
                //RL rotation
                root = rotateRL(root);
            }
        }
    }
    //case4: if the orderprice is equal to the price in root's data element, i.e CHAINING case
    else if(data->price == root->data->price) {
        root->data->addOrder(data);
        root->data->buyQty += data->shareQty * (1-data->type);
        root->data->sellQty += data->shareQty * data->type;
//        cout<<"[AVL] Chaining of order at TableRow, orderID: "<<data->orderID<<endl;
    }

    root->height = std::max(getHeight(root->left),getHeight(root->right)) + 1;

    return root;
}

AVLTable::AVLTable() {

}


int AVLTable::getHeight(OrderTable::Node *node) {
    if (node==nullptr){
        return -1;
    }
    else {
        return node->height;
    }
}

OrderTable::Node *AVLTable::rotateRR(OrderTable::Node *node) {
    Node* X = node->right;
    node->right = X->left;
    X->left = node;
    node->height = std::max(getHeight(node->left),getHeight(node->right)) + 1;
    X->height = std::max(getHeight(X->right),node->height) + 1;
    return X;
}

OrderTable::Node *AVLTable::rotateLL(OrderTable::Node *node) {
    Node* X = node->left;
    node->left = X->right;
    X->right = node;
    node->height = std::max(getHeight(node->left),getHeight(node->right)) + 1;
    X->height = std::max(getHeight(X->left),node->height) + 1;
    return X;
}

OrderTable::Node *AVLTable::rotateRL(OrderTable::Node *node) {

    node->right = rotateLL(node->right);
    return rotateRR(node);
}

OrderTable::Node *AVLTable::rotateLR(OrderTable::Node *node) {
    node->left = rotateRR(node->left);
    return rotateLL(node);
}


long AVLTable::forwardparse() {
    return OrderTable::forwardparse(treeRoot, 0);
}

long AVLTable::reverseparse() {
    return OrderTable::reverseparse(treeRoot, 0);
}

void AVLTable::categorizeOrder() {
    OrderTable::categorizeOrder(treeRoot);
}


