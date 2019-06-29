//
// Created by dotelsaramsha on 6/17/19.
//
#include <iostream>
#include "STLmapTable.h"
#include "Transaction.h"
#include "TransactionLog.h"

using namespace std;

extern long matchStartTime;

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

void STLmapTable::remove(OrderPoint *) {

}

void STLmapTable::lookup(OrderPoint *) {

}

void STLmapTable::calculateEQprice() {
    long supply=0,demand=0;
    TableRow* data;

    for(auto it : Node) {
        data = it.second;
        supply += data->sellQty;
        data->supplyQty = supply;
    }
    for(auto it=Node.rbegin();it!=Node.rend();it++) {
        data = it->second;
        demand += data->buyQty;
        data->demandQty = demand;
        data->tradableQty = min(data->demandQty,data->sellQty);
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
    }
    //Print the cumulative sell and buy as supply and demand
    cout<<"The cumulative supply is: "<<supply<<endl;
    cout<<"The cumulative demand is: "<<demand<<endl;
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

int STLmapTable::getHeight() {
    return 0;
}

STLmapTable::STLmapTable() {
    maxTradableQty = 0;
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
                    eligibleSell.push_back(order);
                }
            }
        }
        else if(data->price > equilibriumPrice) {
            //add to category A or D (Eligible Buy or Pending Sell)
            for (auto order:data->orders) {
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
            for (auto order:data->orders) {
                if(order->type == OrderPoint::BUY) {
                    eligibleBuy.push_back(order);
                }
                else {
                    eligibleSell.push_back(order);
                }
            }
        }

    }
}

void STLmapTable::matchPreOpen(BuyOrderBook *pendingB, SellOrderBook *pendingS) {
    pendingBuy = pendingB;
    pendingSell = pendingS;
    //separate the orders into the 4 categories:
    //1.Eligible Buy(Vec) 2.Eligible Sell(Vec) 3.Pending Buy(BuyOrderBook) 4.Pending Sell(SellOrderBook)
    categorizeOrder();
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
        Transaction* transaction = new Transaction(buyOrder->orderID,sellOrder->orderID,buyOrder->companyID,equilibriumPrice,matchedQty);
        transaction->display();

        long endTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
        cout<<"Time taken for matching: "<<static_cast<double>(endTime-matchStartTime)/1000000<<" milliseconds"<<endl;

        TransactionLog::Instance()->saveToFile(transaction);
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
