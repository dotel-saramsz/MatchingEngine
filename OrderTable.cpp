//
// Created by dotelsaramsha on 4/29/19.
//

#include "OrderTable.h"
#include "Transaction.h"
#include "TransactionLog.h"
#include <iostream>

using namespace std;

extern long matchStartTime;

OrderTable::OrderTable(){
    //base class constructor
    treeRoot = nullptr; //Initially, the treeRoot is NULL
    maxTradableQty = 0;
}

long OrderTable::forwardparse() {
    return forwardparse(treeRoot, 0);
}

long OrderTable::reverseparse() {
    return reverseparse(treeRoot, 0);
}

void OrderTable::categorizeOrder() {
    categorizeOrder(treeRoot);
}

long OrderTable::forwardparse(OrderTable::Node *node, long oldSupply) {
    // inorder traverse (ascending order traverse) the order table and return the cumulative supply to the successor
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

long OrderTable::reverseparse(OrderTable::Node *node, long oldDemand) {
    // reverse inorder traverse (descending order traverse) the order table and return the cumulative demand to the successor
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

void OrderTable::calculateEQprice() {
    // Call forward traversal of the order table which also calculates the cumulative supply at each price point
    long totalSupply = forwardparse();
    cout<<"The cumulative supply is: "<<totalSupply<<endl;
    // Call reverse traversal of the order table and also calculate the cumulative demand and max tradable qty and equilibrium price
    long totalDemand = reverseparse();
    cout<<"The cumulative demand is: "<<totalDemand<<endl;

    //calculate equilibrium price:

    //MATCH CASE 1
    if(equilibriumRows.size() == 1) {
        matchCase = OrderTable::CASE1;

        equilibriumPrice = equilibriumRows.back().first;
        unmatchedQty = equilibriumRows.back().second;
    }
    //MATCH CASE 2
    else {
        matchCase = OrderTable::CASE2;

        vector<pair<float,long> >::iterator itr;
        unmatchedQty = equilibriumRows.front().second;
        equilibriumPrice = equilibriumRows.front().first;
        for(itr = equilibriumRows.begin(); itr != equilibriumRows.end(); itr++) {
            if(abs(itr->second) < abs(unmatchedQty)) {
                unmatchedQty = itr->second;
                equilibriumPrice = itr->first;
                demandGreater = unmatchedQty > 0;
            }
        }
        //TODO- If after completing this loop, the equilibrium price is still 0, then case 3 or 4
    }
    cout<<"The equilibrium price is: "<<equilibriumPrice<<endl;
}

void OrderTable::categorizeOrder(OrderTable::Node *node) {
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
                if(matchCase == OrderTable::CASE1) {
                    eligibleBuy.push_front(order);
                }
                else if(matchCase == OrderTable::CASE2) {
                    if(demandGreater)
                        pendingBuy->push(order);
                    else
                        eligibleBuy.push_front(order);
                }

            }
            else {
                if(matchCase == OrderTable::CASE1) {
                    eligibleSell.push_front(order);
                }
                else if(matchCase == OrderTable::CASE2) {
                    if(demandGreater)
                        eligibleSell.push_front(order);
                    else
                        pendingSell->push(order);
                }
            }
        }

    }
    //traverse to the right child
    categorizeOrder(node->right);
}

void OrderTable::matchPreOpen() {
    pendingBuy = new BuyOrderBook;
    pendingSell = new SellOrderBook;
    OrderPoint *buyOrder, *sellOrder;
    TransactionLog logfile(orderFilename);

    //inorder traversal through the tree to separate the orders into the 4 categories:
    //1.Eligible Buy(Vec) 2.Eligible Sell(Vec) 3.Pending Buy(BuyOrderBook) 4.Pending Sell(SellOrderBook)
    categorizeOrder();

    //match the orders
    if(demandGreater) {
        while(!eligibleBuy.empty() && !eligibleSell.empty()) {
            //match the orders until eligible sell orders get emptied
            buyOrder = eligibleBuy.back();
            sellOrder = eligibleSell.front();
            eligibleBuy.pop_back();
            eligibleSell.pop_front();

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
                eligibleSell.push_front(sellOrder);
            }
            else {
                matchedQty = buyOrder->shareQty; //same as sellOrder->shareQty
            }

            //create a Transaction object
            Transaction* transaction = new Transaction(buyOrder->orderID,sellOrder->orderID,buyOrder->companyID,equilibriumPrice,matchedQty);
            transaction->display();

            long endTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
            cout<<"Time taken for matching: "<<static_cast<double>(endTime-matchStartTime)/1000000<<" milliseconds"<<endl;

            logfile.saveToFile(transaction);
        }

        while(!eligibleBuy.empty()) {
            //some buy orders couldn't be matched. So, add them as pending buy orders
            pendingBuy->push(eligibleBuy.back());
            eligibleBuy.pop_back();
        }

    }
    else {
        while(!eligibleBuy.empty() && !eligibleSell.empty()) {
            //match the orders until eligible buy orders get emptied
            buyOrder = eligibleBuy.front();
            sellOrder = eligibleSell.back();
            eligibleBuy.pop_front();
            eligibleSell.pop_back();

            //handle complete or partial transaction
            long matchedQty;
            if(buyOrder->shareQty > sellOrder->shareQty) {
                //buy order is partially filled
                matchedQty = sellOrder->shareQty;
                buyOrder->shareQty -= matchedQty;
                eligibleBuy.push_front(buyOrder);
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
            Transaction* transaction = new Transaction(buyOrder->orderID,sellOrder->orderID,buyOrder->companyID,equilibriumPrice,matchedQty);
            transaction->display();

            logfile.saveToFile(transaction);
        }

        while(!eligibleSell.empty()) {
            //some sell orders couldn't be matched. So, add them as pending sell orders
            pendingSell->push(eligibleSell.back());
            eligibleSell.pop_back();
        }
    }
}
