#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "user/User.h"
#include "TextFile.h"

using namespace std;

class UserService
{
private:
    TextFile<User> userFile; //файл хранящий всех пользователей
    TextFile<Measurement> measurementFile; //индивидуальный файл для записи замеров каждого пользователя

    User* findUserByUsername(const string& username);
    void loadLatestMeasurement(User* user);

public:
    UserService() : userFile("users.txt"), measurementFile(""){}
    
    User* registerUser(const string& name, const string& password, const string& goal, const Measurement& initialMeasurement);
    User* loginUser(const string& username, const string& password);
    void saveMeasurement(User* user, const Measurement& measurement);
    Measurement getLatestMeasurement(User* user);
};

#endif