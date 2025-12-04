#ifndef MENU_H
#define MENU_H

#include "../../services/include/UserService.h"
#include "../../services/include/WorkoutService.h"
#include "../../services/include/BMICalculator.h"
#include "../../services/include/UndoManager.h"
#include "../../exceptions/include/InputException.h"
#include "../../exceptions/include/FileException.h"
#include "../../exceptions/include/InputValidator.h"
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