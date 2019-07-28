//
// Created by dotelsaramsha on 5/14/19.
//

#include "TransactionLog.h"
#include <iomanip>

TransactionLog::TransactionLog(string filename) {
    //open the transaction log file
    string filepath = string("../transactions/") + filename;
    logfile.open(filepath, ios::out);
    if(!logfile.is_open()) {
        throw "Transaction Log file couldn't be opened";
    }
}

void TransactionLog::saveToFile(Transaction* transaction) {
    logfile<<transaction->transID<<" "<<transaction->shareQty<<" "<<transaction->buyOrderID<<" "<<transaction->price<<" "<<transaction->sellOrderID<<" "<<transaction->timestamp<<endl;
}

TransactionLog::~TransactionLog() {
    logfile.close();
}
