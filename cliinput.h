#ifndef MATCHINGENGINE_CLIINPUT_H
#define MATCHINGENGINE_CLIINPUT_H

#include "header.h"
#include "orderparser.h"

class cliInput{
public:
    void runCLI(){

        string order_input;

        cout<<"--------WELCOME TO MATCHING ENGINE----------"<<endl;
        cout<<"Enter your order:"<<endl;
        while(getline(cin,order_input))
        {

            orderParser newOrder;
            newOrder.parseInput(order_input);

            switch(newOrder.getOrderType())
            {
                case ORDER_BUY:
                    cout<<"It's a buy order"<<endl;
                    //do other stuff
                    break;
                case ORDER_SELL:
                    cout<<"It's a sell order"<<endl;
                    //do other stuff
                    break;
                default:
                    break;

            }
            cout<<"Enter your order:"<<endl;
        }
    }
};
#endif //MATCHINGENGINE_CLIINPUT_H
