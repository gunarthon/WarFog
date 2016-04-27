#ifndef PROPRIETIES_H
#define PROPRIETIES_H

#include "Utils.h"
#include "Propriety.h"

class Proprieties
{
    public:
        Proprieties();
        virtual ~Proprieties();
        void Draw(sf::RenderWindow *window, sf::Font *font);
        String ToString();
        bool Load(StringList content);

        ProprietyList list;
    protected:

    private:
};

#endif // PROPRIETIES_H
