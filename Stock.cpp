#include <stdio.h>
#include <vector>
#include <iostream>
#include "Stock.h"

void Stock::Set_AdjClose(vector<double> AdjClose_){
    AdjClose = AdjClose_;
}

void Stock::Set_Return(double ret){
    Return.push_back(ret);
}

void Stock::Set_AbnormalReturn(double abr){
    AbnormalReturn.push_back(abr);
}

void Stock::Set_Ticker(string t){
    Ticker = t;
}

void Stock::Set_EstEPS(double eeps){
    EstEPS = eeps;
}

void Stock::Set_ActEPS(double aeps){
    ActEPS = aeps;
}

void Stock::Set_Group(string group){
    Group = group;
}

void Stock::Set_AnnounceDate(string date){
    AnnounceDate = date;
}

void Stock::Set_Surprise(double surprise){
    Surprise = surprise;
}

void Stock::Set_StartDateIndex(int sdi){
    StartDateIndex = sdi;
}

void Stock::Set_StartDate(string sd){
    StartDate = sd;
}

void Stock::Set_EndDate(string ed){
    EndDate = ed;
}

vector<double> Stock::Get_AdjClose(){
    return AdjClose;
}


vector<double> Stock::Get_Return(){
    return Return;
}

vector<double> Stock::Get_AbnormalReturn(){
    return AbnormalReturn;
}

int Stock::Get_StartDateIndex(){
    return StartDateIndex;
}

string Stock::Get_StartDate(){
    return StartDate;
}

string Stock::Get_EndDate(){
    return EndDate;
}

string Stock::Get_Ticker(){
    return Ticker;
}

string Stock::Get_Group(){
    return Group;
}

string Stock::Get_AnnounceDate(){
    return AnnounceDate;
}

double Stock::Get_ActEPS(){
    return ActEPS;
}

double Stock::Get_EstEPS(){
    return EstEPS;
}

double Stock::Get_Surprise(){
    return Surprise;
}

void print(std::vector<double> const& a) {
    for (int i = 0; i < a.size(); i++)
    {
        std::cout << a.at(i) << ' ';
    }
    cout<<" \n"<<endl;
}

ostream& operator<<(ostream& os, Stock& stock) {
    os << "StartDateIndex: " << stock.Get_StartDateIndex() << endl;
    os << "Ticker: " << stock.Get_Ticker()<< endl;
    os << "Group: " << stock.Get_Group() <<endl;
    os << "StartDate: " << stock.Get_StartDate() << endl;
    os << "EndDate: " << stock.Get_EndDate() << endl;
    os << "AnnounceDate: " << stock.Get_AnnounceDate() << endl;
    os << "EstEPS: " << stock.Get_EstEPS() << endl;
    os << "ActEPS: " << stock.Get_ActEPS() << endl;
    os << "Surprise: " << stock.Get_Surprise() << endl;
    cout << " \n" << endl;
    os << "AdjClose: " <<endl;
    print(stock.Get_AdjClose());
    os << "Return: " << endl;
    print(stock.Get_Return());
    os << "AbnormalReturn: " << endl;
    print(stock.Get_AbnormalReturn());

    return os;
}
