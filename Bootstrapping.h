#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Stock.h"
#include "Stocklist.h"
#include"bootstrap_tools.h"

using namespace std;


vector<double> findIWBRmt(vector<double> iwb)
{
    vector<double> IWBRETURN;

    for (int i = 0; i < iwb.size() - 1; i++)
    {
        double temp = (iwb[i + 1] - iwb[i]) / iwb[i];
        IWBRETURN.push_back(temp);
    }
    cout << "Rmt calculation done" << endl;
    return IWBRETURN;
}

void bootstrap_tools::calculationStockReturn(AbnormalMap& abnormalReturn, StockMap& StockList, const vector<double>& Rmt)
{
    for (auto itr = StockList.begin(); itr != StockList.end(); itr++) {
        vector<double> abnormal;
        vector<double> stockPrice = itr->second.Get_AdjClose();
        string stockName = itr->first;
        int startDate = itr->second.Get_StartDateIndex();

        for (int i = 0; i < 2 * N; i++)
        {
            double first = stockPrice[i];
            double second = stockPrice[i + 1];

            double Rit = (second - first) / first;
            itr->second.Set_Return(Rit);

            double ARit = Rit - Rmt[startDate + 1+i];
            itr->second.Set_AbnormalReturn(ARit);
            abnormal.push_back(ARit);
        }
        abnormalReturn.insert({ stockName,abnormal });
    };
    cout << "calculationStockReturn" << endl;
}

// Calculate the AAR based on each day.=========================================================
vector<double> bootstrap_tools::calculationOfAAR(AbnormalMap& abnormalReturn)
{
    vector<double> calculation(2*N, 0.0); //Create a vector of size 2*N with all values as 0.0
    for (auto it = abnormalReturn.begin(); it != abnormalReturn.end(); it++) {
        for (int i = 0; i < 2*N; i++) {
            calculation[i] += it->second[i] / random; //random is M
        }
    }
    return calculation;
}
//=============================================================================

// Using the AAR to calculate the CAAR based on each day.
vector<double> bootstrap_tools::calculationOfCAAR(vector<double> &calculation)
{
    vector<double> calculationCAAR(2 * N, 0.0); //Create a vector of size 2*N with all values as 0.0
    calculationCAAR[0] = calculation[0];

    for (int i = 1; i < calculation.size(); i++) {
        calculationCAAR[i] = calculationCAAR[i - 1] + calculation[i];
    }
    return calculationCAAR;
}

//======operator overloading================================
// The operator is used to calculate adding to vectors.
vector<double> operator +(vector<double>& V1, const vector<double>& V2) {
    for (int i = 0; i < V1.size(); i++) {
        // This is for calculating average AAR and CAAR, you have 2N days of data for each stock, and 50 stocks for each group.
        V1[i] = V1[i] + V2[i] / random; // change to 50
    }
    return V1;
}

//CARE
// The operator used to calculate container( matrix format(repeatx2N))and vector.
vector<double> operator+(vector<vector<double>>& V1, vector<double>& V2) {
    for (int i = 0; i < V2.size(); i++)//2N 
    {
        for (int j = 0; j < V1.size(); j++) //repeat 
        {
            // This is used to calculate the AARSD and CAARSD
            V2[i] = V2[i] + V1[j][i] / V1.size();
        }
    }
    return V2;
}

//CARE

// This operator is used to calcuate the subtraction between container and vector.
vector<vector<double>> operator -(vector<vector<double>>& V1, const vector<double>& V2) {
    for (int i = 0; i < V2.size(); i++) {
        for (int j = 0; j < V1.size(); j++) {
            V1[j][i] = V1[j][i] - V2[i];
        }
    }
    return V1;
}

// This operator is used to do the matrix square.
vector<vector<double>> operator ^(vector<vector<double>>& V1, vector<vector<double>>& V2)
{
    for (int i = 0; i < V1[0].size(); i++) {
        for (int j = 0; j < V1.size(); j++) {
            V1[j][i] = V2[j][i] * V2[j][i];
        }
    }
    return V1;
}

// This is used to calculate the square root of the vector.
vector<double> squareRootOperator(vector<double>& V1)
{
    for (int i = 0; i < V1.size(); i++) {
        V1[i] = sqrt(V1[i]);
    }
    return V1;

}


//double average40BoostrapVector(vector<double>& V1)
//{
//    for (int i = 0; i < V1.size(); i++) {
//        V1[i] += V1[i] / repeat;
//    }
//    return V1[10 - 1];
//}
//
//double std40BoostrapVector(vector<double>& V1, double average)
//{
//    for (int i = 1; i < V1.size(); i++) {
//        V1[i] += (V1[i - 1] - average) * (V1[i - 1] - average) / repeat;
//    }
//    return sqrt(V1[V1.size() - 1]);
//}

//=============================================================================

vector<vector<vector<double>>> bootstrap_tools::finalCalculation(GroupOfName &grouplist, vector<double> &Rmt, AbnormalMap &abnormalReturn)
{
    vector<double> original(2*N, 0.0);
    vector<double> AAR1(2 * N, 0.0);
    vector<double> AARSD1(2 * N, 0.0);
    vector<double> AAR2(2 * N, 0.0);
    vector<double> AARSD2(2 * N, 0.0);
    vector<double> AAR3(2 * N, 0.0);
    vector<double> AARSD3(2 * N, 0.0);
    vector<vector<double>> tempAAR1(repeat, original);
    vector<vector<double>> tempAAR2(repeat, original);
    vector<vector<double>> tempAAR3(repeat, original);
    vector<vector<double>> tempCAAR1(repeat, original);
    vector<vector<double>> tempCAAR2(repeat, original);
    vector<vector<double>> tempCAAR3(repeat, original);
    vector<double> CAAR1(2 * N, 0.0);
    vector<double> CAARSD1(2 * N, 0.0);
    vector<double> CAAR2(2 * N, 0.0);
    vector<double> CAARSD2(2 * N, 0.0);
    vector<double> CAAR3(2 * N, 0.0);
    vector<double> CAARSD3(2 * N, 0.0);


    vector<vector<vector<double>>> Result(3, vector<vector<double>>(4));

    //The calculation of all the abonormal return for all stock.


    // simulate for fourty times.
    for (int j = 0; j < repeat; j++) {
        // shuffle the stocks.

        GroupOfName RandomSet = SelectRandom(grouplist);

        //This will be the abnormal return for miss group.
        AbnormalMap abnormalReturn1;
        //This will be the abnormal return for meet group.
        AbnormalMap abnormalReturn2;
        //This will be the abnormal return for the beat group.
        AbnormalMap abnormalReturn3;

        // We need to divide the stock in the abnormal return map(abnormalReturn) 
        //into three groups based on the group data we have in RandomSet.

        for (int i = 0; i < 3; i++) {
            for (auto it = RandomSet[i].begin(); it != RandomSet[i].end(); it++) //go over each group
            { 
                if (abnormalReturn.find(*it) != abnormalReturn.end()) {
                    string name = *it;
                    if (i == 0) {
                        abnormalReturn1.insert({ name,abnormalReturn[name] });
                    }
                    else if (i == 1) {
                        abnormalReturn2.insert({ name,abnormalReturn[name] });
                    }
                    else {
                        abnormalReturn3.insert({ name,abnormalReturn[name] });
                    }
                }
            }
        }


        //Calculate the average abnormal return of each group.
        vector<double> averageAbnormalReturn1;
        averageAbnormalReturn1 = calculationOfAAR(abnormalReturn1);
        vector<double> averageAbnormalReturn2;
        averageAbnormalReturn2 = calculationOfAAR(abnormalReturn2);
        vector<double> averageAbnormalReturn3;
        averageAbnormalReturn3 = calculationOfAAR(abnormalReturn3);
        //cout << "averageAbnormalReturn3" << endl;



        // This is the container that contains the abnormal return of each stock in every simulation(repeatx2N)
        tempAAR1[j] = averageAbnormalReturn1;
        tempAAR2[j] = averageAbnormalReturn2;
        tempAAR3[j] = averageAbnormalReturn3;


        // These are the calculation of average AAR for three groups(# simulations).
        // 1 value for each, vector<double> AAR1(2 * N, 0.0);
        AAR1 = operator+ (AAR1, averageAbnormalReturn1);
        AAR2 = operator+ (AAR2, averageAbnormalReturn2);
        AAR3 = operator+ (AAR3, averageAbnormalReturn3);
        //cout << "AAR1" << endl;

        //This is the calculation of average CAAR for 3 groups (30 simulation).
        vector<double> culmulativeAbnormalReturn1;
        vector<double> culmulativeAbnormalReturn2;
        vector<double> culmulativeAbnormalReturn3;

        culmulativeAbnormalReturn1 = calculationOfCAAR(averageAbnormalReturn1);
        culmulativeAbnormalReturn2 = calculationOfCAAR(averageAbnormalReturn2);
        culmulativeAbnormalReturn3 = calculationOfCAAR(averageAbnormalReturn3);
        //cout << "culmulativeAbnormalReturn3" << endl;

        //This is the container that contains the CAAR of each stock in every simulation
        tempCAAR1[j] = culmulativeAbnormalReturn1;
        tempCAAR2[j] = culmulativeAbnormalReturn2;
        tempCAAR3[j] = culmulativeAbnormalReturn3;

        //These are the calculation of average CAAR for three groups (# simulations)
        CAAR1 = operator+(CAAR1, culmulativeAbnormalReturn1);
        CAAR2 = operator+(CAAR2, culmulativeAbnormalReturn2);
        CAAR3 = operator+(CAAR3, culmulativeAbnormalReturn3);
       // cout << "CAAR3" << endl;

    }


    //SD formula: sqrt(sum([x_i - average x)^2]/n)

    tempAAR1 = tempAAR1 - AAR1;
    tempAAR1 = tempAAR1 ^ tempAAR1;
    AARSD1 = operator+(tempAAR1, AARSD1);
    AARSD1 = squareRootOperator(AARSD1);
    //cout << "AARSD1" << endl;

    tempAAR2 = tempAAR2 - AAR2;
    tempAAR2 = tempAAR2 ^ tempAAR2;
    AARSD2 = operator+(tempAAR2, AARSD2);
    AARSD2 = squareRootOperator(AARSD2);
    //cout << "AARSD2" << endl;

    tempAAR3 = tempAAR3 - AAR3;
    tempAAR3 = tempAAR3 ^ tempAAR3;
    AARSD3 = operator+(tempAAR3, AARSD3);
    AARSD3 = squareRootOperator(AARSD3);
    //cout << "AARSD3" << endl;

    tempCAAR1 = operator-(tempCAAR1, CAAR1);
    tempCAAR1 = operator ^(tempCAAR1, tempCAAR1);
    CAARSD1 = operator+(tempCAAR1, CAARSD1);
    CAARSD1 = squareRootOperator(CAARSD1);
    //cout << "CAARSD1" << endl;

    tempCAAR2 = operator-(tempCAAR2, CAAR2);
    tempCAAR2 = operator ^(tempCAAR2, tempCAAR2);
    CAARSD2 = operator+(tempCAAR2, CAARSD2);
    CAARSD2 = squareRootOperator(CAARSD2);
    //cout << "CAARSD2" << endl;

    tempCAAR3 = operator-(tempCAAR3, CAAR3);
    tempCAAR3 = operator ^(tempCAAR3, tempCAAR3);
    CAARSD3 = operator+(tempCAAR3, CAARSD3);
    CAARSD3 = squareRootOperator(CAARSD3);
    //cout << "CAARSD3" << endl;



    // Assign the result of 30 simulations for AAR, AARSD, CAAR, CAARSD into the matrix that is created.(The form of the matrix will be 3*4*10).

    Result[0][0] = AAR1;
    Result[0][1] = AARSD1;
    Result[0][2] = CAAR1;
    Result[0][3] = CAARSD1;
    //cout << "Result" << endl;

    Result[1][0] = AAR2;
    Result[1][1] = AARSD2;
    Result[1][2] = CAAR2;
    Result[1][3] = CAARSD2;
    //cout << "Result" << endl;

    Result[2][0] = AAR3;
    Result[2][1] = AARSD3;
    Result[2][2] = CAAR3;
    Result[2][3] = CAARSD3;
    //cout << "Result" << endl;


    return Result;
}
