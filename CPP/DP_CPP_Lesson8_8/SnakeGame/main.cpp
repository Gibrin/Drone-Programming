#include <QApplication>
#include "snake.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Snake w; w.show();  // создать окно и показать
    return app.exec();  // запустить цикл обработки событий
}