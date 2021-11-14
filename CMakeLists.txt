cmake_minimum_required(VERSION 3.17)
project(ex2_mitkadem)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex2_mitkadem
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        AnomalyDetector.h
        MainTrain.cpp
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        timeseries.cpp
        timeseries.h)
