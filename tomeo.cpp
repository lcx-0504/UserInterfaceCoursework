#include <QApplication>
#include "customwindow.h"

int main(int argc, char *argv[]) {
    // Create the Qt Application
    QApplication app(argc, argv);

    // Create and show the custom window
    CustomWindow window(nullptr, argv, argc);
    window.show();

    // Execute the application
    return app.exec();
}
