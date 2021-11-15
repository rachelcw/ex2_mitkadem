
#include "SimpleAnomalyDetector.h"

//constructor
SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub

}
//destructor
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

bool SimpleAnomalyDetector::isCorrelationTrue(const string &feat1, const string &feat2) {
    //check vec for correlations
    for (auto iter = cf.begin(); iter != cf.end(); ++iter) {
        // make sure we dont detect twice
        if (iter->feature1 == feat2 && iter->feature2 == feat1) {
            return false;
        }
    }
    return true;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    //  i column
    for (auto const &it1: ts.getDataTable()) {
        string feature = it1.first;
        string correlatedFeature;
        //  j column
        float maxCorrelation = 0;
        for (auto const &it2: ts.getDataTable()) {
            if (it1 != it2) {
                // fill x y arrays from timeseries vec
                float x[it1.second.size()];
                float y[it2.second.size()];
                for (int i = 0; i < it1.second.size(); ++i) {
                    x[i] = it1.second[i];
                    y[i] = it2.second[i];
                }
                float correlation = fabs(pearson(x, y, it1.second.size()));
                //if correlation bigger than the last max correlation which checked
                if (correlation > maxCorrelation) {
                    maxCorrelation = correlation;
                    correlatedFeature = it2.first;
                }
            }
        }
        // make sure isCorrelationTrue and that we dont use correlation twice
        if (maxCorrelation > 0.9 && isCorrelationTrue(feature, correlatedFeature)) {
            insertCorrelated(feature, correlatedFeature, maxCorrelation, ts);
        }
    }
}

void SimpleAnomalyDetector::insertCorrelated(const string& feature ,string corFeature, float correlation, const TimeSeries& ts) {
    correlatedFeatures cFeatStruct;
    cFeatStruct.feature1 = feature;
    cFeatStruct.feature2 = corFeature;
    cFeatStruct.corrlation = correlation;
    int size = ts.getValues(feature).size();
    float x[size], y[size];
    for (int i = 0; i < size ; ++i) {
        x[i]=ts.getValues(cFeatStruct.feature1).at(i);
        y[i]=ts.getValues(cFeatStruct.feature2).at(i);
    }

    Point *pointsReg[size]; //array of points obj
    //Point** pointsReg = new Point* [size]; //array of points obj
    //create arrays of x and y values for the linear reg
    for (int j = 0; j < size; j++) {
        pointsReg[j] = new Point(x[j],y[j]);
    }
    //create regression line for the struct
    cFeatStruct.lin_reg = linear_reg(pointsReg, size);
    float deviation, maxDeviation = 0;
    //calc the dis of point
    for (int i = 0; i < size; ++i) {
        deviation = dev(*pointsReg[i], cFeatStruct.lin_reg);
        if (deviation > maxDeviation) {
            maxDeviation = deviation;
        }
    }
    cFeatStruct.threshold = static_cast<float>(1.1 * maxDeviation);
    cf.push_back(cFeatStruct);
    for (int i = 0; i < size; ++i) {
        delete pointsReg[i];
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> vecReport;
    //go through cor pairs
    for(correlatedFeatures cFeatStruct:cf) {
        int size = ts.getValues(cFeatStruct.feature1).size();
        //go through table rows
        for (int row = 0; row < size ; ++row) {
           Point point(ts.getValues(cFeatStruct.feature1).at(row),ts.getValues(cFeatStruct.feature2).at(row));
           float checkDev = dev(point, cFeatStruct.lin_reg);
           // detect if anomaly
           if (checkDev > cFeatStruct.threshold){
               AnomalyReport anomaly(cFeatStruct.feature1 + "-" + cFeatStruct.feature2, row + 1);
               vecReport.push_back(anomaly);
           }
        }
    }
    return vecReport;
}