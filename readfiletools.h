#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;


//we use this function to read excel file for us 
template <typename T1, typename T2>
T1 read_col(ifstream& file, int data_col){
    file.clear();
    // point read pointer at beginning of file
    file.seekg(0, ios::beg);
    
    string line;
    char *ptr;
    T2 item;
    T1 ans(0);
    //define row and column in every line
    int row = 0;
    int col;
    
    while(getline(file, line)){
        col = 0;
        ptr = strtok((char *)line.c_str(), ",");
        while(ptr){
            if (strcmp(ptr, "")){
                if (col == data_col && row >= 1)
                {
                    stringstream ss;
                    ss << string(ptr);
                    ss >> item;
                    ans.push_back(item);
                }
                ++col;
            }
            ptr = strtok(NULL, ",");
        }
        ++row;
        
    }
    return ans;
     
}
