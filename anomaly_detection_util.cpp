//
// Eden Meidan 207481177
// Racheli Weinberger 208812628
//
#include <cmath>
#include "anomaly_detection_util.h"

float mean(float* x, int size){
    float mean = 0;
    for(int i=0; i < size; i++) {
        mean += x[i];
    }
    return mean/(float)size;
}
// returns the variance of X and Y
float var(float* x, int size){
    float sum = 0;
    //sum of x values
    for(int i=0; i < size; i++){
        sum += pow(x[i],2.0);
    }
    float var = 1/(float)size *sum - pow(mean(x,size),2.0);
    return var;
}
// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float sum = 0;
    for (int i=0; i<size; i++) {
        sum += (x[i]-mean(x,size))*(y[i]-mean(y,size));
    }
    return sum / (float) size;
}
// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
   return (cov(x, y, size))/(sqrt(var(x,size))* sqrt(var(y,size)));
}
// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size) {
    //create two arrays for x and y
    float x[size];
    float y[size];
    //fill both arrays
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x; //take x val of point and save into array
        y[i] = points[i]->y;//take y val of point and save into array
    }
    //create linear line
    float a, b;
    a = cov(x, y, size) / var(x, size);
    b = mean(y, size) - (a * mean(x, size));
    Line line(a, b);
    return line;
}
// returns the deviation between point p and the line
float dev(Point p,Line l) {
    return std::abs(p.y -l.f(p.x));  // |y - f(x)|
}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    Line our_line = linear_reg(points, size);
    float our_dev = dev(p, our_line);
    return our_dev;

}


