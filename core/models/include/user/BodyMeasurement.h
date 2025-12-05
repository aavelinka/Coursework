#ifndef BODYMEASUREMENT_H
#define BODYMEASUREMENT_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>

using namespace std;

struct Date 
{
    int day, month, year;
    
    string toString() const;
    static Date fromString(const string& dateStr);

    friend ostream& operator<<(ostream& os, const Date& date);
    friend istream& operator>>(istream& is, Date& date);
    bool operator>(const Date& a) 
    {
        return tie(this->year, this->month, this->day) > tie(a.year, a.month, a.day);
    }
};

struct Measurement
{
    double weight;
    double height;
    double chest;
    double waist;
    double hips;
    Date date;
};

ostream& operator<<(ostream& os, const Measurement& m);
istream& operator>>(istream& is, Measurement& m);

class BodyMeasurement
{
private:
    Measurement currentMeasurement;
    
public:
    BodyMeasurement() = default;
    
    Measurement getCurrentMeasurement() const;
    void setCurrentMeasurement(const Measurement& measurement);
    bool hasMeasurement() const; 
};

#endif