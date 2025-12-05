#include "user/BodyMeasurement.h"
#include "InputValidator.h"

string Date::toString() const
{
    stringstream ss;
    ss << setfill('0') << setw(4) << year << "-" 
       << setfill('0') << setw(2) << month << "-" 
       << setfill('0') << setw(2) << day;

    return ss.str();
}

Date Date::fromString(const string& dateStr)
{
    Date date = {0, 0, 0};

    date.year = stoi(dateStr.substr(0, 4));
    date.month = stoi(dateStr.substr(5, 2));
    date.day = stoi(dateStr.substr(8, 2));

    return date;
}

ostream& operator<<(ostream& os, const Date& date)
{
    os << date.toString();

    return os;
}

istream& operator>>(istream& is, Date& date)
{
    string dateStr;
    if (getline(is, dateStr, '|'))
    {
        date = Date::fromString(dateStr);
    }
    return is;
}

Measurement BodyMeasurement::getCurrentMeasurement() const
{
    return currentMeasurement;
}

void BodyMeasurement::setCurrentMeasurement(const Measurement& measurement)
{
    currentMeasurement = measurement;
}

bool BodyMeasurement::hasMeasurement() const
{
    return currentMeasurement.date.year != 0; 
}

ostream& operator<<(ostream& os, const Measurement& m)
{
    os << m.weight << "|" 
       << m.height << "|" 
       << m.chest << "|" 
       << m.waist << "|" 
       << m.hips << "|" 
       << m.date.toString() << "\n";
    return os;
}

istream& operator>>(istream& is, Measurement& m)
{
    string line;
    if (getline(is, line))
    {
        if (line.empty())
        {
            is.setstate(ios::failbit);
            return is;
        }
        
        stringstream ss(line);
        string weightStr, heightStr, chestStr, waistStr, hipsStr, dateStr;
        
        if (getline(ss, weightStr, '|') &&
            getline(ss, heightStr, '|') &&
            getline(ss, chestStr, '|') &&
            getline(ss, waistStr, '|') &&
            getline(ss, hipsStr, '|') &&
            getline(ss, dateStr))
        {
            try
            {
                m.weight = stod(weightStr);
                m.height = stod(heightStr);
                m.chest = stod(chestStr);
                m.waist = stod(waistStr);
                m.hips = stod(hipsStr);
                m.date = Date::fromString(dateStr);
            }
            catch (...)
            {
                is.setstate(ios::failbit);
            }
        }
        else
        {
            is.setstate(ios::failbit);
        }
    }

    return is;
}
