#pragma once

#include "../../models/include/user/BodyMeasurement.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <locale>

using namespace std;

template <typename T>
T getValidNumericValue(std::istream& stream, T min, T max)
{
    T value;
    string line;
    
    while (true)
    {
        if (!getline(stream, line))
        {
            if (stream.eof())
            {
                throw std::runtime_error("Конец потока ввода.");
            }
            stream.clear();
            continue;
        }
        
        stringstream ss(line);
        if (ss >> value && ss.eof())
        {
            if (value >= min && value <= max)
            {
                return value;
            }
            else
            {
                stringstream err_ss;
                err_ss << "Число вне допустимого диапазона (от " << fixed << setprecision(1) << min << " до " << max << ").";
                cout << err_ss.str() << "\n";
            }
        }
        else
        {
            cout << "Некорректный ввод. Введите число.\n";
        }
    }
}

string safeGetline(istream& stream, bool isRussianOnly);
string getValidPassword(istream& stream);
Date readDate(istream& stream);
