#pragma once
#include <QWidget>
#include <QList>
#include <QPoint>
#include <QColor>

class Snake : public QWidget {
    Q_OBJECT
public:
    explicit Snake(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void timerEvent(QTimerEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private:
    // Константы игры
    static constexpr int DOT_SIZE = 10;      // размер сегмента в пикселях
    static constexpr int W_DOTS = 40, H_DOTS = 30;  // поле в «клетках»
    static constexpr int DELAY = 140;        // задержка между кадрами (мс)

    // Состояние игры
    QList<QPoint> snake;                     // сегменты змеи: [0] — голова
    QPoint apple;                            // позиция яблока
    enum class Dir { L, R, U, D } dir = Dir::R;  // направление движения
    bool inGame = true;                      // флаг: игра активна
    int timerId = -1, score = 0;             // таймер и счёт

    // Цвета для отрисовки
    const QColor head{144,238,144}, body{0,255,0}, appleC{Qt::red}, bg{Qt::black};

    // Обязательные методы по заданию
    void initGame();           // сброс состояния, старт игры
    void locateApple();        // разместить яблоко в случайной свободной клетке
    void move();               // переместить змею на шаг вперёд
    bool checkCollision();     // проверить столкновение со стеной или телом
    
    // Вспомогательные
    void drawGame(QPainter&);  // отрисовка игрового процесса
    void drawOver(QPainter&);  // экран Game Over
};