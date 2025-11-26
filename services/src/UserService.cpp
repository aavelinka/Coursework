#include "UserService.h"
#include "InputException.h"
#include <algorithm>
#include <iostream>

User* UserService::findUserByUsername(const string& username)
{
    vector<User*> users = userFile.readAllRecords();
    User* foundUser = nullptr;
    for (User* u : users)
    {
        if (u->getName() == username)
        {
            foundUser = u;
        } else
        {
            delete u;
        }
    }
    if (foundUser)
    {
        loadLatestMeasurement(foundUser);
    }
    return foundUser;
}

void UserService::loadLatestMeasurement(User* user)
{
    vector<Measurement*> measurements = measurementFile.readAllRecords();
    Measurement latest = {0.0, 0.0, 0.0, 0.0, 0.0, {0, 0, 0}};
    Date latestDate = {0, 0, 0};
    
    for (Measurement* m : measurements) {
        if (m->date > latestDate) 
        {
            latestDate = m->date;
            latest = *m;
        }
        delete m;
    }
    
    BodyMeasurement bm;
    bm.setCurrentMeasurement(latest);
    user->setMeasurements(bm);
}

User* UserService::registerUser(const string& name, const string& password, const string& goal, const Measurement& initialMeasurement)
{
    if (name.empty() || password.empty()) 
    {
        throw InputException(20, "Имя пользователя и пароль не могут быть пустыми.");
    }

    vector<User*> existingUsers = userFile.readAllRecords();

    // Проверяю существование пользователя и нахожу максимальный ID за один проход
    int newId = 1;
    bool userExists = false;

    for (User* u : existingUsers)
    {
        if (u->getName() == name) 
        {
            userExists = true;
        }
        if (u->getId() >= newId) 
        {
            newId = u->getId() + 1;
        }
    }

    if (userExists) 
    {
        for (User* u : existingUsers) 
        {
            delete u;
        }
        throw InputException(21, "Пользователь с таким именем уже существует.");
    }

    // Создаем нового пользователя
    BodyMeasurement bm;
    bm.setCurrentMeasurement(initialMeasurement);
    User* newUser = new User(name, bm, password, goal, newId);
    userFile.saveRecord(*newUser);
    measurementFile.saveRecord(initialMeasurement);

    for (User* u : existingUsers) 
    {
        delete u;
    }

    return newUser;
}

User* UserService::loginUser(const string& username, const string& password)
{
    User* user = findUserByUsername(username);
    
    // Пользователь не найден
    if (user == nullptr) 
    {
        return nullptr;
    }
    
    if (user->getPassword() == password) 
    {
        return user;  // Возвращаю пользователя 
    } else 
    {
        delete user;  // Пароль неверный - удаляем
        return nullptr;
    }
}

Measurement UserService::getLatestMeasurement(User* user)
{
    loadLatestMeasurement(user);
    return user->getMeasurements().getCurrentMeasurement();
}