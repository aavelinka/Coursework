#include "InputValidator.h"
#include "InputException.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <stdexcept>
#include <regex>

bool containsOnly(const string& str, const regex& pattern)
{
    return regex_match(str, pattern);
}

string safeGetline(istream& stream, bool isRussianOnly)
{
    string line;
    
    // Регулярные выражения для проверки языка:
    // ^[а-яА-ЯёЁ\s\d]+$ - Только русские буквы, пробелы и цифры
    // ^[a-zA-Z\s\d]+$ - Только латинские буквы, пробелы и цифры
    const regex russianRegex("^[а-яА-ЯёЁ\\s\\d]+$");
    const regex englishRegex("^[a-zA-Z\\s\\d]+$");
    
    while (true)
    {
        getline(stream, line);
        
        // 1. Проверка на непустоту
        if (line.empty() || all_of(line.begin(), line.end(), [](char c){ return isspace(static_cast<unsigned char>(c)); }))
        {
            cout << "Ввод не может быть пустым. Повторите:\n";
            continue;
        }

        // 2. Проверка языка
        if (isRussianOnly)
        {
            if (!containsOnly(line, russianRegex))
            {
                cout << "Необходимо использовать только русские буквы, пробелы и цифры. Повторите:\n";
                continue;
            }
        }
        else
        {
            if (!containsOnly(line, englishRegex))
            {
                cout << "Необходимо использовать только латинские буквы, пробелы и цифры. Повторите:\n";
                continue;
            }
        }
        
        return line;
    }
}

string getValidPassword(istream& stream)
{
    string password;
    while (true)
    {
        // Для пароля используем обычный getline(проверка только на непустоту и длину)
        getline(stream, password);
        
        if (password.empty() || all_of(password.begin(), password.end(), [](char c){ return isspace(static_cast<unsigned char>(c)); }))
        {
            cout << "Пароль не может быть пустым. Повторите:\n";
            continue;
        }
        
        if (password.length() >= 6)
        {
            return password;
        }
        cout << "Пароль должен содержать не менее 6 символов. Повторите ввод:\n";
    }
}

Date readDate(istream& stream)
{
    string dateStr;
    Date date = {0, 0, 0};
    
    // Получаем текущую дату для валидации
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;
    
    while (true)
    {
        cout << "Введите дату (YYYY-MM-DD): ";
        // Для даты не делаем проверку языка, так как это просто цифры и дефисы
        if (!getline(stream, dateStr))
        {
            throw std::runtime_error("Конец потока ввода.");
        }
        
        if (dateStr.length() == 10 && dateStr[4] == '-' && dateStr[7] == '-')
        {
            date = Date::fromString(dateStr);
            
            // Простая валидация
            if (date.year >= 2000 && date.year <= currentYear && 
                date.month >= 1 && date.month <= 12 && 
                date.day >= 1 && date.day <= 31)
            {
                // Проверка на дату в будущем (сегодня или прошлое)
                if (date.year < currentYear || 
                    (date.year == currentYear && date.month < currentMonth) ||
                    (date.year == currentYear && date.month == currentMonth && date.day <= currentDay))
                {
                    return date;
                }
            }
        }
        cout << "Некорректный формат даты или дата в будущем. Повторите ввод (YYYY-MM-DD, до сегодняшнего дня).\n";
    }
}