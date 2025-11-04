#ifndef BODYMEASUREMENT_H
#define BODYMEASUREMENT_H

#include <string>
#include <vector>
using namespace std;

struct Measurement {
    double weight;
    double height;
    double chest;
    double waist;
    double hips;
    string date;
};

class BodyMeasurement 
{
protected:
    double indexBody;   
    vector<Measurement> measurements;

public:
    BodyMeasurement();
    ~BodyMeasurement();
    
    void addMeasurement(Measurement&);
    Measurement getLatestMeasurement() const;
    void setIndexBody(double);
    double getIndexBody() const;
};

#endif