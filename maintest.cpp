#define _CRT_SECURE_NO_DEPRECATE
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdio.h>
#include <cmath>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include "Stock.h"
#include "readfiletools.h"
//#include "ExtractData21.h"
#include"Bootstrapping.h"
#include"ExtractYahoo.h"
#include "Stocklist.h"
#include"PlotChart.h"
#include"bootstrap_tools.h"


/*

================================================================================
Next Step:
1. try to move calculationStockReturn into the final calculation, after I move it to final calculation,
the abnormal and return in stocklist is not set 
2. try to make the user input N, now the N is defined in typedef.h
3. try to customize the menu, make it doesn't looks like steven's code
*/

using namespace std;


int main()
{
    // get sp500
    vector<double> IWB;
    vector<string> DATE;

    cout << "Hi!! We are Russell1000 Tracker, Nice to meet you!" << endl << endl << endl;
    cout << "#########################################################################" << endl;
    cout << "##    ####    ####   #############  ####     ####       ########       ##" << endl;
    cout << "##    ####    ####   #############  ####     ####      ####  ####      ##" << endl;
    cout << "##    ####    ####   ####           ####     ####     ####    ####     ##" << endl;
    cout << "##    ############   #############  ####     ####    ####      ####    ##" << endl;
    cout << "##    ############   #############  ####     ####    ####      ####    ##" << endl;
    cout << "##    ####    ####   ####           ####     ####     ####    ####     ##" << endl;
    cout << "##    ####    ####   #############  #######  #######   ####  ####      ##" << endl;
    cout << "##    ####    ####   #############  #######  #######    ########       ##" << endl;
    cout << "#########################################################################" << endl << endl;

    bootstrap_tools bt;

    cout << "Downloading IWB data..." << endl;
    ExtractIWB2("2020-04-01", "2021-04-20", IWB, DATE);

    srand(unsigned(std::time(0))); 
    StockMap StockList;
    vector<string> TickerList;

    //string path = "Allgroups.csv";
    string path = "TestSet2.csv";
    bt.CreateStocklist(path, StockList, TickerList, DATE);
    ExtractStock2(StockList);
    bt.CheckSize(StockList);
    

    //Now we finished all the scripting, we can start bootstrapping
    //1. calculate Rmt, and Rit, ARit for all
    vector<double> Rmt = findIWBRmt(IWB);
    GroupOfName grouplist = divide_group(StockList);

    AbnormalMap abnormalReturn;
    bt.calculationStockReturn(abnormalReturn, StockList, Rmt);
    vector<vector<vector<double>>> Result(3, vector<vector<double>>(4));
    Result = bt.finalCalculation(grouplist, Rmt, abnormalReturn);


    string input;
    int opt;
    bool cont = true;
    while (cont)
    {
        cout << endl << "Hi!! We are Russell1000 Tracker, Nice to meet you!" << endl;
        cout << endl << "Please input the number 1,2,3,4,5 to continue: " << endl;
        cout << "1: Retrieve historical price of any stocks" << endl;
        cout << "2: Stock Information such as Earning and Earning Surprise" << endl;
        cout << "3: Return Metric for each Earning Surprise group: AAR, AAR-SD, CAAR, CAAR-SD" << endl;
        cout << "4: Plot CAAR of 3 Earning Surprise groups" << endl;
        cout << "5: Exit the Program" << endl;
        cin >> input;

        while ((input.at(0) > '5' || input.at(0) <= '0'))
        {
            cout << "Invalid input! Try Again!" << endl;
            cout << "1: Retrieve historical price of any stocks" << endl;
            cout << "2: Stock Information such as Earning and Earning Surprise" << endl;
            cout << "3: Return Metric for each Earning Surprise group: AAR, AAR-SD, CAAR, CAAR-SD" << endl;
            cout << "4: Plot CAAR of 3 Earning Surprise groups" << endl;
            cout << "5: Exit the Program" << endl;
            cin >> input;
        }

        opt = stoi(input);
        string userticker;
        string selection;
        string input_one = "Y";
        string input_two = "Y";
        string input_three = "Y";
        switch (opt) {
        case 1:
            /*
            cout << "Do you wish to continue?" << endl;
            cout << "if yes type : Y or y" << endl;
            cout << "OtherWise type N or n" << endl;
            cout << "---->";

            cin >> input_one;
            */
            while (input_one == "Y" || input_one == "y") {
                cout << endl << "Instruction !!! " << endl;
                cout << "Input stock ticker such as ---> CHE <--- for historical price" << endl << endl;
                cout << "Input the ticker (This is Case Sensitive) :";
                cin >> userticker;
                cout << endl;
                if (StockList.find(userticker) != StockList.end())
                {
                    int sdate = StockList[userticker].Get_StartDateIndex();
                    cout << "Adjust price of stock " << userticker << endl;
                    for (auto price : StockList[userticker].Get_AdjClose()) {
                        cout << DATE[sdate++] << " " << price << endl;
                    }

                }

                else if (userticker == "IWB")
                {
                    int sdate = 0;
                    cout << "Adjust price of stock " << userticker << endl;
                    for (auto itr = IWB.begin(); itr != IWB.end(); itr++) {
                        cout << DATE[sdate++] << " " << *itr << endl;
                    }
                }
                else {
                    cout << "No Data!!! for this input ticker" << endl;
                }

                cout << endl << endl << "Do you wish to Stay for another stock price?" << endl;
                cout << "if yes type : Y or y" << endl;
                cout << "OtherWise type N or n" << endl;
                cout << "---->";
                cin >> input_one;
            }
            break;


        case 2:
            /*
            cout << "Do you wish to continue?" << endl;
            cout << "if yes type : Y or y" << endl;
            cout << "OtherWise type N or n" << endl;
            cout << "----> ";
            cin >> input_two;
            */
            while (input_two == "Y" || input_two == "y") {
                cout << endl << "Instruction !!! " << endl;
                cout << "Input stock ticker such as ---> CHE <--- for stock information such as EPS" << endl << endl;
                cout << "Input the ticker (This is Case Sensitive) :";
                cin >> userticker;
                cout << endl;
                if (StockList.count(userticker)) {
                    //Daily prices -- Vector checked!
                    //Cumulative daily return -- Vector checked!
                    //The group the stock belongs to
                    //Earning Announcement Date, Period ending, Estimated,Reported Earnings, Surprise and %Surprise
                    //cout << "Ticker:             " << StockList[userticker].Get_Ticker() << endl;
                    //cout << "Earnings Call Date: " << StockList[userticker].Get_AnnounceDate() << endl;
                    //cout << "Actual EPS:         " << StockList[userticker].Get_ActEPS() << endl;
                    //cout << "Estimated EPS:      " << StockList[userticker].Get_EstEPS() << endl;
                    //cout << "Surprise:           " << StockList[userticker].Get_Surprise() << endl;
                    //cout << "Group:              " << StockList[userticker].Get_Group() << endl;
                    //======================================================================================================
                    cout << StockList[userticker] << endl;
                    vector<double> cummulative((2 * bt.get_N()) + 1, 0.0);
                    cout << "Day  ||  Daily Return(%)  ||  Cumulative Return(%)" << endl;
                    for (int i = 1; i <= (2 * bt.get_N()); i++) // HAVE CHANGE
                    {
                        double rett = (StockList[userticker].Get_AdjClose()[i] / StockList[userticker].Get_AdjClose()[i - 1] - 1);
                        cummulative[i] = rett + cummulative[i - 1];
                        cout << (i - 2 * bt.get_N() / 2) << "          " << setprecision(6) << rett << "          " << cummulative[i] << endl;
                    }
                    cout << endl;
                }
                else {
                    cout << "No Data!!! for this input ticker" << endl;
                }

                cout << endl << endl << "Do you wish to Stay for another stock information?" << endl;
                cout << "if yes type : Y or y" << endl;
                cout << "OtherWise type N or n" << endl;
                cout << "----> ";
                cin >> input_two;


            }
            break;

        case 3:
            /*
            cout << "Do you wish to continue?" << endl;
            cout << "if yes type : y" << endl;
            cout << "OtherWise type : n" << endl;
            cout << "----> ";
            cin >> input_three;
            */
            while (input_three == "Y" || input_three == "y") {
                cout << endl << "Input number of the group below : " << endl;
                cout << " 0 |for miss group" << endl;
                cout << " 1 |for beat group" << endl;
                cout << " 2 |for meet group" << endl;
                cout << "---> ";


                int groupselect;
                cin >> groupselect;
                //printstats(groupselect);
                if (groupselect != 1 && groupselect != 2 && groupselect != 0) {
                    cout << "Invalid number!" << endl;
                    break;
                }
                //cout << "GroupSelect    |   AAR     |    AAR-SD     |    CAAR    |    CAAR-SD " << endl;
                //cout << groupselect << setprecision(6) << "       " << Result[groupselect][0][0] << "       " << Result[groupselect][1][0] << "       " << Result[groupselect][2][0] << "       " << Result[groupselect][3][0] << endl;

                cout << "Day    |   AAR     |    AAR-SD     |    CAAR    |    CAAR-SD " << endl;
                for (int i = 0; i < 2 * bt.get_N(); i++) {
                    cout << setprecision(0) << i - bt.get_N() << "    ";
                    cout << setprecision(6) << Result[groupselect][0][i] << "    " << Result[groupselect][1][i] << "    " << Result[groupselect][2][i] << "    " << Result[groupselect][3][i] << endl; //  DEADHERE
                }

                cout << endl << endl << "Do you wish to Stay for another stock information?" << endl;
                cout << "if yes type : Y or y" << endl;
                cout << "OtherWise type N or n" << endl;
                cout << "----> ";
                cin >> input_three;


            }
            break;

        case 4:
            //call gnuplot and plot.

            plotChart(Result[2][2], Result[1][2], Result[0][2]); 
            cout << "Plot, done!" << endl;
            break;

        case 5:
            cout << "Confirmed the Exit press Y" << endl;
            cin >> selection;
            if (selection == "Y" || selection == "y")
            {
                cout << "THANK YOU!! Have a Great Day!!" << endl << endl << endl;
                cout << "################################################################################################" << endl;
                cout << "##    ############   ####    ####       #####      ####     ####  ####    ####  ############  ##" << endl;
                cout << "##    ############   ####    ####      #######     #####    ####  ####   ####   ############  ##" << endl;
                cout << "##        ####       ####    ####     #### ####    ######   ####  ####  ####    ####          ##" << endl;
                cout << "##        ####       ############    ####   ####   #######  ####  #########     ############  ##" << endl;
                cout << "##        ####       ############   #############  ####  #######  #########     ############  ##" << endl;
                cout << "##        ####       ####    ####   ####     ####  ####   ######  ####  ####            ####  ##" << endl;
                cout << "##        ####       ####    ####   ####     ####  ####    #####  ####   ####   ############  ##" << endl;
                cout << "##        ####       ####    ####   ####     ####  ####     ####  ####    ####  ############  ##" << endl;
                cout << "################################################################################################" << endl;
                return 0;
            }
            else
            {
                cout << "No Exit, Continue the application" << endl;
                break;
            }
            break;

        default:
            break;
        };
    };

    return 0;

}