#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>
#include <QEvent>

#include "mainwindow.h"

static constexpr int MIN_SPAWN_DELAY_MS = 100;
static constexpr int MAX_SPAWN_DELAY_MS = 1000;

static constexpr int MIN_FALL_SPEED = 1;
static constexpr int MAX_FALL_SPEED = 5;

static constexpr int MOVE_TIMEOUT_MS = 50;

static constexpr int MAX_SPAWN_Y = 100;
static constexpr int SNOWFLAKE_WIDTH = 50;
static constexpr int SNOWFLAKE_HEIGHT = 50;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, this, [timer, this] {
        const int spawnTimeout = QRandomGenerator::global()->bounded(MIN_SPAWN_DELAY_MS,
                                                                 MAX_SPAWN_DELAY_MS);
        const int fallSpeed = QRandomGenerator::global()->bounded(MIN_FALL_SPEED, MAX_FALL_SPEED);

        QPushButton* snowflake = new QPushButton("*", this);
        const int randomX = QRandomGenerator::global()->bounded(0, this->width() - SNOWFLAKE_WIDTH);
        const int randomY = QRandomGenerator::global()->bounded(0, MAX_SPAWN_Y);

        snowflake->setStyleSheet("QPushButton { background-color: transparent; "
                                "border: none; font-weight: bold;"
                                "font-size: 48px; }");

        snowflake->setGeometry(QRect(QPoint(randomX, randomY), QSize(SNOWFLAKE_WIDTH, SNOWFLAKE_HEIGHT)));
        snowflake->setProperty("fallSpeed", fallSpeed);
        snowflake->show();

        connect(snowflake,  &QPushButton::clicked, this, [snowflake] {
            snowflake->deleteLater();
        });

        QTimer* moveTimer = new QTimer(snowflake);

        connect(moveTimer, &QTimer::timeout, this, [snowflake, this] {
            int moveSpeed = snowflake->property("fallSpeed").toInt();

            if (snowflake->underMouse()) {
                moveSpeed *= 2;
            }

            snowflake->move(QPoint(snowflake->x(),
                                   snowflake->y() + moveSpeed));
            const int maxY = this->height() - snowflake->height();
            const int maxX = this->width() - snowflake->width();

            if (snowflake->x() >= maxX) {
                snowflake->deleteLater();
                return;
            }

            if (snowflake->y() < maxY) {
                return;
            }

            this->setStyleSheet("QMainWindow { background-color: red; }");
            this->setWindowTitle("You LOSE!");
            snowflake->deleteLater();
        });

        moveTimer->start(MOVE_TIMEOUT_MS);

        timer->start(spawnTimeout);
    });

    timer->start(MAX_SPAWN_DELAY_MS);
}

MainWindow::~MainWindow()
{

}