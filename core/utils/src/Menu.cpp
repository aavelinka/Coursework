#include "Menu.h"

UndoManager Menu::undoManager; 
WorkoutPlan Menu::currentWorkoutPlan;
UserService Menu::userService;
WorkoutService Menu::workoutService;

void Menu::draw(const string& title, const string options[], int numOptions) 
{
    int max_len = title.length();
    for (int i = 0; i < numOptions; i++) 
    {
        if (options[i].length() > max_len) 
        {
            max_len = options[i].length();
        }
    }
    max_len += 4;
    
    cout << "+" << string(max_len, '-') << "+" << endl;
    cout << "| " << title << string(max_len - title.length() - 1, ' ') << "|" << endl;
    cout << "+" << string(max_len, '-') << "+" << endl;
    for (int i = 0; i < numOptions; i++) 
    {
        cout << "| " << options[i] << string(max_len - options[i].length() - 1, ' ') << "|" << endl;
    }
    cout << "+" << string(max_len, '-') << "+" << endl;
} 

Measurement Menu::getFirstMeasurement()
{
    double weight, height, chest, waist, hips;
    
    cout << "\n--- ПЕРВОНАЧАЛЬНЫЕ ЗАМЕРЫ ---\n";
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
    
    Measurement measurement = {weight, height, chest, waist, hips, today};
    return measurement;
}

string Menu::readGoal(istream& stream)
{
    string goals[] = {"похудение", "набор мышечной массы", "поддержание формы"};
    
    const int numOptions = 3;
    string options[numOptions] = {
        "1. Похудение",
        "2. Набор мышечной массы",
        "3. Поддержание формы"
    };

    Menu::draw("Выберите Вашу цель", options, numOptions);
    cout << "Ваш выбор (1-3): ";
    int goalChoice = getValidNumericValue(stream, 1, 3);

    return goals[goalChoice];
}

void Menu::registerNewUser()
{
    string name, password, goal;
    
    cout << "\n--- РЕГИСТРАЦИЯ ---\n";
    cout << "Введите Имя пользователя (русский): ";
    name = safeGetline(cin, true);
    cout << "Введите Пароль (минимум 6 символов, без проверки языка): ";
    password = getValidPassword(cin);
    
    goal = readGoal(cin);
    
    Measurement initialMeasurement = getFirstMeasurement();
    
    try
    {
        User* user = userService.registerUser(name, password, goal, initialMeasurement);
        cout << "\nРегистрация успешна! Добро пожаловать, " << user->getName() << ".\n";
        delete user;
        undoManager.clear();
    } catch (const InputException& e)
    {
        cout << "\nОшибка: " << e.what() << endl;
    }
}

User* Menu::loginProcess()
{
    string name, password;
    cout << "\n--- ВХОД В СИСТЕМУ ---\n";
    cout << "Введите Имя пользователя: ";
    name = safeGetline(cin, true);
    cout << "Введите Пароль: ";
    password = getValidPassword(cin);
    
    User* user = userService.loginUser(name, password);
    if (user == nullptr)
    {
        cout << "\nОшибка: Неверное имя пользователя или пароль.\n";
    }
    return user;
}

Measurement Menu::inputNewMeasurement()
{
    double weight, height, chest, waist, hips;
    
    cout << "\n--- НОВЫЕ ЗАМЕРЫ ---\n";
    cout << "Введите Ваш текущий вес (кг, 40.0 - 200.0): ";
    weight = getValidNumericValue(cin, 40.0, 200.0);
    cout << "Введите Ваш текущий рост (см, 100.0 - 250.0): ";
    height = getValidNumericValue(cin, 100.0, 250.0);
    cout << "Введите объем груди (см, 50.0 - 150.0): ";
    chest = getValidNumericValue(cin, 50.0, 150.0);
    cout << "Введите объем талии (см, 40.0 - 150.0): ";
    waist = getValidNumericValue(cin, 40.0, 150.0);
    cout << "Введите объем бедер (см, 60.0 - 150.0): ";
    hips = getValidNumericValue(cin, 60.0, 150.0);
    
    Date today = readDate(cin);
    
    Measurement measurement = {weight, height, chest, waist, hips, today};
    return measurement;
}

void Menu::handleNewMeasurement(User* user)
{
    Measurement oldMeasurement = user->getMeasurements().getCurrentMeasurement();
    Measurement newMeasurement = inputNewMeasurement();
    userService.saveMeasurement(user, newMeasurement);
    
    undoManager.addAction([user, oldMeasurement]() {
        userService.saveMeasurement(user, oldMeasurement);
        cout << "\n[ОТМЕНА]: Предыдущие замеры восстановлены.\n";
    });
    
    cout << "\nЗамеры успешно сохранены.\n";
}

void Menu::handleGeneratePlan(User* user)
{
    WorkoutPlan oldPlan = workoutService.loadUserWorkout(user->getId());
    
    Measurement m = userService.getLatestMeasurement(user);
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
        
        string options[2] = {"1. Автоматическая генерация", "2. Ручное создание"};
        Menu::draw("Выберите тип генерации плана", options, 2);
        cout << "Ваш выбор (1-2): ";
        int choice = getValidNumericValue(cin, 1, 2);
        
        WorkoutPlan plan;
        if (choice == 1)
        {
            plan = workoutService.generatePlan(user, bmi, fitnessLevel, m);
        } else
        {
            plan = workoutService.createCustomPlan(user);
        }
        
        workoutService.saveUserWorkout(user->getId(), plan);
        currentWorkoutPlan = plan;
        
        undoManager.addAction([user, oldPlan]() {
            workoutService.saveUserWorkout(user->getId(), oldPlan);
            cout << "\n[ОТМЕНА]: Предыдущий план тренировок восстановлен.\n";
        });

        cout << "\nПлан тренировок успешно создан!\n";
        workoutService.printPlan(currentWorkoutPlan);
    }
    catch (const InputException& e)
    {
        cout << "\nОшибка: " << e.what() << endl;
    }
}

void Menu::handleViewPlan(User* user)
{
    try
    {
        currentWorkoutPlan = workoutService.loadUserWorkout(user->getId());
        if (currentWorkoutPlan.cardio.empty() && currentWorkoutPlan.upperBody.empty() && currentWorkoutPlan.lowerBody.empty())
        {
            cout << "\nПлан тренировок не найден. Пожалуйста, сгенерируйте его сначала (опция 2).\n";
        } else
        {
            cout << "\n--- ТЕКУЩИЙ ПЛАН ТРЕНИРОВОК ---\n";
            workoutService.printPlan(currentWorkoutPlan);
        }
    } catch (const FileException& e)
    {
        cout << "\nОшибка: " << e.what() << endl;
    }
}

void Menu::handleUndoAction()
{
    if (undoManager.undo())
    {
        cout << "\nДействие успешно отменено.\n";
    } else
    {
        cout << "\nНечего отменять (стек отмены пуст).\n";
    }
}

void Menu::run()
{
    string options[3] = {"1. Войти", "2. Зарегистрироваться", "3. Выход"};
    Menu::draw("ФИТНЕС ТРЕКЕР: Авторизация", options, 3);
    cout << "Выберите опцию (1-3): ";
    
    int initialChoice = getValidNumericValue(cin, 1, 3);
    
    User* currentUser = nullptr;
    
    if (initialChoice == 1)
    {
        currentUser = loginProcess();
    } else if (initialChoice == 2)
    {
        registerNewUser();
        currentUser = loginProcess(); 
    } else 
    {
        cout << "Выход..." << endl;
        return;
    }
    
    if (currentUser == nullptr)
    {
        cout << "Не удалось войти в систему.\n";
        return;
    }
    
    int choice = 0;
    
    while (choice != 5)
    {
        Measurement m = userService.getLatestMeasurement(currentUser);
        double bmi = BMICalculator::calculateBMI(m.weight, m.height);
        
        string title = "ФИТНЕС ТРЕКЕР - " + currentUser->getName();
        string bmiClassification = "ИМТ: Неизвестно";
        string fitnessLevel = "";

        if (m.date.year != 0)
        {
            bmiClassification = "ИМТ: " + to_string(bmi) + " (" + BMICalculator::classifyUser(bmi, currentUser->getGoal()) + ")";
            fitnessLevel = "Уровень: " + BMICalculator::getFitnessLevel(bmi, currentUser->getGoal());
        }

        const int numOptions = 5;
        string menuOptions[numOptions] = {
            "1. Внести новые замеры",
            "2. Сгенерировать план тренировок",
            "3. Посмотреть текущий план тренировок",
            "4. Отменить последнее действие (Ctrl+Z)",
            "5. Выход (Сохранение данных)"
        };

        Menu::draw(title, menuOptions, numOptions);
        
        if (m.date.year != 0)
        {
            cout << "Последние замеры (" << m.date.toString() << "): Вес: " << m.weight << " кг, Рост: " << m.height << " см\n";
            cout << bmiClassification << "\n";
            cout << fitnessLevel << "\n";
        }
        cout << "Выберите опцию: ";
        
        choice = getValidNumericValue(cin, 1, 5);
        
        try
        {
            switch (choice)
            {
                case 1:
                    handleNewMeasurement(currentUser);
                    break;
                case 2:
                    handleGeneratePlan(currentUser);
                    break;
                case 3:
                    handleViewPlan(currentUser);
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
    
    delete currentUser;
}
