#include "WorkoutService.h"
#include "InputException.h"
#include "TextFile.h"
#include <type_traits>

vector<CardioTraining*> WorkoutService::loadCardioExercises()
{
    TextFile<CardioTraining> file("cardio.txt");
    return file.readAllRecords();
}

vector<UpperBodyWorkout*> WorkoutService::loadUpperBodyExercises()
{
    TextFile<UpperBodyWorkout> file("upperworkout.txt");
    return file.readAllRecords();
}

vector<LowerBodyWorkout*> WorkoutService::loadLowerBodyExercises()
{
    TextFile<LowerBodyWorkout> file("lowerworkout.txt");
    return file.readAllRecords();
}

WorkoutPlan WorkoutService::generatePlan(User* user, double bmi, const string& fitnessLevel, const Measurement& measurement)
{
    WorkoutPlan plan;
    string goal = user->getGoal();
    
    if (bmi < 16.0 && goal == "похудение")
    {
        throw InputException(40, "Ошибка: при критически низком ИМТ (< 16.0) нельзя выбирать цель похудения. Рекомендуется набор мышечной массы.");
    }
    else if (bmi > 40.0 && goal == "набор мышечной массы")
    {
        throw InputException(41, "Ошибка: при критически высоком ИМТ (> 40.0) нельзя выбирать цель набора мышечной массы. Рекомендуется похудение.");
    }
    
    vector<CardioTraining*> allCardio = loadCardioExercises();
    vector<UpperBodyWorkout*> allUpperBody = loadUpperBodyExercises();
    vector<LowerBodyWorkout*> allLowerBody = loadLowerBodyExercises();
    
    int setsChange;
    int repsChange;
    double durationMultiplier;

    if (fitnessLevel == "начальный")
    {
        setsChange = -1;
        repsChange = -2;
        durationMultiplier = 0.75;
    } else if (fitnessLevel == "средний")
    {
        setsChange = 0;
        repsChange = 0;
        durationMultiplier = 1.0;
    } else
    {
        setsChange = 1;
        repsChange = 2;
        durationMultiplier = 1.25;
    }

    int setsGoalAdjust;
    if(goal == "набор мышечной массы") 
    {
        setsGoalAdjust = 1;
    } else 
    {
        setsGoalAdjust = 0;
    }
    double durationGoalAdjust; 
    if(goal == "похудение")
    {
        durationGoalAdjust = 1.2;
    } else if(goal == "набор мышечной массы")
    {
        durationGoalAdjust = 0.7;
    } else
    {
        durationGoalAdjust = 1.0;
    }

    int cardioCount = 0;
    if (goal == "похудение")
    {
        if(bmi >= 30.0)
        {
            cardioCount = 4;
        } else if(bmi >= 25.0)
        {
            cardioCount = 3;
        } else 
        {
            cardioCount = 2;
        }
    }
    else if (goal == "набор мышечной массы")
    {
        cardioCount = 1;
    }
    else
    {
        cardioCount = 2;
    }

    for (int i = 0; i < cardioCount && i < (int)allCardio.size(); ++i)
    {
        CardioTraining* baseEx = allCardio[i];
        double newDuration = max(10.0, round(baseEx->getDuration() * durationMultiplier * durationGoalAdjust / 5.0) * 5.0);
        
        plan.cardio.push_back(new CardioTraining(baseEx->getExerciseName(), newDuration));
    }

    int upperCount;
    if (goal == "набор мышечной массы")
    {
        if(fitnessLevel == "начальный")
        {
            upperCount = 5;
        } else if(fitnessLevel == "средний")
        {
            upperCount = 7;
        } else
        {
            upperCount = 9;
        }
    } else
    {
        if(fitnessLevel == "начальный")
        {
            upperCount = 3;
        } else if(fitnessLevel == "средний")
        {
            upperCount = 5;
        } else
        {
            upperCount = 7;
        }
    }
    vector<string> upperBodyParts = {"Спина", "Грудь", "Плечи", "Бицепс", "Трицепс"};
    selectUpperBodyExercises(allUpperBody, upperCount, upperBodyParts, plan.upperBody, setsChange, repsChange, setsGoalAdjust);
    
    int lowerCount;
    if (goal == "набор мышечной массы")
    {
        if(fitnessLevel == "начальный")
        {
            lowerCount = 4;
        } else if(fitnessLevel == "средний")
        {
            lowerCount = 6;
        } else
        {
            lowerCount = 8;
        }
    } else
    {
        if(fitnessLevel == "начальный")
        {
            lowerCount = 2;
        } else if(fitnessLevel == "средний")
        {
            lowerCount = 4;
        } else
        {
            lowerCount = 6;
        }
    }
    vector<string> lowerBodyParts = {"Квадрицепс", "Бицепс бедра", "Ягодицы"};
    selectLowerBodyExercises(allLowerBody, lowerCount, lowerBodyParts, plan.lowerBody, setsChange, repsChange, setsGoalAdjust);

    for (CardioTraining* ex : allCardio) 
    {
        delete ex;
    }
    for (UpperBodyWorkout* ex : allUpperBody) 
    {
        delete ex;
    }
    for (LowerBodyWorkout* ex : allLowerBody) 
    {
        delete ex;
    }
    
    return plan;
}

void selectUpperBodyExercises(const vector<UpperBodyWorkout*>& allExercises, int targetCount, const vector<string>& bodyParts, vector<UpperBodyWorkout*>& planVector, int setsChange, int repsChange, int setsGoalAdjust)
{
    //totalNeeded берет минимум через функцию min, чтобы не выйти за границы доступных упражнений
    int totalNeeded = min(targetCount, (int)allExercises.size());
    //вектор нулей длиной, равной количеству мышечных групп
    vector<int> exercisesPerPart(bodyParts.size(), 0);
    
    int perPart = totalNeeded / bodyParts.size(); //базовое количество упражнений на каждую группу
    int remainder = totalNeeded % bodyParts.size(); //остаток который нужно распределить
    for (size_t i = 0; i < bodyParts.size(); ++i)
    {
        //распределение по группам. первые remainder групп получают на 1 упражнение больше. пример: [3, 2, 2]
        exercisesPerPart[i] = perPart + (i < (size_t)remainder ? 1 : 0);
    }
    
    int added = 0;
    for (size_t partIdx = 0; partIdx < bodyParts.size() && added < totalNeeded; ++partIdx)
    {
        int count = 0;
        for (UpperBodyWorkout* ex : allExercises)
        {
            if (ex->getUpperBodyPart() == bodyParts[partIdx] && count < exercisesPerPart[partIdx] && added < totalNeeded)
            {
                int baseSets = ex->getSets();
                int baseReps = ex->getReps();

                int finalSets = max(1, baseSets + setsChange + setsGoalAdjust);
                int finalReps = max(6, baseReps + repsChange);
                
                UpperBodyWorkout* newEx = new UpperBodyWorkout(ex->getExerciseName(), finalSets, finalReps, ex->getUpperBodyPart());
                planVector.push_back(newEx);
                count++;
                added++;
            }
        }
    }
}

void selectLowerBodyExercises(const vector<LowerBodyWorkout*>& allExercises, int targetCount, const vector<string>& bodyParts, vector<LowerBodyWorkout*>& planVector, int setsChange, int repsChange, int setsGoalAdjust)
{
    int totalNeeded = min(targetCount, (int)allExercises.size());
    vector<int> exercisesPerPart(bodyParts.size(), 0);
    
    int perPart = totalNeeded / bodyParts.size();
    int remainder = totalNeeded % bodyParts.size();
    for (size_t i = 0; i < bodyParts.size(); ++i)
    {
        exercisesPerPart[i] = perPart + (i < (size_t)remainder ? 1 : 0);
    }
    
    int added = 0;
    for (size_t partIdx = 0; partIdx < bodyParts.size() && added < totalNeeded; ++partIdx)
    {
        int count = 0;
        for (LowerBodyWorkout* ex : allExercises)
        {
            if (ex->getLowerBodyPart() == bodyParts[partIdx] && count < exercisesPerPart[partIdx] && added < totalNeeded)
            {
                int baseSets = ex->getSets();
                int baseReps = ex->getReps();

                int finalSets = max(1, baseSets + setsChange + setsGoalAdjust);
                int finalReps = max(6, baseReps + repsChange);
                
                LowerBodyWorkout* newEx = new LowerBodyWorkout(ex->getExerciseName(), finalSets, finalReps, ex->getLowerBodyPart());
                planVector.push_back(newEx);
                count++;
                added++;
            }
        }
    }
}

WorkoutPlan WorkoutService::createCustomPlan(User* user)
{
    WorkoutPlan plan;
    cout << "\n=== СОЗДАНИЕ ИНДИВИДУАЛЬНОГО ПЛАНА ТРЕНИРОВКИ ===\n\n";
    
    int userId = user->getId();
    
    cout << "ДОБАВЛЕНИЕ КАРДИО УПРАЖНЕНИЙ:\n";
    char addMore;
    do {
        cout << "Введите название кардио упражнения: ";
        cin.ignore();
        string name = safeGetline(cin, true);
        
        cout << "Введите продолжительность (в минутах): ";
        double duration = getValidNumericValue(cin, 0.0, 120.0);
        
        plan.cardio.push_back(new CardioTraining(name, duration));
        
        cout << "Добавить еще одно кардио упражнение? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    // Добавление упражнений для верхней части тела
    cout << "\nДОБАВЛЕНИЕ УПРАЖНЕНИЙ ДЛЯ ВЕРХНЕЙ ЧАСТИ ТЕЛА:\n";
    vector<string> upperBodyParts = {"Спина", "Грудь", "Плечи", "Бицепс", "Трицепс"};
    do {
        string name, bodyPart;
        int sets, reps;
        
        cout << "Введите название упражнения: ";
        cin.ignore();
        name = safeGetline(cin, true);
        
        cout << "Доступные части тела: ";
        for (size_t i = 0; i < upperBodyParts.size(); ++i) {
            cout << upperBodyParts[i];
            if (i < upperBodyParts.size() - 1) 
            {
                cout << ", ";
            }
        }
        cout << "\nВведите часть тела: ";
        bodyPart = safeGetline(cin, true);
        
        // Проверка корректности части тела
        bool validPart = false;
        for (const string& part : upperBodyParts) {
            if (part == bodyPart) {
                validPart = true;
                break;
            }
        }
        if (!validPart) {
            cout << "Ошибка: неверная часть тела. Используйте одну из доступных.\n";
            continue;
        }
        
        cout << "Введите количество подходов: ";
        sets = getValidNumericValue(cin, 1, 20);
        cout << "Введите количество повторений: ";
        reps = getValidNumericValue(cin, 1, 20);
        
        plan.upperBody.push_back(new UpperBodyWorkout(name, sets, reps, bodyPart));
        
        cout << "Добавить еще одно упражнение для верхней части тела? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    // Добавление упражнений для нижней части тела
    cout << "\nДОБАВЛЕНИЕ УПРАЖНЕНИЙ ДЛЯ НИЖНЕЙ ЧАСТИ ТЕЛА:\n";
    vector<string> lowerBodyParts = {"Квадрицепс", "Бицепс бедра", "Ягодицы"};
    do {
        string name, bodyPart;
        int sets, reps;
        
        cout << "Введите название упражнения: ";
        cin.ignore();
        name = safeGetline(cin, true);
        
        cout << "Доступные части тела: ";
        for (size_t i = 0; i < lowerBodyParts.size(); ++i) {
            cout << lowerBodyParts[i];
            if (i < lowerBodyParts.size() - 1) cout << ", ";
        }
        cout << "\nВведите часть тела: ";
        bodyPart = safeGetline(cin, true);
        
        // Проверка корректности части тела
        bool validPart = false;
        for (const string& part : lowerBodyParts) {
            if (part == bodyPart) {
                validPart = true;
                break;
            }
        }
        if (!validPart) {
            cout << "Ошибка: неверная часть тела. Используйте одну из доступных.\n";
            continue;
        }
        
        cout << "Введите количество подходов: ";
        sets = getValidNumericValue(cin, 1, 20);
        cout << "Введите количество повторений: ";
        reps = getValidNumericValue(cin, 1, 20);
        
        plan.lowerBody.push_back(new LowerBodyWorkout(name, sets, reps, bodyPart));
        
        cout << "Добавить еще одно упражнение для нижней части тела? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    // Сохраняем созданный план
    saveUserWorkout(userId, plan);
    
    cout << "Индивидуальный план тренировки успешно создан и сохранен!\n";
    return plan;
}

void WorkoutService::printPlan(const WorkoutPlan& plan)
{
    cout << "\n=== ПЛАН ТРЕНИРОВКИ ===\n\n";
    
    if (!plan.cardio.empty())
    {
        cout << setw(30) << left << "КАРДИО ТРЕНИРОВКИ" << "\n";
        cout << setw(20) << left << "Вид" << "| " << "Продолжительность, мин" << "\n";
        cout << string(40, '-') << "\n";
        for (const CardioTraining* ex : plan.cardio)
        {
            cout << setw(20) << left << ex->getExerciseName() << "| " << ex->getDuration() << "\n";
        }
        cout << "\n";
    }
    
    if (!plan.upperBody.empty())
    {
        cout << setw(30) << left << "ВЕРХНЯЯ ЧАСТЬ ТЕЛА" << "\n";
        cout << setw(15) << left << "Часть тела" << "| " << setw(20) << left << "Название" << "| " 
             << setw(10) << left << "Подходы" << "| " << "Повторения" << "\n";
        cout << string(60, '-') << "\n";
        for (const UpperBodyWorkout* ex : plan.upperBody)
        {
            cout << setw(15) << left << ex->getUpperBodyPart() << "| " 
                 << setw(20) << left << ex->getExerciseName() << "| " 
                 << setw(10) << left << ex->getSets() << "| " 
                 << ex->getReps() << "\n";
        }
        cout << "\n";
    }
    
    if (!plan.lowerBody.empty())
    {
        cout << setw(30) << left << "НИЖНЯЯ ЧАСТЬ ТЕЛА" << "\n";
        cout << setw(15) << left << "Часть тела" << "| " << setw(20) << left << "Название" << "| " 
             << setw(10) << left << "Подходы" << "| " << "Повторения" << "\n";
        cout << string(60, '-') << "\n";
        for (const LowerBodyWorkout* ex : plan.lowerBody)
        {
            cout << setw(15) << left << ex->getLowerBodyPart() << "| " 
                 << setw(20) << left << ex->getExerciseName() << "| " 
                 << setw(10) << left << ex->getSets() << "| " 
                 << ex->getReps() << "\n";
        }
        cout << "\n";
    }
    
    if (plan.cardio.empty() && plan.upperBody.empty() && plan.lowerBody.empty())
    {
        cout << "Нет доступных упражнений. Попробуйте сгенерировать план.\n";
    }
}

void WorkoutService::saveUserWorkout(int userId, const WorkoutPlan& plan)
{
    // Создаем отдельные файлы для каждого типа упражнений
    string cardioFilename = "cardio_" + to_string(userId) + ".txt";
    string upperBodyFilename = "upper_body_" + to_string(userId) + ".txt";
    string lowerBodyFilename = "lower_body_" + to_string(userId) + ".txt";
    
    // Сохраняем кардио упражнения
    TextFile<CardioTraining> cardioFile(cardioFilename);
    cardioFile.clearFile();
    for (const CardioTraining* ex : plan.cardio) 
    {
        cardioFile.saveRecord(*ex);
    }
    
    // Сохраняем упражнения для верхней части тела
    TextFile<UpperBodyWorkout> upperBodyFile(upperBodyFilename);
    upperBodyFile.clearFile();
    for (const UpperBodyWorkout* ex : plan.upperBody) 
    {
        upperBodyFile.saveRecord(*ex);
    }
    
    // Сохраняем упражнения для нижней части тела
    TextFile<LowerBodyWorkout> lowerBodyFile(lowerBodyFilename);
    lowerBodyFile.clearFile();
    for (const LowerBodyWorkout* ex : plan.lowerBody) 
    {
        lowerBodyFile.saveRecord(*ex);
    }
}

WorkoutPlan WorkoutService::loadUserWorkout(int userId)
{
    WorkoutPlan plan;
    
    // Загружаем из отдельных файлов для каждого типа упражнений
    string cardioFilename = "cardio_" + to_string(userId) + ".txt";
    string upperBodyFilename = "upper_body_" + to_string(userId) + ".txt";
    string lowerBodyFilename = "lower_body_" + to_string(userId) + ".txt";
    
    // Загружаем кардио упражнения
    TextFile<CardioTraining> cardioFile(cardioFilename);
    if (cardioFile.hasRecords()) 
    {
        plan.cardio = cardioFile.readAllRecords();
    }
    
    // Загружаем упражнения для верхней части тела
    TextFile<UpperBodyWorkout> upperBodyFile(upperBodyFilename);
    if (upperBodyFile.hasRecords()) 
    {
        plan.upperBody = upperBodyFile.readAllRecords();
    } 
    
    // Загружаем упражнения для нижней части тела
    TextFile<LowerBodyWorkout> lowerBodyFile(lowerBodyFilename);
    if (lowerBodyFile.hasRecords()) 
    {
        plan.lowerBody = lowerBodyFile.readAllRecords();
    } 
    
    return plan;
}