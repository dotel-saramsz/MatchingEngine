#include "orderparser.h"


void orderParser::validateInput(vector<string>results)
{
    //validate order type
    //validate company
    //validate qty
    //validate price

    //use REG EX for validating user inputs
}

void orderParser::generateOrderID()
{
    /*
        For real time distributed applications UUID1 can be used (See RFC4122)

        Here only the timestamp value is used for generating unique ID
        [time in milliseconds since 1970]
    */
    ORDER_ID = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    std::cout << ORDER_ID << endl;

}

void orderParser::parseInput(string input) {
    //Split the input string and initialize the object variables
    input=input+' ';
    vector<string>results;
    int length=input.size();
    string word="";             //empty string
    for(int i=0;i<=length;i++)
    {
        if(input[i]!=' ')
        {
            word+=input[i];
        }
        else
        {
            //add this to results vector
            results.push_back(word);
            //reset word
            word="";
        }

    }

    //TODO :: validateInput(results)

    //Initialize Object Variables
    transform(results[0].begin(), results[0].end(), results[0].begin(), ::toupper);
    ORDER_TYPE=results[0];
    transform(results[1].begin(), results[1].end(), results[1].begin(), ::toupper);
    CMP_NAME=results[1];
    QTY=stoi(results[2]);    //string to integer
    PRICE=stof(results[3]);  //string to float


    //generate a unique orderID here
    generateOrderID();


}

enOrderType orderParser::getOrderType() {
    if (!ORDER_TYPE.compare("BUY"))  //str1.compare(str2) : returns 0 when both strings match
    {
        return ORDER_BUY;
    }
    else if (!ORDER_TYPE.compare("SELL"))
    {
        return ORDER_SELL;
    }
}

