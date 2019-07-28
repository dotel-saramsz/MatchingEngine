//
// Created by dotelsaramsha on 5/14/19.
// Singleton Class to log the transactions and handle the log file as well
//

#ifndef ORDERMATCHING_TRANSACTIONLOG_H
#define ORDERMATCHING_TRANSACTIONLOG_H

#include "Transaction.h"
#include <fstream>
#include <string>

class TransactionLog {
private:
    fstream logfile;
public:
    TransactionLog(string filename);
    ~TransactionLog();
    void saveToFile(Transaction*);


};


#endif //ORDERMATCHING_TRANSACTIONLOG_H
