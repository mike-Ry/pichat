
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Qt Test Window");
    mainWindow.resize(400, 300);
    
    QWidget* centralWidget = new QWidget(&mainWindow);
    mainWindow.setCentralWidget(centralWidget);
    
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    
    QLabel* label = new QLabel("Qt is working!", centralWidget);
    layout->addWidget(label);
    
    QPushButton* button = new QPushButton("Quit", centralWidget);
    layout->addWidget(button);
    
    QObject::connect(button, &QPushButton::clicked, &app, &QApplication::quit);
    
    mainWindow.show();
    return app.exec();
}
