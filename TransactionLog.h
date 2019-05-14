//
// Created by dotelsaramsha on 5/14/19.
// Singleton Class to log the transactions and handle the log file as well
//

#ifndef ORDERMATCHING_TRANSACTIONLOG_H
#define ORDERMATCHING_TRANSACTIONLOG_H

#include "Transaction.h"
#include <fstream>

class TransactionLog {
private:
    TransactionLog();
    TransactionLog(const TransactionLog&);
    TransactionLog& operator=(const TransactionLog&);
    static TransactionLog* loggerInstance;
    fstream logfile;
public:
    static TransactionLog* Instance();
    void saveToFile(Transaction*);

};


#endif //ORDERMATCHING_TRANSACTIONLOG_H
