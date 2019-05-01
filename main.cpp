#include <iostream>
#include <algorithm>
#include <vector>

#include "OrderPoint.h"
#include "OrderTable.h"
#include "AVLTable.h"
#include "MapTable.h"
#include "FileReader.h"

#include <string>
#include <cstring>

int main(int argc, char** argv) {
    OrderTable* orderTable;
    cout<<argv[1]<<" Implementation"<<endl;
    if(strcmp(argv[1],"AVL") == 0 || strcmp(argv[1],"avl") == 0){
        orderTable = new AVLTable;
    }
    else {
        orderTable = new MapTable;
    }

    FileReader orderFile = FileReader("/media/windows2/LIS Internship/Ordermatching/orders.txt");
    orderFile.fillOrderTable(orderTable);

    //compute the cumulative supply in a forward parse through the sorted table
    //compute the cumulative demand in a backward parse through the sorted table
    orderTable->calculateEQprice();

    //separate the orders into different categories


    return 0;
}