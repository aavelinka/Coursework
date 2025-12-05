#ifndef MENU_H
#define MENU_H

#include "UserService.h"
#include "WorkoutService.h"
#include "BMICalculator.h"
#include "UndoManager.h"
#include "InputException.h"
#include "FileException.h"
#include "InputValidator.h"
#include <limits>
#include <functional>

using namespace std;

class Menu 
{
private:
    static UndoManager undoManager; 
    static WorkoutPlan currentWorkoutPlan;
    static UserService userService;
    static WorkoutService workoutService;

public:
    static void draw(const string& title, const string options[], int numOptions);
    static Measurement getFirstMeasurement();
    static string readGoal(istream& stream);
    static void registerNewUser();
    static User* loginProcess();
    static Measurement inputNewMeasurement();
    static void handleNewMeasurement(User* user);
    static void handleGeneratePlan(User* user);
    static void handleViewPlan(User* user);
    static void handleUndoAction();
    static void run();
};

#endif
