//
// Created by dotelsaramsha on 5/14/19.
//

#include "TransactionLog.h"
#include <iomanip>

TransactionLog* TransactionLog::loggerInstance = nullptr;

TransactionLog::TransactionLog() {
    //open the transaction log file
    logfile.open("transactions.log", ios::out);
    if(!logfile.is_open()) {
        throw "Log file couldn't be opened";
    }
}

TransactionLog *TransactionLog::Instance() {
    if(loggerInstance == nullptr) {
        //the Instance hasn't been created yet
        loggerInstance = new TransactionLog;
    }
    return loggerInstance;
}

void TransactionLog::saveToFile(Transaction* transaction) {
    logfile<<transaction->transID<<" "<<transaction->shareQty<<" "<<transaction->buyOrderID<<" "<<transaction->price<<" "<<transaction->sellOrderID<<" "<<transaction->timestamp<<endl;
}
