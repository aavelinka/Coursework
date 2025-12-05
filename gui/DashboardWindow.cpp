#include "DashboardWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QFormLayout>
#include <QDate>
#include <QMessageBox>
#include <QStyle>
#include <QTabWidget>
#include <QPalette>
#include <QScrollArea>
#include <QInputDialog>
#include <QTextOption>
#include <sstream>
#include <iomanip>
#include "InputException.h"
#include "BMICalculator.h"
#include "TextFile.h"
#include "HistoryChartWidget.h"

DashboardWindow::DashboardWindow(UserService* userService,
                                 WorkoutService* workoutService,
                                 std::unique_ptr<User> user,
                                 QWidget* parent)
    : QMainWindow(parent),
      userService(userService),
      workoutService(workoutService),
      user(std::move(user))
{
    buildUi();
    applyBranding();
    refreshUserSummary();
}

void DashboardWindow::setUser(std::unique_ptr<User> newUser)
{
    user = std::move(newUser);
    refreshUserSummary();
}

void DashboardWindow::buildUi()
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* root = new QVBoxLayout(central);
    root->setContentsMargins(32, 32, 32, 32);
    root->setSpacing(18);

    titleLabel = new QLabel(this);
    QFont titleFont("Avenir Next", 22, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setStyleSheet("color: #0f2f2f;");
    titleLabel->setWordWrap(true);

    latestMeasurementLabel = new QLabel(this);
    latestMeasurementLabel->setStyleSheet("color: rgba(15,47,47,0.85);");
    latestMeasurementLabel->setWordWrap(true);

    tabs = new QTabWidget(this);
    tabs->setObjectName("mainTabs");
    tabs->setTabPosition(QTabWidget::North);

    // Tab 1: History / progress
    QWidget* progressTab = new QWidget(this);
    QVBoxLayout* progressLayout = new QVBoxLayout(progressTab);
    progressLayout->setSpacing(14);

    QHBoxLayout* topInfo = new QHBoxLayout();
    topInfo->setSpacing(12);

    chart = new HistoryChartWidget(this);
    chart->setObjectName("chartWidget");
    chart->setMinimumHeight(260);

    QWidget* measureCard = new QWidget(this);
    measureCard->setObjectName("card");
    QVBoxLayout* measureLayout = new QVBoxLayout(measureCard);
    measureLayout->setSpacing(10);
    QLabel* measureTitle = new QLabel("Измерения", this);
    measureTitle->setStyleSheet("font-weight: 700; color: #0f2f2f; font-size: 16px;");

    QFormLayout* measureForm = new QFormLayout();
    measureForm->setSpacing(8);
    measureForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    measureForm->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    weightEdit = new QLineEdit(this);
    weightEdit->setPlaceholderText("кг");
    weightEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    heightEdit = new QLineEdit(this);
    heightEdit->setPlaceholderText("см");
    heightEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    chestEdit = new QLineEdit(this);
    chestEdit->setPlaceholderText("см");
    chestEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    waistEdit = new QLineEdit(this);
    waistEdit->setPlaceholderText("см");
    waistEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    hipsEdit = new QLineEdit(this);
    hipsEdit->setPlaceholderText("см");
    hipsEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    measureForm->addRow("Вес", weightEdit);
    measureForm->addRow("Рост", heightEdit);
    measureForm->addRow("Грудь", chestEdit);
    measureForm->addRow("Талия", waistEdit);
    measureForm->addRow("Бедра", hipsEdit);

    QPushButton* saveMeasurementBtn = new QPushButton("Сохранить измерения", this);
    saveMeasurementBtn->setObjectName("filledButton");
    saveMeasurementBtn->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(saveMeasurementBtn, &QPushButton::clicked, this, &DashboardWindow::saveMeasurement);

    bmiInfoLabel = new QLabel(this);
    bmiInfoLabel->setStyleSheet("color: rgba(15,47,47,0.9); font-size: 14px; font-weight: 600;");
    bmiInfoLabel->setWordWrap(true);

    measureLayout->addWidget(measureTitle);
    measureLayout->addLayout(measureForm);
    measureLayout->addWidget(saveMeasurementBtn);
    measureLayout->addWidget(bmiInfoLabel);

    topInfo->addWidget(chart, 2);
    measureCard->setMinimumWidth(420);
    topInfo->addWidget(measureCard, 3);
    progressLayout->addLayout(topInfo);

    tabs->addTab(progressTab, QIcon::fromTheme("view-statistics", style()->standardIcon(QStyle::SP_DirHomeIcon)), "История измерений");

    // Tab 2: Workouts and add
    QWidget* workoutsTab = new QWidget(this);
    QVBoxLayout* workoutLayout = new QVBoxLayout(workoutsTab);
    workoutLayout->setSpacing(14);

    QHBoxLayout* planPanels = new QHBoxLayout();
    planPanels->setSpacing(12);

    // Auto plan
    QWidget* autoCard = new QWidget(this);
    autoCard->setObjectName("card");
    QVBoxLayout* autoLayout = new QVBoxLayout(autoCard);
    autoLayout->setSpacing(8);
    QLabel* autoTitle = new QLabel("Автогенерация плана", this);
    autoTitle->setStyleSheet("font-weight: 700; color: #0f2f2f; font-size: 15px;");

    planView = new QTextEdit(this);
    planView->setReadOnly(true);
    planView->setMinimumHeight(240);
    planView->setLineWrapMode(QTextEdit::WidgetWidth);
    planView->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    planView->setStyleSheet("background: rgba(255,255,255,0.95); border-radius: 12px; padding: 12px;");

    QPushButton* generateBtn = new QPushButton("Сгенерировать план", this);
    generateBtn->setObjectName("filledButton");
    generateBtn->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    connect(generateBtn, &QPushButton::clicked, this, &DashboardWindow::generatePlan);

    autoLayout->addWidget(autoTitle);
    autoLayout->addWidget(planView);
    autoLayout->addWidget(generateBtn, 0, Qt::AlignLeft);

    // Manual plan
    QWidget* manualCard = new QWidget(this);
    manualCard->setObjectName("card");
    QVBoxLayout* manualLayout = new QVBoxLayout(manualCard);
    manualLayout->setSpacing(8);
    QLabel* manualTitle = new QLabel("Ручной план", this);
    manualTitle->setStyleSheet("font-weight: 700; color: #0f2f2f; font-size: 15px;");

    workoutsHistory = new QTextEdit(this);
    workoutsHistory->setReadOnly(true);
    workoutsHistory->setMinimumHeight(240);
    workoutsHistory->setLineWrapMode(QTextEdit::WidgetWidth);
    workoutsHistory->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    workoutsHistory->setPlaceholderText("Добавьте упражнения вручную или храните заметки по тренировкам...");
    workoutsHistory->setStyleSheet("background: rgba(255,255,255,0.95); border-radius: 12px; padding: 12px;");

    QPushButton* addManualBtn = new QPushButton("Добавить тренировку вручную", this);
    addManualBtn->setObjectName("ghostButton");
    addManualBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    connect(addManualBtn, &QPushButton::clicked, this, [this]() {
        bool ok = false;
        QString text = QInputDialog::getMultiLineText(this, "Добавить тренировку", "Описание", "", &ok);
        if (ok && !text.trimmed().isEmpty())
        {
            workoutsHistory->append("• " + text.trimmed());
        }
    });

    manualLayout->addWidget(manualTitle);
    manualLayout->addWidget(workoutsHistory);
    manualLayout->addWidget(addManualBtn, 0, Qt::AlignLeft);

    planPanels->addWidget(autoCard, 1);
    planPanels->addWidget(manualCard, 1);

    workoutLayout->addLayout(planPanels);

    tabs->addTab(workoutsTab, QIcon::fromTheme("sports", style()->standardIcon(QStyle::SP_ArrowForward)), "Тренировки");

    QPushButton* logoutBtn = new QPushButton("Выйти", this);
    logoutBtn->setObjectName("ghostButton");
    logoutBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    logoutBtn->setFixedWidth(120);
    connect(logoutBtn, &QPushButton::clicked, this, &DashboardWindow::logout);

    root->addWidget(titleLabel);
    root->addWidget(latestMeasurementLabel);
    root->addWidget(tabs);
    root->addStretch();
    root->addWidget(logoutBtn, 0, Qt::AlignRight);

    setCentralWidget(central);
    setMinimumSize(900, 600);
}

void DashboardWindow::applyBranding()
{
    setStyleSheet(R"(
        QWidget {
            background: #f8f1d4;
            color: #0f2f2f;
            font-family: 'Helvetica Neue', 'Arial';
            selection-background-color: transparent;
            selection-color: #0f2f2f;
        }
        QWidget#card {
            background: rgba(255,255,255,0.88);
            border-radius: 16px;
            border: 1px solid rgba(15,47,47,0.12);
            padding: 16px;
        }
        QLineEdit {
            padding: 4px 8px;
            border-radius: 8px;
            border: 1px solid rgba(15,47,47,0.35);
            background: rgba(255,255,255,0.96);
            font-size: 14px;
            min-width: 260px;
        }
        QLineEdit:focus { border: 1px solid #0f2f2f; }
        QPushButton#filledButton {
            background: #0f2f2f;
            color: #f8f1d4;
            padding: 10px 14px;
            border-radius: 12px;
            border: none;
            font-weight: 600;
        }
        QPushButton#filledButton:hover { background: #144040; }
        QPushButton#ghostButton {
            background: transparent;
            color: #0f2f2f;
            padding: 10px 14px;
            border-radius: 12px;
            border: 1px solid rgba(15,47,47,0.35);
            font-weight: 600;
        }
        QPushButton#ghostButton:hover { background: rgba(15,47,47,0.08); }
        QTabBar::tab {
            padding: 10px 16px;
            margin-right: 8px;
            background: rgba(255,255,255,0.65);
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
        }
        QLabel {
        background: transparent;
        }
        QTabBar::tab:selected {
            background: #0f2f2f;
            color: #f8f1d4;
        }
    )");

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#f8f1d4"));
    pal.setColor(QPalette::Base, QColor("#f8f1d4"));
    pal.setColor(QPalette::AlternateBase, QColor("#f8f1d4"));
    setPalette(pal);
    setAutoFillBackground(true);
}

void DashboardWindow::refreshUserSummary()
{
    if (!user)
    {
        return;
    }
    QString greeting = QString("Привет, %1").arg(QString::fromStdString(user->getName()));
    titleLabel->setText(greeting);

    Measurement latest = user->getMeasurements().getCurrentMeasurement();
    if (latest.date.year != 0)
    {
        latestMeasurementLabel->setText(
            QString("Последние измерения: %1 кг, %2 см. Дата: %3-%4-%5")
                .arg(latest.weight, 0, 'f', 1)
                .arg(latest.height, 0, 'f', 1)
                .arg(latest.date.year)
                .arg(latest.date.month, 2, 10, QChar('0'))
                .arg(latest.date.day, 2, 10, QChar('0'))
        );

        weightEdit->setText(QString::number(latest.weight, 'f', 1));
        heightEdit->setText(QString::number(latest.height, 'f', 1));
        chestEdit->setText(QString::number(latest.chest, 'f', 1));
        waistEdit->setText(QString::number(latest.waist, 'f', 1));
        hipsEdit->setText(QString::number(latest.hips, 'f', 1));
    }
    else
    {
        latestMeasurementLabel->setText("Последние измерения отсутствуют.");
    }

    updateBmiInfo(latest);
    refreshHistory();
}

Measurement DashboardWindow::readMeasurementInputs() const
{
    Measurement m{};
    bool ok = true;
    m.weight = weightEdit->text().toDouble(&ok); if (!ok) throw std::runtime_error("Укажите вес.");
    m.height = heightEdit->text().toDouble(&ok); if (!ok) throw std::runtime_error("Укажите рост.");
    m.chest = chestEdit->text().toDouble(&ok); if (!ok) m.chest = 0.0;
    m.waist = waistEdit->text().toDouble(&ok); if (!ok) m.waist = 0.0;
    m.hips = hipsEdit->text().toDouble(&ok); if (!ok) m.hips = 0.0;
    QDate today = QDate::currentDate();
    m.date = {today.day(), today.month(), today.year()};
    return m;
}

void DashboardWindow::saveMeasurement()
{
    if (!user)
    {
        return;
    }
    try
    {
        Measurement m = readMeasurementInputs();
        userService->saveMeasurement(user.get(), m);
        QMessageBox::information(this, "Сохранено", "Измерения сохранены.");
        refreshUserSummary();
        refreshHistory();
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(this, "Ошибка", ex.what());
    }
}

void DashboardWindow::generatePlan()
{
    if (!user)
    {
        return;
    }
    try
    {
        Measurement m = readMeasurementInputs();
        double bmi = BMICalculator::calculateBMI(m.weight, m.height);
        std::string fitness = BMICalculator::getFitnessLevel(bmi, user->getGoal());
        WorkoutPlan plan = workoutService->generatePlan(user.get(), bmi, fitness, m);

        std::stringstream ss;
        ss << "Кардио:\n";
        for (const CardioTraining* c : plan.cardio)
        {
            ss << " • " << c->getExerciseName() << " — " << c->getDuration() << " минут\n";
        }
        ss << "\nВерх:\n";
        for (const UpperBodyWorkout* u : plan.upperBody)
        {
            ss << " • " << u->getExerciseName() << " (" << u->getUpperBodyPart() << "), "
               << u->getSets() << "x" << u->getReps() << "\n";
        }
        ss << "\nНиз:\n";
        for (const LowerBodyWorkout* l : plan.lowerBody)
        {
            ss << " • " << l->getExerciseName() << " (" << l->getLowerBodyPart() << "), "
               << l->getSets() << "x" << l->getReps() << "\n";
        }

        planView->setPlainText(QString::fromStdString(ss.str()));
        workoutsHistory->append("Автоплан создан");
    }
    catch (const InputException& ex)
    {
        QMessageBox::warning(this, "Ошибка плана", ex.what());
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(this, "Ошибка", ex.what());
    }
}

void DashboardWindow::logout()
{
    emit logoutRequested();
}

std::vector<Measurement> DashboardWindow::loadMeasurements() const
{
    std::vector<Measurement> result;
    if (!user)
    {
        return result;
    }
    std::string path = "measurements_" + std::to_string(user->getId()) + ".txt";
    TextFile<Measurement> file(path);
    std::vector<Measurement*> data = file.readAllRecords();
    for (Measurement* m : data)
    {
        if (m)
        {
            result.push_back(*m);
            delete m;
        }
    }
    return result;
}

void DashboardWindow::refreshHistory()
{
    chart->setMeasurements(loadMeasurements());
}

void DashboardWindow::updateBmiInfo(const Measurement& latest)
{
    if (!bmiInfoLabel)
    {
        return;
    }
    if (latest.date.year == 0 || !user)
    {
        bmiInfoLabel->setText("Нет данных для ИМТ.");
        return;
    }
    double bmi = BMICalculator::calculateBMI(latest.weight, latest.height);
    std::string fitness = BMICalculator::getFitnessLevel(bmi, user->getGoal());
    QString info = QString("ИМТ: %1 (%2) • Цель: %3")
        .arg(bmi, 0, 'f', 1)
        .arg(QString::fromStdString(fitness))
        .arg(QString::fromStdString(user->getGoal()));
    bmiInfoLabel->setText(info);
}
