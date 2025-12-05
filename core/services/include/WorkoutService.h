#ifndef WORKOUTSERVICE_H
#define WORKOUTSERVICE_H

#include "workout/CardioTraining.h"
#include "workout/UpperBodyWorkout.h"
#include "workout/LowerBodyWorkout.h"
#include "user/User.h"
#include "InputValidator.h"
#include "TextFile.h"
#include <random>
#include <cmath>

using namespace std;

struct WorkoutPlan
{
    vector<CardioTraining*> cardio;
    vector<UpperBodyWorkout*> upperBody;
    vector<LowerBodyWorkout*> lowerBody;
    
    WorkoutPlan() = default;
    WorkoutPlan(const WorkoutPlan& other)
    {
        for (const CardioTraining* c : other.cardio)
        {
            cardio.push_back(new CardioTraining(*c));
        }
        for (const UpperBodyWorkout* u : other.upperBody)
        {
            upperBody.push_back(new UpperBodyWorkout(*u));
        }
        for (const LowerBodyWorkout* l : other.lowerBody)
        {
            lowerBody.push_back(new LowerBodyWorkout(*l));
        }
    }
    ~WorkoutPlan()
    {
        for (CardioTraining* c : cardio) 
        {
            delete c;
        }
        for (UpperBodyWorkout* u : upperBody) 
        {
            delete u;
        }
        for (LowerBodyWorkout* l : lowerBody) 
        {
            delete l;
        }
    }
    WorkoutPlan& operator=(const WorkoutPlan& other)
    {
        if (this != &other) 
        {
            for (CardioTraining* c : cardio) 
            {
                delete c;
            }
            for (UpperBodyWorkout* u : upperBody) 
            {
                delete u;
            }
            for (LowerBodyWorkout* l : lowerBody) 
            {
                delete l;
            }
            cardio.clear();
            upperBody.clear();
            lowerBody.clear();
            
            for (const CardioTraining* c : other.cardio)
            {
                cardio.push_back(new CardioTraining(*c));
            }
            for (const UpperBodyWorkout* u : other.upperBody)
            {
                upperBody.push_back(new UpperBodyWorkout(*u));
            }
            for (const LowerBodyWorkout* l : other.lowerBody)
            {
                lowerBody.push_back(new LowerBodyWorkout(*l));
            }
        }
        
        return *this;
    }
};

class WorkoutService
{
private:
    vector<CardioTraining*> loadCardioExercises();
    vector<UpperBodyWorkout*> loadUpperBodyExercises();
    vector<LowerBodyWorkout*> loadLowerBodyExercises();
    
public:
    WorkoutService() {}
    ~WorkoutService() {}
    
    WorkoutPlan generatePlan(User* user, double bmi, const string& fitnessLevel, const Measurement& measurement);
    WorkoutPlan createCustomPlan(User* user);
    void selectUpperBodyExercises(const vector<UpperBodyWorkout*>& allExercises, int targetCount, const vector<string>& bodyParts, vector<UpperBodyWorkout*>& planVector, int setsChange, int repsChange, int setsGoalAdjust);
    void selectLowerBodyExercises(const vector<LowerBodyWorkout*>& allExercises, int targetCount, const vector<string>& bodyParts, vector<LowerBodyWorkout*>& planVector, int setsChange, int repsChange, int setsGoalAdjust);
    void printPlan(const WorkoutPlan& plan);
    void saveUserWorkout(int userId, const WorkoutPlan& plan);
    WorkoutPlan loadUserWorkout(int userId);
};

#endif
