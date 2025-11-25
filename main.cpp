#include "UserService.h"
#include "WorkoutService.h"
#include "BMICalculator.h"
#include "UndoManager.h"
#include "InputException.h"
#include "FileException.h"
#include "InputValidator.h"
#include <limits>
#include <map>
#include <functional>

using namespace std;

UndoManager undoManager; 
WorkoutPlan currentWorkoutPlan;

void drawMenu(const string& title, const string options[], int numOptions) {
    int max_len = title.length();
    for (int i = 0; i < numOptions; i++) {
        if (options[i].length() > max_len) max_len = options[i].length();
    }
    max_len += 4;
    cout << "+" << string(max_len, '-') << "+" << endl;
    cout << "| " << title << string(max_len - title.length() - 1, ' ') << "|" << endl;
    cout << "+" << string(max_len, '-') << "+" << endl;
    for (int i = 0; i < numOptions; i++) {
        cout << "| " << options[i] << string(max_len - options[i].length() - 1, ' ') << "|" << endl;
    }
    cout << "+" << string(max_len, '-') << "+" << endl;
}

void printMainMenu(User* user, double bmi)
{
    Measurement m = user->getMeasurements().getCurrentMeasurement();
    
    string title = "ФИТНЕС ТРЕКЕР - " + user->getName();
    
    string bmiClassification = "ИМТ: Неизвестно";
    string fitnessLevel = "";

    if (m.date.year != 0)
    {
        bmiClassification = "ИМТ: " + to_string(bmi) + " (" + BMICalculator::classifyUser(bmi, user->getGoal()) + ")";
        fitnessLevel = "Уровень: " + BMICalculator::getFitnessLevel(bmi, user->getGoal());
    }

    const int numOptions = 5;
    string options[numOptions] = {
        "1. Внести новые замеры",
        "2. Сгенерировать план тренировок",
        "3. Посмотреть текущий план тренировок",
        "4. Отменить последнее действие (Ctrl+Z)",
        "5. Выход (Сохранение данных)"
    };

    drawMenu(title, options, numOptions);
    
    if (m.date.year != 0)
    {
        cout << "Последние замеры (" << m.date.toString() << "): Вес: " << m.weight << " кг, Рост: " << m.height << " см\n";
        cout << bmiClassification << "\n";
        cout << fitnessLevel << "\n";
    }
    cout << "Выберите опцию: ";
}

string readGoal(istream& stream)
{
    map<int, string> goals = {{1, "похудение"}, {2, "набор мышечной массы"}, {3, "поддержание формы"}};
    
    const int numOptions = 3;
    string options[numOptions] = {
        "1. Похудение",
        "2. Набор мышечной массы",
        "3. Поддержание формы"
    };

    drawMenu("Выберите Вашу цель", options, numOptions);
    cout << "Ваш выбор (1-3): ";
    
    int goalChoice = getValidNumericValue(stream, 1, 3);
    return goals[goalChoice];
}

void registerNewUser(UserService& us)
{
    string name, password, goal;
    double weight, height;
    
    cout << "\n--- РЕГИСТРАЦИЯ ---\n";
    cout << "Введите Имя пользователя (латиница): ";
    name = safeGetline(cin, false);
    cout << "Введите Пароль (минимум 6 символов, без проверки языка): ";
    password = getValidPassword(cin);
    
    goal = readGoal(cin);
    
    cout << "Введите Ваш текущий вес (кг, 40.0 - 200.0): ";
    weight = getValidNumericValue(cin, 40.0, 200.0);
    cout << "Введите Ваш текущий рост (см, 100.0 - 250.0): ";
    height = getValidNumericValue(cin, 100.0, 250.0);
    
    Date today = readDate(cin);
    
    Measurement initialMeasurement = {weight, height, 0.0, 0.0, 0.0, today};
    
    try
    {
        User* user = us.registerUser(name, password, goal, initialMeasurement);
        cout << "\nРегистрация успешна! Добро пожаловать, " << user->getName() << ".\n";
        delete user;
        undoManager.clear();
    }
    catch (const InputException& e)
    {
        cout << "\nОшибка: " << e.what() << endl;
    }
}

User* loginProcess(UserService& us)
{
    string name, password;
    cout << "\n--- ВХОД В СИСТЕМУ ---\n";
    cout << "Введите Имя пользователя: ";
    name = safeGetline(cin, false);
    cout << "Введите Пароль: ";
    password = getValidPassword(cin);
    
    User* user = us.loginUser(name, password);
    if (user == nullptr)
    {
        cout << "\nОшибка: Неверное имя пользователя или пароль.\n";
    }
    return user;
}

void handleNewMeasurement(UserService& us, User* user)
{
    Measurement oldMeasurement = user->getMeasurements().getCurrentMeasurement();
    
    double weight, height, chest, waist, hips;
    
    cout << "\n--- НОВЫЕ ЗАМЕРЫ ---\n";
    cout << "Введите Ваш текущий вес (кг, 40.0 - 200.0): ";
    weight = getValidNumericValue<double>(cin, 40.0, 200.0);
    cout << "Введите Ваш текущий рост (см, 100.0 - 250.0): ";
    height = getValidNumericValue(cin, 100.0, 250.0);
    cout << "Введите объем груди (см, 50.0 - 150.0): ";
    chest = getValidNumericValue(cin, 50.0, 150.0);
    cout << "Введите объем талии (см, 40.0 - 150.0): ";
    waist = getValidNumericValue(cin, 40.0, 150.0);
    cout << "Введите объем бедер (см, 60.0 - 150.0): ";
    hips = getValidNumericValue(cin, 60.0, 150.0);
    
    Date today = readDate(cin);
    
    Measurement newMeasurement = {weight, height, chest, waist, hips, today};
    us.saveMeasurement(user, newMeasurement);
    
    undoManager.addAction([&us, user, oldMeasurement]() {
        us.saveMeasurement(user, oldMeasurement);
        cout << "\n[ОТМЕНА]: Предыдущие замеры восстановлены.\n";
    });
    
    cout << "\nЗамеры успешно сохранены.\n";
}

void handleGeneratePlan(UserService& us, WorkoutService& ws, User* user)
{
    WorkoutPlan oldPlan = ws.loadUserWorkout(user->getId());
    
    Measurement m = us.getLatestMeasurement(user);
    if (m.date.year == 0)
    {
        cout << "Необходимо внести начальные замеры (опция 1).\n";
        return;
    }

    double bmi = BMICalculator::calculateBMI(m.weight, m.height);
    string fitnessLevel = BMICalculator::getFitnessLevel(bmi, user->getGoal());
    
    if (fitnessLevel == "нельзя худеть!")
    {
        cout << "\nОшибка: Ваш ИМТ слишком низок (<18.0) для цели 'похудение'. Смените цель или внесите корректные замеры.\n";
        return;
    }

    try
    {
        cout << "\n--- ГЕНЕРАЦИЯ ПЛАНА ---\n";
        cout << "Ваш расчетный уровень: " << fitnessLevel << " (на основе ИМТ и цели).\n";
        
        string options[2] = {"1. Да", "2. Нет"};
        drawMenu("Подтвердите генерацию плана", options, 2);
        cout << "Ваш выбор (1-2): ";
        int choice = getValidNumericValue(cin, 1, 2);
        
        if (choice == 1)
        {
            WorkoutPlan plan = ws.generatePlan(user, bmi, fitnessLevel, m);
            ws.saveUserWorkout(user->getId(), plan);
            currentWorkoutPlan = plan;
            
            undoManager.addAction([&ws, user, oldPlan]() {
                ws.saveUserWorkout(user->getId(), oldPlan);
                cout << "\n[ОТМЕНА]: Предыдущий план тренировок восстановлен.\n";
            });

            cout << "\nПлан тренировок успешно сгенерирован!\n";
            ws.printPlan(currentWorkoutPlan);
        }
    }
    catch (const InputException& e)
    {
        cout << "\nОшибка: " << e.what() << endl;
    }
}

void handleViewPlan(WorkoutService& ws, User* user)
{
    try
    {
        currentWorkoutPlan = ws.loadUserWorkout(user->getId());
        if (currentWorkoutPlan.cardio.empty() && currentWorkoutPlan.upperBody.empty() && currentWorkoutPlan.lowerBody.empty())
        {
            cout << "\nПлан тренировок не найден. Пожалуйста, сгенерируйте его сначала (опция 2).\n";
        }
        else
        {
            cout << "\n--- ТЕКУЩИЙ ПЛАН ТРЕНИРОВОК ---\n";
            ws.printPlan(currentWorkoutPlan);
        }
    }
    catch (const FileException& e)
    {
        cout << "\nОшибка: " << e.what() << endl;
    }
}

void handleUndoAction()
{
    if (undoManager.undo())
    {
        cout << "\nДействие успешно отменено.\n";
    }
    else
    {
        cout << "\nНечего отменять (стек отмены пуст).\n";
    }
}

void app(User* user)
{
    UserService userService;
    WorkoutService workoutService;
    int choice = 0;
    
    while (choice != 5)
    {
        Measurement m = userService.getLatestMeasurement(user);
        double bmi = BMICalculator::calculateBMI(m.weight, m.height);
        
        printMainMenu(user, bmi);
        choice = getValidNumericValue(cin, 1, 5);
        
        try
        {
            switch (choice)
            {
                case 1:
                    handleNewMeasurement(userService, user);
                    break;
                case 2:
                    handleGeneratePlan(userService, workoutService, user);
                    break;
                case 3:
                    handleViewPlan(workoutService, user);
                    break;
                case 4:
                    handleUndoAction();
                    break;
                case 5:
                    cout << "\nСпасибо за использование! Данные сохранены. До свидания.\n";
                    break;
            }
        }
        catch (const InputException& e)
        {
            cout << "\nОшибка ввода: " << e.what() << endl;
        }
        catch (const FileException& e)
        {
            cout << "\nОшибка файла: " << e.what() << endl;
        }
        catch (const exception& e)
        {
            cout << "\nПроизошла неожиданная ошибка: " << e.what() << endl;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    UserService us;
    User* currentUser = nullptr;
    int initialChoice;
    
    string options[3] = {"1. Войти", "2. Зарегистрироваться", "3. Выход"};
    drawMenu("ФИТНЕС ТРЕКЕР: Авторизация", options, 3);
    cout << "Выберите опцию (1-3): ";
    
    try
    {
        initialChoice = getValidNumericValue(cin, 1, 2);
    
        if (initialChoice == 1)
        {
            currentUser = loginProcess(us);
        } else if (initialChoice == 2)
        {
            registerNewUser(us);
            currentUser = loginProcess(us); 
        } else 
        {
            return;
        }
    
        if (currentUser != nullptr)
        {
            app(currentUser);
            delete currentUser;
        } else
        {
            cout << "Не удалось войти в систему. Завершение работы.\n";
        }
    } catch (const exception& e)
    {
        cout << "Критическая ошибка: " << e.what() << endl;
        if (currentUser != nullptr) 
        {
            delete currentUser;
        }
    }

    return 0;
}