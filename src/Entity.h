#ifndef ENTITY_H
#define ENTITY_H

class Entity {
    private:
        int posX;
        int posY;

        int health;
        int attackPower;
        int resistances[5]; // Physical, Poison, Ice, Fire, Soul
};

#endif