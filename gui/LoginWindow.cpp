#include "LoginWindow.h"
#include "DashboardWindow.h"
#include "RegistrationDialog.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QIcon>
#include <QStyle>
#include <QMessageBox>
#include <QPalette>

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent)
{
    buildUi();
    applyBranding();
}

LoginWindow::~LoginWindow() = default;

void LoginWindow::buildUi()
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(48, 48, 48, 48);

    QLabel* logo = new QLabel("FITNESS-TO-GO", this);
    logo->setAlignment(Qt::AlignCenter);
    QFont logoFont("Avenir Next", 24, QFont::Bold);
    logoFont.setCapitalization(QFont::AllUppercase);
    logo->setFont(logoFont);
    logo->setStyleSheet("color: #0f2f2f;");

    QLabel* slogan = new QLabel("Лучше сегодня, чем никогда.", this);
    slogan->setAlignment(Qt::AlignCenter);
    QFont sloganFont("Helvetica Neue", 13, QFont::Light);
    sloganFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    slogan->setFont(sloganFont);
    slogan->setStyleSheet("color: rgba(15,47,47,0.85); margin-bottom: 24px;");

    QFrame* card = new QFrame(this);
    card->setObjectName("authCard");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(28, 32, 28, 32);
    cardLayout->setSpacing(16);

    QLabel* welcome = new QLabel("Добро пожаловать", this);
    welcome->setAlignment(Qt::AlignCenter);
    welcome->setStyleSheet("color: #0f2f2f;");
    QFont welcomeFont("Helvetica Neue", 18, QFont::Bold);
    welcome->setFont(welcomeFont);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(12);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Имя пользователя");
    usernameEdit->addAction(QIcon::fromTheme("user-identity", style()->standardIcon(QStyle::SP_FileIcon)), QLineEdit::LeadingPosition);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Пароль");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->addAction(QIcon::fromTheme("dialog-password", style()->standardIcon(QStyle::SP_FileDialogDetailedView)), QLineEdit::LeadingPosition);

    form->addRow("Имя", usernameEdit);
    form->addRow("Пароль", passwordEdit);

    QPushButton* loginButton = new QPushButton("Войти", this);
    loginButton->setObjectName("loginButton");
    loginButton->setCursor(Qt::PointingHandCursor);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    QLabel* registerLink = new QLabel(this);
    registerLink->setObjectName("registerLink");
    registerLink->setText("<a href='#'>Нет аккаунта? Зарегистрируйтесь сейчас!</a>");
    registerLink->setAlignment(Qt::AlignCenter);
    registerLink->setTextFormat(Qt::RichText);
    registerLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    registerLink->setOpenExternalLinks(false);
    connect(registerLink, &QLabel::linkActivated, this, [this](const QString&) { openRegistration(); });

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setStyleSheet("color: #b00020;");
    errorLabel->setVisible(false);

    cardLayout->addWidget(welcome);
    cardLayout->addLayout(form);
    cardLayout->addWidget(loginButton);
    cardLayout->addWidget(registerLink);
    cardLayout->addWidget(errorLabel);

    rootLayout->addWidget(logo);
    rootLayout->addWidget(slogan);
    rootLayout->addStretch();
    rootLayout->addWidget(card, 0, Qt::AlignCenter);
    rootLayout->addStretch();

    setCentralWidget(central);
    setMinimumSize(560, 520);
}

void LoginWindow::applyBranding()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#f8f1d4"));
    pal.setColor(QPalette::Base, QColor("#f8f1d4"));
    pal.setColor(QPalette::AlternateBase, QColor("#f8f1d4"));
    setPalette(pal);
    setAutoFillBackground(true);

    setStyleSheet(R"(
        QWidget {
            background: #f8f1d4;
            color: #0f2f2f;
            font-family: 'Helvetica Neue', 'Arial';
            selection-background-color: transparent;
            selection-color: #0f2f2f;
        }
        QFrame#authCard {
            background: rgba(255,255,255,0.85);
            border-radius: 18px;
            border: 1px solid rgba(15,47,47,0.12);
            box-shadow: 0 10px 30px rgba(15,47,47,0.10);
        }
        QLineEdit {
            padding: 0px 0px;
            border: 1px solid rgba(15,47,47,0.35);
            border-radius: 12px;
            background: transparent;
        }
        QLineEdit:focus {
            background: transparent;
            border: 1px solid #0f2f2f;
            outline: none;
        }
        QPushButton#loginButton {
            background: #0f2f2f;
            color: #f8f1d4;
            padding: 12px 16px;
            border: none;
            border-radius: 12px;
            font-weight: 600;
            letter-spacing: 0.3px;
        }
        QPushButton#loginButton:hover { background: #144040; }
        QLabel {
         background: transparent;
        }
        QLabel#registerLink {
            background: transparent;
            color: rgba(15,47,47,0.8);
            padding: 6px 0;
        }
        QLabel#registerLink:hover {
            background: transparent;
            color: #0f2f2f;
        }
    )");
}

void LoginWindow::handleLogin()
{
    errorLabel->setVisible(false);
    const std::string username = usernameEdit->text().toStdString();
    const std::string password = passwordEdit->text().toStdString();

    std::unique_ptr<User> user(userService.loginUser(username, password));
    if (!user)
    {
        showError("Неверное имя или пароль.");
        return;
    }
    openDashboard(std::move(user));
}

void LoginWindow::openRegistration()
{
    RegistrationDialog dialog(&userService, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        openDashboard(dialog.takeUser());
    }
}

void LoginWindow::openDashboard(std::unique_ptr<User> user)
{
    if (!user)
    {
        return;
    }
    if (!dashboard)
    {
        dashboard = std::make_unique<DashboardWindow>(&userService, &workoutService, std::move(user));
        dashboard->setAttribute(Qt::WA_DeleteOnClose, false);
    }
    else
    {
        dashboard->setUser(std::move(user));
    }

    dashboard->show();
    dashboard->raise();
    hide();
    connect(dashboard.get(), &DashboardWindow::logoutRequested, this, [this]() {
        show();
        dashboard->hide();
    });
}

void LoginWindow::showError(const QString& message)
{
    errorLabel->setText(message);
    errorLabel->setVisible(true);
    QMessageBox::warning(this, "Ошибка входа", message);
}
