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

AVLTable::Node* AVLTable::insert(Node* root, Node* parent,OrderPoint* data) {
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
    treeRoot = nullptr; //Initially, the treeRoot is NULL
    maxTradableQty = 0;
}


int AVLTable::getHeight(AVLTable::Node *node) {
    if (node==nullptr){
        return -1;
    }
    else {
        return node->height;
    }
}

AVLTable::Node *AVLTable::rotateRR(AVLTable::Node *node) {
    Node* X = node->right;
    node->right = X->left;
    X->left = node;
    node->height = std::max(getHeight(node->left),getHeight(node->right)) + 1;
    X->height = std::max(getHeight(X->right),node->height) + 1;
    return X;
}

AVLTable::Node *AVLTable::rotateLL(AVLTable::Node *node) {
    Node* X = node->left;
    node->left = X->right;
    X->right = node;
    node->height = std::max(getHeight(node->left),getHeight(node->right)) + 1;
    X->height = std::max(getHeight(X->left),node->height) + 1;
    return X;
}

AVLTable::Node *AVLTable::rotateRL(AVLTable::Node *node) {
    node->right = rotateLL(node->right);
    return rotateRR(node);
}

AVLTable::Node *AVLTable::rotateLR(AVLTable::Node *node) {
    node->left = rotateRR(node->left);
    return rotateLL(node);
}

long AVLTable::forwardparse(Node *node, long oldSupply) {
    // inorder traverse (ascending order traverse) the AVL tree and return the cumulative supply to the successor
    if(node == nullptr)
        return oldSupply;
    long supply = forwardparse(node->left, oldSupply);
    //do what you want with the current node: Display and add up the sellQty
    supply += node->data->sellQty;
    node->data->supplyQty = supply;
//    cout<<node->data->price<<" | "<<node->data->buyQty<<" | "<<node->data->sellQty<<" | "<<node->data->demandQty<<" | "<<node->data->supplyQty<<endl;
    //go to the right subtree
    long newSupply = forwardparse(node->right, supply);
    return newSupply;
}

long AVLTable::reverseparse(AVLTable::Node *node, long oldDemand) {
    // reverse inorder traverse (descending order traverse) the AVL tree and return the cumulative demand to the successor
    if(node == nullptr)
        return oldDemand;
    long demand = reverseparse(node->right, oldDemand);
    //do what you want with the current node: Display and add up the buyQty and check if max tradable
    demand += node->data->buyQty;
    node->data->demandQty = demand;
    node->data->tradableQty = std::min(node->data->demandQty,node->data->supplyQty);
    node->data->unmatchedQty = node->data->demandQty-node->data->supplyQty;
    //cases for setting the equilibrium price
    //case 1
    if (node->data->tradableQty > maxTradableQty) {
        maxTradableQty = node->data->tradableQty;
        equilibriumRows.clear();
        equilibriumRows.push_back(make_pair(node->data->price,node->data->unmatchedQty));
    }
    //case 2
    else if (node->data->tradableQty == maxTradableQty) {
        equilibriumRows.push_back(make_pair(node->data->price,node->data->unmatchedQty));
    }
    //case 3 and 4 can be added after prev day closing price is added
    cout<<node->data->price<<" | "<<node->data->buyQty<<" | "<<node->data->sellQty<<" | "<<node->data->demandQty<<" | "<<node->data->supplyQty<<" | "<<node->data->tradableQty<<" | "<<node->data->unmatchedQty<<endl;
    //go to the right subtree
    long newDemand = reverseparse(node->left, demand);
    return newDemand;
}


void AVLTable::categorizeOrder(AVLTable::Node *node) {
    if(node == nullptr){
        return;
    }
    //traverse to the left child
    categorizeOrder(node->left);
    //Process the current node. Each node can have multiple orders.
    //Check if the node is eq node or not and categorize accordingly
    if(node->data->price < equilibriumPrice) {
        //add to category B or C (Eligible sell or Pending Buy)
        for (auto order:node->data->orders) {   //new range based for-loop in C++
            if(order->type == OrderPoint::BUY) {
                pendingBuy->push(order);
            }
            else {
                eligibleSell.push_front(order);
            }
        }
    }
    else if(node->data->price > equilibriumPrice) {
        //add to category A or D (Eligible Buy or Pending Sell)
        for (auto order:node->data->orders) {
            if(order->type == OrderPoint::BUY) {
                eligibleBuy.push_front(order);
            }
            else {
                pendingSell->push(order);
            }
        }
    }
    else {
        //add to A or B (Eligible buy or Eligible sell)
        for (auto order:node->data->orders) {
            if(order->type == OrderPoint::BUY) {
                eligibleBuy.push_front(order);
            }
            else {
                eligibleSell.push_front(order);
            }
        }
    }
    //traverse to the right child
    categorizeOrder(node->right);
}


long AVLTable::forwardparse() {
    return forwardparse(treeRoot, 0);
}

long AVLTable::reverseparse() {
    return reverseparse(treeRoot, 0);
}

void AVLTable::categorizeOrder() {
    categorizeOrder(treeRoot);
}


