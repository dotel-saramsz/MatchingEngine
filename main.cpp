#include <iostream>
#include <algorithm>
#include <vector>

#include "OrderPoint.h"
#include "OrderTable.h"
#include "AVLTable.h"
#include "MapTable.h"
#include "FileReader.h"
#include "OrderContainer.h"
#include "BuyOrderBook.h"
#include "SellOrderBook.h"
#include <string>
#include <cstring>
#include <chrono>
#include <unistd.h>


int main(int argc, char** argv) {
    long beginTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
    OrderTable* orderTable;
    cout<<argv[1]<<" Implementation"<<endl;
    if(strcmp(argv[1],"AVL") == 0 || strcmp(argv[1],"avl") == 0){
        orderTable = new AVLTable;
    }
    else {
        orderTable = new MapTable;
    }

    FileReader orderFile = FileReader("orders.txt");
    orderFile.fillOrderTable(orderTable);
    long matchStartTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();

    //compute the cumulative supply in a forward parse through the sorted table
    //compute the cumulative demand in a backward parse through the sorted table
    orderTable->calculateEQprice();

    //declare the Buy and Sell Orderbooks that are to be filled with the pending buy and sell orders
    auto pendingBuy = new BuyOrderBook;
    auto pendingSell = new SellOrderBook;

    //call the preorder matching on the orderTable. Also send the two orderbooks to be filled with pending orders as this operation takes place
    orderTable->matchPreOpen(pendingBuy, pendingSell);

    //display the pending buy and sell orders
    long endTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout<<"Time taken for matching: "<<(endTime-matchStartTime)<<" nanoseconds"<<endl;
    cout<<"Total time taken: "<<(endTime-beginTime)<<" nanoseconds"<<endl;
    return 0;
}