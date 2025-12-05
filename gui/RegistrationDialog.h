#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <memory>
#include "UserService.h"

class QLineEdit;
class QComboBox;

class RegistrationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegistrationDialog(UserService* userService, QWidget* parent = nullptr);
    std::unique_ptr<User> takeUser();

private slots:
    void handleRegister();

private:
    Measurement buildMeasurement() const;
    void applyBranding();

    UserService* userService;
    std::unique_ptr<User> createdUser;

    QLineEdit* usernameEdit{nullptr};
    QLineEdit* passwordEdit{nullptr};
    QComboBox* goalBox{nullptr};
    QLineEdit* weightEdit{nullptr};
    QLineEdit* heightEdit{nullptr};
    QLineEdit* chestEdit{nullptr};
    QLineEdit* waistEdit{nullptr};
    QLineEdit* hipsEdit{nullptr};
};

#endif
