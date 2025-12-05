#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "UserService.h"
#include "WorkoutService.h"

class QLabel;
class QTextEdit;
class QLineEdit;
class QTabWidget;
class HistoryChartWidget;

class DashboardWindow : public QMainWindow
{
    Q_OBJECT
public:
    DashboardWindow(UserService* userService,
                    WorkoutService* workoutService,
                    std::unique_ptr<User> user,
                    QWidget* parent = nullptr);

    void setUser(std::unique_ptr<User> user);

signals:
    void logoutRequested();

private slots:
    void saveMeasurement();
    void generatePlan();
    void logout();

private:
    void buildUi();
    void applyBranding();
    void refreshUserSummary();
    void refreshHistory();
    Measurement readMeasurementInputs() const;
    std::vector<Measurement> loadMeasurements() const;
    void updateBmiInfo(const Measurement& latest);

    UserService* userService;
    WorkoutService* workoutService;
    std::unique_ptr<User> user;

    QLabel* titleLabel{nullptr};
    QLabel* latestMeasurementLabel{nullptr};
    QLabel* bmiInfoLabel{nullptr};
    QTextEdit* planView{nullptr};
    QTextEdit* workoutsHistory{nullptr};
    QLineEdit* weightEdit{nullptr};
    QLineEdit* heightEdit{nullptr};
    QLineEdit* chestEdit{nullptr};
    QLineEdit* waistEdit{nullptr};
    QLineEdit* hipsEdit{nullptr};
    QTabWidget* tabs{nullptr};
    HistoryChartWidget* chart{nullptr};
};

#endif
