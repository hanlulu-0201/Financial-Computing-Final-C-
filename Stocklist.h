#pragma once
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <cstdlib>      // std::rand, std::srand
#include <stdio.h>
#include <string>
#include <fstream>
#include <cmath>
#include <map>
#include "readfiletools.h"
#include "Stock.h"
#include"bootstrap_tools.h"
using namespace std;

/*
The data structure for group name is [[Ticker from group Beat],[Ticker from group Meet],[Ticker from group Miss]]
*/


void bootstrap_tools::CreateStocklist(const string& path, StockMap& StockList, vector<string>& TickerList, vector<string>& Tradedate)
{
    ifstream file;
    file.open(path);

    vector<string> id = read_col<vector<string>, string>(file, 0);
    vector<string> date = read_col<vector<string>, string>(file, 1);
    vector<double> actual = read_col<vector<double>, double>(file, 2);
    vector<double> est = read_col<vector<double>, double>(file, 3);
    vector<double> surprise = read_col<vector<double>, double>(file, 4);
    vector<string> group = read_col<vector<string>, string>(file, 5);

    file.close();

    TickerList = id;

    int count = 0;
    int n = id.size();
    int date_size = Tradedate.size();
    int dateindex = 60;
    for (int i = 0; i < n; ++i)
    {
        Stock temstock;
        temstock.Set_Ticker(id[i]);
        temstock.Set_AnnounceDate(date[i]);
        temstock.Set_EstEPS(est[i]);
        temstock.Set_ActEPS(actual[i]);
        temstock.Set_Surprise(surprise[i]);
        temstock.Set_Group(group[i]);

        //the maximum for N is 60
        for (int j = 60; j < date_size - 10; j++)
        {
            if (Tradedate[j] == date[i])
            {
                dateindex = j;
                j = date_size;
            }
            //cout << "Tradedate[j]" << Tradedate[j] << endl;
            //cout << "date[i]" << date[i] << endl;
        }
        temstock.Set_StartDateIndex(dateindex - N);
        temstock.Set_StartDate(Tradedate[dateindex - N]);
        temstock.Set_EndDate(Tradedate[dateindex + N]);
        StockList[id[i]] = temstock;
        count += 1;
    }
    cout << count << " stocks is in stocklist \n" << endl;

}

void bootstrap_tools::CheckSize(StockMap& StockList)
{
    for (auto itr = StockList.begin(); itr != StockList.end(); )
    {
        vector<double> stockPrice = itr->second.Get_AdjClose();
        string stockName = itr->first;
        if (stockPrice.size() != 2 * N + 1)
        {
            cout << stockName << " adj size not 2n+1" << endl;
            itr++;
            StockList.erase(stockName);
        }
        else
        {
            itr++;
        };
    }
    cout << " all the size of adj close price has been checked\n" << endl;
}

GroupOfName divide_group(StockMap& StockList)
{
    GroupOfName stock_groups(3);
    for (auto itr = StockList.begin(); itr != StockList.end(); itr++)
    {
        if (itr->second.Get_Group() == "Miss")
        {
            stock_groups[0].push_back(itr->second.Get_Ticker());
        }
        else if (itr->second.Get_Group() == "Meet")
        {
            stock_groups[1].push_back(itr->second.Get_Ticker());
        }
        else {
            stock_groups[2].push_back(itr->second.Get_Ticker());
        }
    }
    cout << "Stocks have been seperated to different groups\n" << endl;
    return stock_groups;
}

GroupOfName SelectRandom(GroupOfName& stock_groups)
{
    GroupOfName RandomSet(3);
    random_shuffle(stock_groups[0].begin(), stock_groups[0].end());
    random_shuffle(stock_groups[1].begin(), stock_groups[1].end());
    random_shuffle(stock_groups[2].begin(), stock_groups[2].end());
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < random; j++) {
            RandomSet[i].push_back(stock_groups[i][j]);
        }
    }
    cout << "Stocks have been randomly selected to a new groups" << endl;
    return RandomSet;
}

