//
// Created by dotelsaramsha on 4/29/19.
//

#include "AVLTable.h"
#include <iostream>
#include "Transaction.h"

using namespace std;

void AVLTable::insert(OrderPoint* orderPoint) {
    treeRoot = insert(treeRoot,nullptr,orderPoint);
    cout<<"[AVL] Inserted Order: "<<orderPoint->type<<" | "<<orderPoint->orderID<<" | "<<orderPoint->price<<" | "<<orderPoint->shareQty<<endl;
}

void AVLTable::remove(OrderPoint* orderPoint) {

}

void AVLTable::lookup(OrderPoint* orderPoint) {

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

AVLTable::Node* AVLTable::lookup(TableRow &data) {
    return nullptr;
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
    cout<<node->data->price<<" | "<<node->data->buyQty<<" | "<<node->data->sellQty<<" | "<<node->data->demandQty<<" | "<<node->data->supplyQty<<endl;
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
    cout<<node->data->price<<" | "<<node->data->buyQty<<" | "<<node->data->sellQty<<" | "<<node->data->demandQty<<" | "<<node->data->supplyQty<<endl;
    //go to the right subtree
    long newDemand = reverseparse(node->left, demand);
    return newDemand;
}

void AVLTable::calculateEQprice() {
    // Call Inorder traversal of the AVL tree which also calculates the cumulative supply at each price point
    long totalSupply = forwardparse(treeRoot, 0);
    cout<<"The cumulative supply is: "<<totalSupply<<endl;
    // Call reverse Inorder traversal of the AVL tree and also calculate the cumulative demand and max tradable qty and equilibrium price
    long totalDemand = reverseparse(treeRoot,0);
    cout<<"The cumulative demand is: "<<totalDemand<<endl;
    //calculate equilibrium price:
    if(equilibriumRows.size() == 1) {
        equilibriumPrice = equilibriumRows.back().first;
        unmatchedQty = equilibriumRows.back().second;
    }
    else {
        vector<pair<float,long> >::iterator itr;
        unmatchedQty = equilibriumRows.front().second;
        for(itr = equilibriumRows.begin(); itr != equilibriumRows.end(); itr++) {
            if(abs(itr->second) < abs(unmatchedQty)) {
                unmatchedQty = itr->second;
                equilibriumPrice = itr->first;
            }
        }
    }
    cout<<"The equilibrium price is: "<<equilibriumPrice<<endl;
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
                eligibleSell.push_back(order);
            }
        }
    }
    else if(node->data->price > equilibriumPrice) {
        //add to category A or D (Eligible Buy or Pending Sell)
        for (auto order:node->data->orders) {
            if(order->type == OrderPoint::BUY) {
                eligibleBuy.push_back(order);
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
                eligibleBuy.push_back(order);
            }
            else {
                eligibleSell.push_back(order);
            }
        }
    }
    //traverse to the right child
    categorizeOrder(node->right);
}

void AVLTable::matchPreOpen(BuyOrderBook *pendingB, SellOrderBook *pendingS) {
    pendingBuy = pendingB;
    pendingSell = pendingS;
    //inorder traversal through the tree to separate the orders into the 4 categories:
    //1.Eligible Buy(Vec) 2.Eligible Sell(Vec) 3.Pending Buy(BuyOrderBook) 4.Pending Sell(SellOrderBook)
    categorizeOrder(treeRoot);
    //match the orders
    while(!eligibleBuy.empty() && !eligibleSell.empty()) {
        //match the orders until either of the containers get emptied
        OrderPoint* buyOrder = eligibleBuy.back();
        OrderPoint* sellOrder = eligibleSell.back();
        eligibleBuy.pop_back();
        eligibleSell.pop_back();
        //handle complete or partial transaction
        long matchedQty;
        if(buyOrder->shareQty > sellOrder->shareQty) {
            //buy order is partially filled
            matchedQty = sellOrder->shareQty;
            buyOrder->shareQty -= matchedQty;
            eligibleBuy.push_back(buyOrder);
        }
        else if(buyOrder->shareQty < sellOrder->shareQty) {
            //sell order is partially filled
            matchedQty = buyOrder->shareQty;
            sellOrder->shareQty -= matchedQty;
            eligibleSell.push_back(sellOrder);
        }
        else {
            matchedQty = buyOrder->shareQty; //same as sellOrder->shareQty
        }
        //create a Transaction object
        Transaction transaction(buyOrder->orderID,sellOrder->orderID,buyOrder->companyID,equilibriumPrice,matchedQty);
        transaction.display();
    }

    while(!eligibleBuy.empty()) {
        //some buy orders couldn't be matched. So, add them as pending buy orders
        pendingBuy->push(eligibleBuy.back());
        eligibleBuy.pop_back();
    }

    while(!eligibleSell.empty()) {
        //some sell orders couldn't be matched. So, add them as pending sell orders
        pendingSell->push(eligibleSell.back());
        eligibleSell.pop_back();
    }

}


