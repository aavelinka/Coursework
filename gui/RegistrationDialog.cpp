#include "RegistrationDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QMessageBox>
#include <QStyle>
#include "InputException.h"

RegistrationDialog::RegistrationDialog(UserService* userService, QWidget* parent)
    : QDialog(parent),
      userService(userService)
{
    setWindowTitle("Регистрация");
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(12);

    QLabel* title = new QLabel("Создайте аккаунт", this);
    title->setStyleSheet("font-weight: 700; color: #0f2f2f;");
    QFont f("Helvetica Neue", 16, QFont::Bold);
    title->setFont(f);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(8);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Имя пользователя");
    usernameEdit->addAction(style()->standardIcon(QStyle::SP_FileIcon), QLineEdit::LeadingPosition);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Пароль");
    passwordEdit->addAction(style()->standardIcon(QStyle::SP_FileDialogContentsView), QLineEdit::LeadingPosition);

    goalBox = new QComboBox(this);
    goalBox->addItems({"похудение", "набор мышечной массы", "поддержание формы"});

    weightEdit = new QLineEdit(this);
    weightEdit->setPlaceholderText("кг");

    heightEdit = new QLineEdit(this);
    heightEdit->setPlaceholderText("см");

    chestEdit = new QLineEdit(this);
    waistEdit = new QLineEdit(this);
    hipsEdit = new QLineEdit(this);

    form->addRow("Имя", usernameEdit);
    form->addRow("Пароль", passwordEdit);
    form->addRow("Цель", goalBox);
    form->addRow("Вес", weightEdit);
    form->addRow("Рост", heightEdit);
    form->addRow("Грудь", chestEdit);
    form->addRow("Талия", waistEdit);
    form->addRow("Бедра", hipsEdit);

    QPushButton* registerBtn = new QPushButton("Зарегистрироваться", this);
    registerBtn->setObjectName("filledButton");
    registerBtn->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(registerBtn, &QPushButton::clicked, this, &RegistrationDialog::handleRegister);

    root->addWidget(title);
    root->addLayout(form);
    root->addWidget(registerBtn, 0, Qt::AlignRight);

    applyBranding();
    setMinimumWidth(380);
}

void RegistrationDialog::applyBranding()
{
    setStyleSheet(R"(
        QDialog {
            background: #f8f1d4;
            color: #0f2f2f;
            font-family: 'Helvetica Neue', 'Arial';
        }
        QLineEdit {
            padding: 8px 10px;
            border-radius: 10px;
            border: 1px solid rgba(15,47,47,0.3);
            background: rgba(255,255,255,0.95);
        }
        QLineEdit:focus { border: 1px solid #0f2f2f; }
        QComboBox {
            padding: 8px 10px;
            border-radius: 10px;
            border: 1px solid rgba(15,47,47,0.3);
            background: rgba(255,255,255,0.95);
        }
        QLabel {
        background: transparent;
        }
        QPushButton#filledButton {
            background: #0f2f2f;
            color: #f8f1d4;
            padding: 10px 14px;
            border-radius: 12px;
            border: none;
            font-weight: 600;
        }
        QPushButton#filledButton:hover { background: #144040; }
    )");
}

std::unique_ptr<User> RegistrationDialog::takeUser()
{
    return std::move(createdUser);
}

Measurement RegistrationDialog::buildMeasurement() const
{
    Measurement m{};
    bool ok = true;
    m.weight = weightEdit->text().toDouble(&ok); if (!ok) throw std::runtime_error("Введите корректный вес.");
    m.height = heightEdit->text().toDouble(&ok); if (!ok) throw std::runtime_error("Введите корректный рост.");
    m.chest = chestEdit->text().toDouble(&ok); if (!ok) m.chest = 0.0;
    m.waist = waistEdit->text().toDouble(&ok); if (!ok) m.waist = 0.0;
    m.hips = hipsEdit->text().toDouble(&ok); if (!ok) m.hips = 0.0;
    QDate today = QDate::currentDate();
    m.date = {today.day(), today.month(), today.year()};
    return m;
}

void RegistrationDialog::handleRegister()
{
    try
    {
        const std::string username = usernameEdit->text().toStdString();
        const std::string password = passwordEdit->text().toStdString();
        const std::string goal = goalBox->currentText().toStdString();
        Measurement m = buildMeasurement();

        createdUser.reset(userService->registerUser(username, password, goal, m));
        QMessageBox::information(this, "Успех", "Аккаунт создан, добро пожаловать!");
        accept();
    }
    catch (const InputException& ex)
    {
        QMessageBox::warning(this, "Ошибка", ex.what());
    }
    catch (const std::exception& ex)
    {
        QMessageBox::warning(this, "Ошибка", ex.what());
    }
}
