#pragma once
#include <iostream>     // std::cout
#include <stdio.h>
#include <vector>       // std::vector
#include <string>
#include <map>
#include"Stock.h"

using namespace std;


//int N = 20; //total day = 2N+1
int random = 20; //number of stocks selected in each group
int repeat = 40; //repeat calculation # times
typedef vector<vector<string>> GroupOfName;
typedef map<string, vector<double>> AbnormalMap;
typedef map<string, Stock> StockMap;

class bootstrap_tools {
    int N;
public:
    bootstrap_tools() {

        cout << "Please Input N number Between 30 to 60" << endl;
        int N_ = 1;
        bool condition = true;
        while (condition) {

            if (condition) { cout << "Input N number " << endl; }
            cin >> N_;
            if ((N_ >= 30) && (N_ <= 60)) {
                condition = false;
            }

            if (N_ < 30 || N_ > 60) {
                cout << "Invalid N number Should be Between 30 and 60" << endl;
                cout << "Please Try Again\n" << endl;
            }
        }
        N = N_; 
    }
    void calculationStockReturn(AbnormalMap& abnormalReturn, StockMap& StockList, const vector<double> &Rmt);
    vector<double> calculationOfAAR(AbnormalMap& abnormalReturn);
    vector<double> calculationOfCAAR(vector<double>& calculation);
    vector<vector<vector<double>>> finalCalculation(GroupOfName& grouplist, vector<double>& Rmt, AbnormalMap& abnormalReturn);
    void CreateStocklist(const string& path, StockMap& StockList, vector<string>& TickerList, vector<string>& Tradedate);
    void CheckSize(StockMap& StockList);
    int get_N() {
        return N;
    }
};
