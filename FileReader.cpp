//
// Created by dotelsaramsha on 5/1/19.
//

#include "FileReader.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

FileReader::FileReader(string filename) {
    csvFile.open(filename, ios::in);
    if(!csvFile.is_open()) {
        throw "File Opening Error";
    }
}

void FileReader::fillOrderTable(OrderTable *orderTable) {
    OrderPoint::orderType type;
    int itype;
    long shareQty;
    float price;
    string companyID;
    long clientID;

    while(csvFile>>itype>>ws>>companyID>>ws>>price>>ws>>shareQty>>ws>>clientID){
        //Create orderPoints and insert into the orderTable
        type = static_cast<OrderPoint::orderType>(itype);
        OrderPoint* order = new OrderPoint(type,companyID,price,shareQty,clientID);
        orderTable->insert(order);
    }

    csvFile.close();
}
