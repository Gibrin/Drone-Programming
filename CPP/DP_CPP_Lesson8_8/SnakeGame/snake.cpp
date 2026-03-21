#include "snake.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>

Snake::Snake(QWidget *parent) : QWidget(parent) {
    setFixedSize(W_DOTS * DOT_SIZE, H_DOTS * DOT_SIZE);
    setWindowTitle("🐍 Змейка");
    initGame();
}

void Snake::initGame() {
    inGame = true; score = 0; dir = Dir::R; snake.clear();
    for (int i = 0; i < 3; ++i) snake.append({20 - i, 15});
    locateApple();
    if (timerId != -1) killTimer(timerId);
    timerId = startTimer(DELAY);
}

void Snake::locateApple() {
    do {
        apple = {QRandomGenerator::global()->bounded(W_DOTS),
                 QRandomGenerator::global()->bounded(H_DOTS)};
    } while (snake.contains(apple));
}

void Snake::move() {
    QPoint head = snake.first();
    switch (dir) {
        case Dir::L: head.rx()--; break;
        case Dir::R: head.rx()++; break;
        case Dir::U: head.ry()--; break;
        case Dir::D: head.ry()++; break;
    }
    snake.prepend(head);
    (head == apple) ? (++score, locateApple()) : snake.removeLast();
    setWindowTitle(QString("🐍 Счёт: %1").arg(score));
}

bool Snake::checkCollision() {
    QPoint head = snake.first();
    int x = head.x(), y = head.y();
    if (x < 0 || x >= W_DOTS || y < 0 || y >= H_DOTS) return true;
    for (int i = 1; i < snake.size(); ++i)
        if (head == snake[i]) return true;
    return false;
}

void Snake::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);
    if (!inGame) return;
    move();
    if (checkCollision()) { inGame = false; killTimer(timerId); }
    repaint();
}

void Snake::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), bg);
    inGame ? drawGame(p) : drawOver(p);
}

void Snake::drawGame(QPainter &p) {
    p.setPen(Qt::NoPen);
    p.setBrush(appleC);
    p.drawRect(apple.x() * DOT_SIZE, apple.y() * DOT_SIZE, DOT_SIZE, DOT_SIZE);
    for (int i = 0; i < snake.size(); ++i) {
        p.setBrush(i == 0 ? head : body);
        QPoint pt = snake[i];
        p.drawRect(pt.x() * DOT_SIZE, pt.y() * DOT_SIZE, DOT_SIZE, DOT_SIZE);
    }
}

void Snake::drawOver(QPainter &p) {
    p.setPen(Qt::white);
    p.setFont({"Arial", 20, QFont::Bold});
    p.drawText(rect(), Qt::AlignCenter, "Game Over\nPress SPACE");
}

void Snake::keyPressEvent(QKeyEvent *e) {
    if (!inGame && e->key() == Qt::Key_Space) { initGame(); return; }
    if (!inGame) return;
    
    Dir nd = dir;
    switch (e->key()) {
        case Qt::Key_Left:  if (dir != Dir::R) nd = Dir::L; break;
        case Qt::Key_Right: if (dir != Dir::L) nd = Dir::R; break;
        case Qt::Key_Up:    if (dir != Dir::D) nd = Dir::U; break;
        case Qt::Key_Down:  if (dir != Dir::U) nd = Dir::D; break;
        default: return QWidget::keyPressEvent(e);
    }
    dir = nd;
}