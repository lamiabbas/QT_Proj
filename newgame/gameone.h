#ifndef GAMEONE_H
#define GAMEONE_H
#include <QWidget>
#include <QPushButton>
#include <QLayout>
class gameone: public QWidget
{
public:
    gameone();
    QPushButton* back_button = new QPushButton("back to main screen");
    QPushButton* win_game_button = new QPushButton("win game -> advance to next stage");
    QHBoxLayout* layoutt = new QHBoxLayout;
    ~gameone();
};

#endif // GAMEONE_H
