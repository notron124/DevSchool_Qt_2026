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

static constexpr int SPAWN_Y = 100;
static constexpr int SNOWFLAKE_WIDTH = 50;
static constexpr int SNOWFLAKE_HEIGHT = 50;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, this, [timer, this] {
        int spawnTimeout = QRandomGenerator::global()->bounded(MIN_SPAWN_DELAY_MS,
                                                                 MAX_SPAWN_DELAY_MS);
        int fallSpeed = QRandomGenerator::global()->bounded(MIN_FALL_SPEED, MAX_FALL_SPEED);

        QPushButton* snowflake = new QPushButton("*", this);
        int randomX = QRandomGenerator::global()->bounded(0, this->width() - SNOWFLAKE_WIDTH);
        snowflake->setGeometry(QRect(QPoint(randomX, SPAWN_Y), QSize(SNOWFLAKE_WIDTH, SNOWFLAKE_HEIGHT)));
        snowflake->setProperty("fallSpeed", fallSpeed);
        snowflake->show();
        snowflake->installEventFilter(this);

        connect(snowflake,  &QPushButton::clicked, this, [snowflake] {
            snowflake->deleteLater();
        });

        QTimer* moveTimer = new QTimer(snowflake);

        connect(moveTimer, &QTimer::timeout, this, [snowflake, this]{
            snowflake->move(QPoint(snowflake->x(),
                                   snowflake->y() + snowflake->property("fallSpeed").toInt()));
            int maxY = this->height() - snowflake->height();
            if (snowflake->y() >= maxY) {
                this->setStyleSheet("QMainWindow { background-color: red; }");
                this->setWindowTitle("You LOSE!");
                snowflake->deleteLater();
            }
        });

        moveTimer->start(MOVE_TIMEOUT_MS);

        timer->start(spawnTimeout);
    });

    timer->start(MAX_SPAWN_DELAY_MS);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    int fallSpeed = obj->property("fallSpeed").toInt();

    switch(event->type()) {
    case QEvent::Enter:
        obj->setProperty("fallSpeed", fallSpeed * 2);
        return true;

    case QEvent::Leave:
        obj->setProperty("fallSpeed", fallSpeed / 2);
        return true;

    default:
        break;
    }

    return QWidget::eventFilter(obj, event);
}
