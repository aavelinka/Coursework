#include "BMICalculator.h"
#include <iostream>

double BMICalculator::calculateBMI(double weight, double height)
{
    if (height <= 0)
    {
        return 0.0;
    }
    double heightInMeters = height / 100.0;
    return weight / (heightInMeters * heightInMeters);
}

string BMICalculator::classifyUser(double bmi, const string& goal)
{
    if (bmi < 18.5)
    {
        return "недостаточный вес";
    }
    else if (bmi < 25.0)
    {
        return "нормальный вес";
    }
    else if (bmi < 30.0)
    {
        return "избыточный вес";
    }
    else
    {
        return "ожирение";
    }
}

string BMICalculator::getFitnessLevel(double bmi, const string& goal)
{
    if (goal == "похудение")
    {
        if (bmi >= 30.0)
        {
            return "начальный";
        } else if (bmi >= 25.0)
        {
            return "средний";
        } else if(bmi >= 18.0)
        {
            return "продвинутый"; 
        } else 
        {
            return "нельзя худеть!"; 
        }
    }
    else if (goal == "набор мышечной массы")
    {
        if (bmi < 20.0 || bmi > 30.0)
        {
            return "начальный";
        } else if (bmi < 25.0)
        {
            return "средний";
        } else
        {
            return "продвинутый";
        }
    }
    else // поддержание формы
    {
        if (bmi >= 18.0 && bmi < 25.0)
        {
            return "продвинутый";
        }
        else
        {
            return "средний";
        }
    }
}