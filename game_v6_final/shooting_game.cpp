#include "shooting_game.h"
#include <QDebug>

/* default constructor that creates a graphics scene with a player p, scoreboard, and then sets a timer to update_scoreboard every 50ms
 * @param N/A
 * @return N/A
 */
shooting_game::shooting_game()
{
    scene = new QGraphicsScene(QRect(0, 0, 600, 400));
    scene->addItem(p);
    p->setPos(400,500);
    scene->addItem(scoreboard);
    scoreboard->setTextInteractionFlags(Qt::TextEditable);
    scoreboard->setFont(*f);
    setScene(scene);
    this->setFixedSize(800, 700);
    connect(timer,SIGNAL(timeout()),this,SLOT(update_scoreboard()));
    timer->start(50);

}

/* function that updates the scoreboard with the current player's score
 * @param N/A
 * @return N/A
 */
void shooting_game::update_scoreboard(){
    scoreboard->setPlainText(QString("Score: ") + QString::number(p->score));
    if(p->score==20){        //player meets criteria for winning score
        emit won_game();
        p->timer->stop();
    }

    if(p->lost){            //player has lost
        emit lose_game();
        p->lost = false;
        p->score = 0;
        p->delete_enemies = true;
        p->timer->stop();
    }
}

/* keyPressEvent - function that moves p position when left and right keys are pressed, and creates a moving bullet when spacebar is pressed
 * @param N/A
 * @return N/A
 */
void shooting_game::keyPressEvent(QKeyEvent *event){
        if (event->key() == Qt::Key_Left){      //if left key is pressed, move player left
            if (p->pos().x() > -110)
               p->setPos(p->pos().x()-10,p->pos().y());
        }
        else if (event->key() == Qt::Key_Right){ //if right key is pressed, move player right
             if (p->pos().x() < 660)
               p->setPos(p->pos().x()+10,p->pos().y());
        }
        else if(event->key()==Qt::Key_Space){   //if spacebar is pressed, create new bullet and add to scene
            bullet * b = new bullet(p);
            b->setPos(p->pos().x()-25,p->pos().y()-25);
            scene->addItem(b);
        }
}

/* destructor of shooting gaem
 * @param N/A
 * @return N/A
 */
shooting_game::~shooting_game(){
    delete scene;
    delete timer;
    delete f;
}

/* constructor of player; creates image of a ship and calls spawn when timer is out
 * @param N/A
 * @return N/A
 */
player::player()
{
    setPixmap(QPixmap (":/ship.png").scaled(50,50));
    QObject::connect(timer,SIGNAL(timeout()), this,SLOT(spawn()));
}

/* spawn- function that creates a new enemy and adds to scene
 * @param N/A
 * @return N/A
 */
void player::spawn(){
    enemy* e = new enemy(this);
    scene()->addItem(e);
}

/* constructor of a bullet- creates an image of a bullet and calls move every 50ms
 * @param player_1 is the 'owner' of this bullet
 * @return N/A
 */
bullet::bullet( player* p ):owner(p)
{
    setPixmap(QPixmap (":/bullet.png").scaled(100,100));
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(50);
}

/* move - function that moves the position of the bullet up, or deletes it if it collides with an enemy or moves off the screen
 * @param N/A
 * @return N/A
 */
void bullet::move(){
    setPos(this->pos().x() ,pos().y()-20);  //move bullet up by 20
    QList<QGraphicsItem*> colliding_items=this->collidingItems();
        for(int i = 0; i < colliding_items.size(); ++i){    //iterates through list of collidingItems() with this bullet
            if(typeid(*(colliding_items[i])) == typeid(enemy))  //if bullet collides with an enemy delete both objects and free memory
            {
                    owner->score++;
                    scene()->removeItem(colliding_items[i]);
                    scene()->removeItem(this);
                    delete colliding_items[i];
                    delete this;
                    return;
            }
        }
    if (pos().y() < 0){                     //if off screen-remove bullet from scene and delete
            scene()->removeItem(this);
            delete this;
        }
}

/* constructor of an enemy, which creates the image of an enemy and sets it in the scene at a random x position, then calls move every 50ms
 * @param play is the owner of this enemy
 * @return N/A
 */
enemy::enemy(player* play): p(play)
{
    int random_number = rand() % 700;
    setPos(random_number,0);
    setPixmap(QPixmap(":/enemy.png").scaled(20,20));
    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(50);
}

/* function that moves the enemy down the screen, deletes them when the game is over, and checks if collided with the player
 * @param N/A
 * @return N/A
 * */
void enemy::move(){
    if(p->delete_enemies){      //if delete_enemies is true ( when game lost) - remove enemy and delete
           scene()->removeItem(this);
           delete this;
           return;
    }


    QList<QGraphicsItem*> colliding_items=this->collidingItems();
       for(int i = 0; i < colliding_items.size(); ++i){    //iterates through list of colliding items with enemy
           if(typeid(*(colliding_items[i])) == typeid(player))     //if item colliding with this enemy is a player, remove both from scene and free memory
           {
                   p->lost= true;
                   scene()->removeItem(this);
                   p->setPos(900,p->pos().y());
                   delete this;
                   return;
           }
       }


    if (pos().y() > 700){ //if enemy is off screen
            scene()->removeItem(this);
            delete this;
        }

    setPos(x(),y()+5);
}

