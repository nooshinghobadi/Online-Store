#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "database.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize the database
    if (!Database::initialize()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to initialize the database. Exiting the application.");
        return -1; // Exit if database initialization fails
    }

    // Create and show the main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Advanced Online Store Simulation");
    mainWindow.show();

    // Run the application
    return app.exec();
}
