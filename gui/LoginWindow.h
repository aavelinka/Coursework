#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "UserService.h"
#include "WorkoutService.h"

class QLineEdit;
class QLabel;
class DashboardWindow;
class RegistrationDialog;

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

private slots:
    void handleLogin();
    void openRegistration();

private:
    void buildUi();
    void applyBranding();
    void openDashboard(std::unique_ptr<User> user);
    void showError(const QString& message);

    UserService userService;
    WorkoutService workoutService;
    std::unique_ptr<DashboardWindow> dashboard;

    QLineEdit* usernameEdit{nullptr};
    QLineEdit* passwordEdit{nullptr};
    QLabel* errorLabel{nullptr};
};

#endif
