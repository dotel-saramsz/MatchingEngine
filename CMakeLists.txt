cmake_minimum_required(VERSION 3.13)
project(Ordermatching)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ordermatching main.cpp OrderBook.cpp OrderBook.h OrderTable.cpp OrderTable.h AVLTable.cpp AVLTable.h MapTable.cpp MapTable.h TableRow.cpp TableRow.h OrderPoint.cpp OrderPoint.h FileReader.cpp FileReader.h OrderContainer.h OrderContainer.cpp BuyOrderBook.cpp BuyOrderBook.h SellOrderBook.cpp SellOrderBook.h Transaction.cpp Transaction.h TransactionLog.cpp TransactionLog.h STLmapTable.cpp STLmapTable.h)