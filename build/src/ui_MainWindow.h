/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_Chat;
    QAction *actionSettings;
    QAction *actionExit;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QWidget *sidebarWidget;
    QVBoxLayout *sidebarLayout;
    QLabel *logoLabel;
    QPushButton *newChatButton;
    QListWidget *chatHistoryList;
    QPushButton *settingsButton;
    QWidget *chatWidget;
    QVBoxLayout *chatLayout;
    QTextEdit *chatDisplay;
    QHBoxLayout *inputLayout;
    QTextEdit *messageInput;
    QPushButton *sendButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(900, 600);
        actionNew_Chat = new QAction(MainWindow);
        actionNew_Chat->setObjectName(QString::fromUtf8("actionNew_Chat"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        sidebarWidget = new QWidget(splitter);
        sidebarWidget->setObjectName(QString::fromUtf8("sidebarWidget"));
        sidebarWidget->setMinimumSize(QSize(200, 0));
        sidebarWidget->setMaximumSize(QSize(250, 16777215));
        sidebarLayout = new QVBoxLayout(sidebarWidget);
        sidebarLayout->setObjectName(QString::fromUtf8("sidebarLayout"));
        logoLabel = new QLabel(sidebarWidget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        logoLabel->setFont(font);

        sidebarLayout->addWidget(logoLabel);

        newChatButton = new QPushButton(sidebarWidget);
        newChatButton->setObjectName(QString::fromUtf8("newChatButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/new_chat.png"), QSize(), QIcon::Normal, QIcon::Off);
        newChatButton->setIcon(icon);

        sidebarLayout->addWidget(newChatButton);

        chatHistoryList = new QListWidget(sidebarWidget);
        chatHistoryList->setObjectName(QString::fromUtf8("chatHistoryList"));

        sidebarLayout->addWidget(chatHistoryList);

        settingsButton = new QPushButton(sidebarWidget);
        settingsButton->setObjectName(QString::fromUtf8("settingsButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsButton->setIcon(icon1);

        sidebarLayout->addWidget(settingsButton);

        splitter->addWidget(sidebarWidget);
        chatWidget = new QWidget(splitter);
        chatWidget->setObjectName(QString::fromUtf8("chatWidget"));
        chatLayout = new QVBoxLayout(chatWidget);
        chatLayout->setObjectName(QString::fromUtf8("chatLayout"));
        chatDisplay = new QTextEdit(chatWidget);
        chatDisplay->setObjectName(QString::fromUtf8("chatDisplay"));
        chatDisplay->setReadOnly(true);

        chatLayout->addWidget(chatDisplay);

        inputLayout = new QHBoxLayout();
        inputLayout->setObjectName(QString::fromUtf8("inputLayout"));
        messageInput = new QTextEdit(chatWidget);
        messageInput->setObjectName(QString::fromUtf8("messageInput"));
        messageInput->setMaximumSize(QSize(16777215, 100));

        inputLayout->addWidget(messageInput);

        sendButton = new QPushButton(chatWidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        sendButton->setIcon(icon2);
        sendButton->setMinimumSize(QSize(80, 40));

        inputLayout->addWidget(sendButton);


        chatLayout->addLayout(inputLayout);

        splitter->addWidget(chatWidget);

        verticalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew_Chat);
        menuFile->addSeparator();
        menuFile->addAction(actionSettings);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "PiChat - AI Chat Assistant", nullptr));
        actionNew_Chat->setText(QCoreApplication::translate("MainWindow", "New Chat", nullptr));
        actionSettings->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        logoLabel->setText(QCoreApplication::translate("MainWindow", "PiChat", nullptr));
        newChatButton->setText(QCoreApplication::translate("MainWindow", "New Chat", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        chatDisplay->setStyleSheet(QCoreApplication::translate("MainWindow", "QTextEdit { background-color: #1e1e1e; border: 1px solid #5c5c5c; border-radius: 4px; }", nullptr));
        messageInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type your message here...", nullptr));
        messageInput->setStyleSheet(QCoreApplication::translate("MainWindow", "QTextEdit { background-color: #2d2d2d; border: 1px solid #5c5c5c; border-radius: 4px; }", nullptr));
        sendButton->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        sendButton->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton { background-color: #2a82da; color: white; border-radius: 4px; }", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
