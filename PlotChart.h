#pragma once

#include <vector>
#include <string>
#include"Bootstrapping.h"
using namespace std;

inline void plotChart(vector<double> beat, vector<double> meet, vector<double> miss) {
    FILE* gnuplotPipe, * tempDataFile;
    size_t dataSize = beat.size() - 1;
    const char* tempDataFileBeat = "Beat";
    const char* tempDataFileMeet = "Meet";
    const char* tempDataFileMiss = "Miss";

    double x1, y1, x2, y2, x3, y3;

    //set gnuplot enviorment
    //gnuplotPipe = _popen("/opt/local/bin/gnuplot", "w");


    gnuplotPipe = _popen("C:\\PROGRA~1\\gnuplot\\bin\\gnuplot.exe", "w");
    

    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileBeat, tempDataFileMeet, tempDataFileMiss);
        fflush(gnuplotPipe);

        //open beat file
        tempDataFile = fopen(tempDataFileBeat, "w");
        for (size_t  i = 0; i <= dataSize; i++) {   // change from int to size_t
            x1 = i; // -10 / 2; //Need to change this to N
            y1 = beat[i];
            fprintf(tempDataFile, "%lf %lf\n", x1 - dataSize / 2, y1);
        }
        fclose(tempDataFile);

        //open meet file
        tempDataFile = fopen(tempDataFileMeet, "w");
        for (size_t i = 0; i <= dataSize; i++) {    // change from int to size_t
            x2 = i; // -10 / 2;
            y2 = meet[i];
            fprintf(tempDataFile, "%lf %lf\n", x2 - dataSize / 2, y2);
        }
        fclose(tempDataFile);

        //open miss file
        tempDataFile = fopen(tempDataFileMiss, "w");
        for (size_t i = 0; i <= dataSize; i++) {  // change from int to size_t
            x3 = i; // -10 / 2;
            y3 = miss[i];
            fprintf(tempDataFile, "%lf %lf\n", x3 - dataSize / 2, y3);
        }
        fclose(tempDataFile);

        printf("press enter to continue...");
        //getchar();
        //remove(tempDataFileBeat);
        //remove(tempDataFileMeet);
        //remove(tempDataFileMiss);

        //fprintf(gnuplotPipe, "exit \n");
    }
    else {
        printf("gnuplot not found...");
    }
}



