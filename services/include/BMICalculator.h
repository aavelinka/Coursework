#ifndef BMICALCULATOR_H
#define BMICALCULATOR_H

#include "user/BodyMeasurement.h"
#include <cmath>

using namespace std;

class BMICalculator
{
public:
    static double calculateBMI(double weight, double height);
    static string classifyUser(double bmi, const string& goal); 
    static string getFitnessLevel(double bmi, const string& goal);
};

#endif