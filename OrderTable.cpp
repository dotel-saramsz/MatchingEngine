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
    //can't think of anything to put in here as of now
}

void OrderTable::calculateEQprice() {
    // Call forward traversal of the order table which also calculates the cumulative supply at each price point
    long totalSupply = forwardparse();
    cout<<"The cumulative supply is: "<<totalSupply<<endl;
    // Call reverse traversal of the order table and also calculate the cumulative demand and max tradable qty and equilibrium price
    long totalDemand = reverseparse();
    cout<<"The cumulative demand is: "<<totalDemand<<endl;
    //calculate equilibrium price:
    if(equilibriumRows.size() == 1) {
        equilibriumPrice = equilibriumRows.back().first;
        unmatchedQty = equilibriumRows.back().second;
    }
    else {
        vector<pair<float,long> >::iterator itr;
        unmatchedQty = equilibriumRows.front().second;
        equilibriumPrice = equilibriumRows.front().first;
        for(itr = equilibriumRows.begin(); itr != equilibriumRows.end(); itr++) {
            if(abs(itr->second) < abs(unmatchedQty)) {
                unmatchedQty = itr->second;
                equilibriumPrice = itr->first;
            }
        }
        //TODO- If after completing this loop, the equilibrium price is still 0, then case 3 or 4
    }
    cout<<"The equilibrium price is: "<<equilibriumPrice<<endl;
}

void OrderTable::matchPreOpen() {
    pendingBuy = new BuyOrderBook;
    pendingSell = new SellOrderBook;

    //inorder traversal through the tree to separate the orders into the 4 categories:
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
