#include "Proprieties.h"

Proprieties::Proprieties()
{

}

Proprieties::~Proprieties()
{
    for(unsigned i = 0; i < list.size(); ++i)
        delete list[i];
}

void Proprieties::Draw(sf::RenderWindow *window, sf::Font *font)
{

}

bool Proprieties::Load(StringList content)
{
    if(content.size() == 0 || content.size() % 2 || content.size() / 2 != list.size())
        return false;
    for(unsigned i = 0; i < content.size(); i += 2)
    {
        if(content[i] != list[i/2]->name)
            return false;
        if(!list[i/2]->Set(content[i+1]))
            return false;
    }
    return true;
}

String Proprieties::ToString()
{
    std::stringstream s;
    for(unsigned i = 0; i < list.size(); i++)
    {
        s << list[i]->name << " " << list[i]->ValueToString();
        if(i != list.size() - 1)
            s << " ";
    }
    return s.str();
}
