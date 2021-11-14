#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
    //key = features names
    map<string, vector<float>> data_table;
    // features name vec
    vector<string> features;
public:
    //construct
    TimeSeries(const char *CSVfileName);
    //checks if file cant be opened
    void readCSVFile(const char *CSVfileName);

    //get data from map
    map<string, vector<float>> getDataTable() const;

    //get values vector according to feature name
    vector<float> getValues(string feature) const;

    // gets vector of words without commas
    vector<string> splitLine(const string& line, char comma);

    //add values to map
    void setMap(vector<string> values);
};

#endif /* TIMESERIES_H_ */
