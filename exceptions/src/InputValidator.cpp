#include "InputValidator.h"
#include "InputException.h"
#include <cctype>
#include <ctime>
#include <regex>

// ВАЖНО: Предполагаем, что Date теперь доступен через инклюд в InputValidator.h

bool containsOnly(const string& str, const regex& pattern)
{
    return regex_match(str, pattern);
}

bool isEmptyOrWhitespace(const string& str) 
{
    if (str.empty()) 
    {
        return true;
    }
    for (char c : str) 
    {
        if (!isspace(static_cast<unsigned char>(c))) 
        {
            return false;
        }
    }
    return true;
}

string safeGetline(istream& stream, bool isRussianOnly)
{
    string line;
    const regex russianRegex("^[а-яА-ЯёЁ\\s\\d]+$");
    const regex englishRegex("^[a-zA-Z\\s\\d]+$");

    bool valid = false;
    do
    {
        try
        {
            if (!getline(stream, line))
            {
                stream.clear();
                throw InputException(300, "Ошибка чтения потока. Попробуйте снова.");
            }

            if (isEmptyOrWhitespace(line))
            {
                throw InputException(301, "Ввод не может быть пустым. Повторите.");
            }

            if (isRussianOnly)
            {
                if (!containsOnly(line, russianRegex))
                {
                    throw InputException(302, "Необходимо использовать только русские буквы, пробелы и цифры. Повторите.");
                }
            }
            else
            {
                if (!containsOnly(line, englishRegex))
                {
                    throw InputException(303, "Необходимо использовать только латинские буквы, пробелы и цифры. Повторите.");
                }
            }
            
            valid = true;
            return line;
        }
        catch (const InputException& e)
        {
            cout << "Error: " << e.what() << "\n";
            valid = false;
        }
    } while (!valid);

    return line;
}

string getValidPassword(istream& stream)
{
    string password;
    bool valid = false;

    do
    {
        try
        {
            if (!getline(stream, password))
            {
                stream.clear();
                throw InputException(400, "Ошибка чтения потока для пароля. Попробуйте снова.");
            }

            if (isEmptyOrWhitespace(password))
            {
                throw InputException(401, "Пароль не может быть пустым. Повторите.");
            }
            
            if (password.length() >= 6)
            {
                valid = true;
                return password;
            }

            throw InputException(402, "Пароль должен содержать не менее 6 символов. Повторите ввод.");
        }
        catch (const InputException& e)
        {
            cout << "Error: " << e.what() << "\n";
            valid = false;
        }
    } while (!valid);

    return password;
}

Date readDate(istream& stream)
{
    string dateStr;
    Date date = {0, 0, 0};
    
    time_t now = time(0);
    tm* ltm = localtime(&now);
    Date currentDate = {ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year};
    
    bool valid = false;
    do
    {
        try
        {
            cout << "Введите дату (YYYY-MM-DD): ";
            if (!getline(stream, dateStr))
            {
                throw std::runtime_error("Конец потока ввода."); 
            }
            
            if (dateStr.length() != 10 || dateStr[4] != '-' || dateStr[7] != '-')
            {
                throw InputException(500, "Некорректный формат даты. Ожидался YYYY-MM-DD.");
            }
            
            date = Date::fromString(dateStr); 
            
            if (date.year < 2000 || date > currentDate || 
                date.month < 1 || date.month > 12 || 
                date.day < 1 || date.day > 31)
            {
                throw InputException(501, "Некорректные значения даты (год, месяц или день).");
            }

            if (date > currentDate)
            {
                throw InputException(502, "Дата не может быть в будущем.");
            }
            
            valid = true;
            return date;

        }
        catch (const InputException& e)
        {
            cout << "Error: " << e.what() << "\n";
            valid = false;
        }
    } while (!valid);
    
    return date;
}