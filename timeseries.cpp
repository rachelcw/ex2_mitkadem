#include "timeseries.h"
//construct

TimeSeries::TimeSeries(const char *CSVfileName) {
    readCSVFile(CSVfileName);
}

void TimeSeries::readCSVFile(const char *CSVfileName) {
    ifstream CSVfile(CSVfileName);
    if (!CSVfile.is_open()) {
        throw runtime_error("couldn't open this file.");
    }
    string line;
    getline(CSVfile, line);  //get first line from file = features
    features = splitLine(line, ','); //creat features vector
    vector<string> vectorValues; // new vec saves ech time one more line
    // convert all lines into feature vectors from text as float.
    while (getline(CSVfile, line)) {
        vectorValues = splitLine(line, ',');
        setMap(vectorValues);
    }
    CSVfile.close();
}
/***
 * this function gets csv line and pushes every value into new vector
 * @param line - from the csv file
 * @param comma - remove comma from the csv line
 * @return vector each csv line as a string
 */
vector<string> TimeSeries::splitLine(const string& line, char comma) {
   vector<string> vector;
    stringstream strStream(line);
    string strValue;
    //go through all csv lines
    while(getline(strStream, strValue, comma)) {
        vector.push_back(strValue);
    }
    return vector;
}

/**
 * convert vector of each csv line as a string to colm vectors.
 * each feature becomes its own cloumn = vector.
 * @param line
 * @param vector
 *
 */
void TimeSeries::setMap(vector<string> vector) {
    // go through each string and fill each feature vec
    for (int i = 0; i < features.size(); ++i) {
        data_table[features[i]].push_back(stof(vector[i])); //push value into vector
    }
}

map<string, vector<float>> TimeSeries::getDataTable() const {
    return data_table;
}

vector<float> TimeSeries::getValues(string feature) const {
    return getDataTable()[feature];
}