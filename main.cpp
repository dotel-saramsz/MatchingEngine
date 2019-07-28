#include <iostream>
#include <algorithm>
#include <vector>

#include "OrderPoint.h"
#include "OrderTable.h"
#include "AVLTable.h"
#include "MapTable.h"
#include "STLmapTable.h"
#include "FileReader.h"
#include "OrderContainer.h"
#include "BuyOrderBook.h"
#include "SellOrderBook.h"
#include <string>
#include <cstring>
#include <chrono>
#include <unistd.h>

long matchStartTime;

int main(int argc, char** argv) {
    long beginTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
    OrderTable* orderTable;
    cout<<argv[1]<<" Implementation"<<endl;
    if(strcmp(argv[1],"AVL") == 0 || strcmp(argv[1],"avl") == 0){
        orderTable = new AVLTable;
    }
    else if(strcmp(argv[1],"RBT") == 0 || strcmp(argv[1],"rbt") == 0){
        orderTable = new MapTable;
    }
    else {
        orderTable = new STLmapTable;
    }
    string filepath = string("../orders/") + string(argv[2]);

    FileReader orderFile = FileReader(filepath);
    orderFile.fillOrderTable(orderTable);
    matchStartTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();

    //compute the cumulative supply in a forward parse through the sorted table
    //compute the cumulative demand in a backward parse through the sorted table
    orderTable->calculateEQprice();

    //call the preorder matching on the orderTable. Also send the two orderbooks to be filled with pending orders as this operation takes place
    orderTable->matchPreOpen();

    //display the pending buy and sell orders
    long endTime = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
//    cout<<"Time taken for matching: "<<(endTime-matchStartTime)<<" nanoseconds"<<endl;
    cout<<"Total time taken: "<< static_cast<double>(endTime-beginTime)/1000000<<" milliseconds"<<endl;
    return 0;
}