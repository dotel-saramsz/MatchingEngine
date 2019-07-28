//
// Created by dotelsaramsha on 6/17/19.
//
#include <iostream>
#include "STLmapTable.h"
#include "Transaction.h"
#include "TransactionLog.h"

using namespace std;

void STLmapTable::insert(OrderPoint *data) {
    //Check if a row indexed at the data->price is already created
    auto it = Node.find(data->price);
    if(it == Node.end()) {
        //No table row at that price is not found. So, insert it directly
        TableRow* row = new TableRow(data->price,data,data->shareQty*(1-data->type),data->shareQty*data->type);
        Node.insert(pair<float,TableRow*>(data->price, row));
    }
    else {
        //A table row at this price is found. So, chain it
        TableRow* row = it->second;
        row->addOrder(data);
        row->buyQty += data->shareQty * (1-data->type);
        row->sellQty += data->shareQty * data->type;
    }
}

STLmapTable::STLmapTable() {

}

void STLmapTable::categorizeOrder() {
    for(auto it : Node) {
        auto data = it.second;
        //Process the current node. Each node can have multiple orders.
        //Check if the node is eq node or not and categorize accordingly
        if(data->price < equilibriumPrice) {
            //add to category B or C (Eligible sell or Pending Buy)
            for (auto order:data->orders) {   //new range based for-loop in C++
                if(order->type == OrderPoint::BUY) {
                    pendingBuy->push(order);
                }
                else {
                    eligibleSell.push_front(order);
                }
            }
        }
        else if(data->price > equilibriumPrice) {
            //add to category A or D (Eligible Buy or Pending Sell)
            for (auto order:data->orders) {
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
            for (auto order:data->orders) {
                if(order->type == OrderPoint::BUY) {
                    eligibleBuy.push_front(order);
                }
                else {
                    eligibleSell.push_front(order);
                }
            }
        }

    }
}

long STLmapTable::forwardparse() {
    long supply = 0;
    TableRow* data;

    for(auto it : Node) {
        data = it.second;
        supply += data->sellQty;
        data->supplyQty = supply;
    }

    return supply;
}

long STLmapTable::reverseparse() {
    long demand = 0;
    TableRow* data;
    for(auto it=Node.rbegin();it!=Node.rend();it++) {
        data = it->second;
        demand += data->buyQty;
        data->demandQty = demand;
        data->tradableQty = min(data->demandQty,data->supplyQty);
        data->unmatchedQty = data->demandQty-data->supplyQty;
        //cases for setting the equilibrium price
        //case 1
        if (data->tradableQty > maxTradableQty) {
            maxTradableQty = data->tradableQty;
            equilibriumRows.clear();
            equilibriumRows.push_back(make_pair(data->price,data->unmatchedQty));
        }
            //case 2
        else if (data->tradableQty == maxTradableQty) {
            equilibriumRows.push_back(make_pair(data->price,data->unmatchedQty));
        }
        cout<<data->price<<" | "<<data->buyQty<<" | "<<data->sellQty<<" | "<<data->demandQty<<" | "<<data->supplyQty<<" | "<<data->tradableQty<<" | "<<data->unmatchedQty<<endl;
    }

    return demand;
}

