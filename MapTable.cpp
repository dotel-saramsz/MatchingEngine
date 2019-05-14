#include "MapTable.h"
#include <iostream>

using namespace std;

//constructor
MapTable::MapTable() {
    treeRoot= nullptr;
}

//get node color
int MapTable::getColor(MapTable::Node *&node) {
    if(node== nullptr)
        return BLACK;

    return node->color;
}


//set the color of node
void MapTable::setColor(MapTable::Node *&node, int color) {
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
void MapTable::remove(OrderPoint* orderPoint) {

}

void MapTable::lookup(OrderPoint* orderPoint) {

}

//left rotation : same as AVL
void MapTable::rotateLeft(MapTable::Node *&node) {
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
void MapTable::rotateRight(MapTable::Node *&node) {
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


MapTable::Node* MapTable::insert(MapTable::Node *root, MapTable::Node *parent, OrderPoint *data) {
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

    //root->height = std::max(getHeight(root->left),getHeight(root->right)) + 1;

    return root;
}

/*
 * In Red-Black Tree, Recoloring and Rotation are used for balancing tree.
 *
 * This method "fixInsertViolation(Node *&)" performs recoloring first and if it doesn't work,it goes for rotation.
 * 1. If uncle is RED, recoloring
 * 2. If uncle is BLACK, rotation and/or recoloring
 */
void MapTable::fixInsertViolation(MapTable::Node *&node) {

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


long MapTable::forwardparse(MapTable::Node *node, long oldSupply) {
    // inorder traverse (ascending order traverse) the AVL tree and return the cumulative supply to the successor
    if(node == nullptr)
        return oldSupply;
    long supply = forwardparse(node->left, oldSupply);
    //do what you want with the current node: Display and add up the sellQty
    supply += node->data->sellQty;
    node->data->supplyQty = supply;
    cout<<node->data->price<<" | "<<node->data->buyQty<<" | "<<node->data->sellQty<<" | "<<node->data->demandQty<<" | "<<node->data->supplyQty<<endl;
    //go to the right subtree
    long newSupply = forwardparse(node->right, supply);
    return newSupply;
}

long MapTable::reverseparse(MapTable::Node *node, long oldDemand) {
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
    cout<<node->data->price<<" | "<<node->data->buyQty<<" | "<<node->data->sellQty<<" | "<<node->data->demandQty<<" | "<<node->data->supplyQty<<endl;
    //go to the right subtree
    long newDemand = reverseparse(node->left, demand);
    return newDemand;

}
void MapTable::calculateEQprice() {
    // Call Inorder traversal of the AVL tree which also calculates the cumulative supply at each price point
    long totalSupply = forwardparse(treeRoot, 0);
    cout<<"The cumulative supply is: "<<totalSupply<<endl;
    // Call reverse Inorder traversal of the AVL tree and also calculate the cumulative demand and max tradable qty and equilibrium price
    long totalDemand = reverseparse(treeRoot,0);
    cout<<"The cumulative demand is: "<<totalDemand<<endl;
    //calculate equilibrium price:
    if(equilibriumRows.size() == 1) {
        equilibriumPrice = equilibriumRows.back().first;
    }
    else {
        vector<pair<float,long> >::iterator itr;
        long unmatchedQty = abs(equilibriumRows.front().second);
        for(itr = equilibriumRows.begin(); itr != equilibriumRows.end(); itr++) {
            if(abs(itr->second)<unmatchedQty) {
                unmatchedQty = abs(itr->second);
                equilibriumPrice = itr->first;
            }
        }
    }
    cout<<"[MAP]The equilibrium price is: "<<equilibriumPrice<<endl;

}

void MapTable::matchPreOpen(BuyOrderBook *pendingBuy, SellOrderBook *pendingSell) {

}
