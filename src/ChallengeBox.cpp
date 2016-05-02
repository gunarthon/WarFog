#include "ChallengeBox.h"

ChallengeBox::ChallengeBox(sf::Vector2f pos, sf::Vector2f size) : Rectangle(pos.x, pos.y, size.x, size.y)
{
    collide = false;
    win = false;
    loose = false;
    density = -5.0;
    this->color = Color(178,0,255,100);
    this->partId = -1;
    this->keepIn = true;
    this->strict = true;
    this->toWin = true;
    this->memory = false;
    proprieties.list.clear();

    proprieties.list.push_back(new Propriety("id", true, (int*) &id));
    proprieties.list.push_back(new Propriety("static", BOOL, 1, true, &isStatic));
    proprieties.list.push_back(new Propriety("outline", BOOL, 1, true, &outline));
    //proprieties.list.push_back(new Propriety("terrain", BOOL, 0, &terrain));
    //proprieties.list.push_back(new Propriety("undragable", BOOL, 0, &undragable));
    proprieties.list.push_back(new Propriety("angle", SLIDER, 1, true, NULL, &angle, NULL, -360, 360));
    proprieties.list.push_back(new Propriety("color.a", SLIDER, 1, true, NULL, &color.a, NULL, 0, 255));

    proprieties.list.push_back(new Propriety("centerX", SLIDER, 1, false, NULL, &centerX, NULL));
    proprieties.list.push_back(new Propriety("centerY", SLIDER, 1, false, NULL, &centerY, NULL));


    proprieties.list.push_back(new Propriety("x", SLIDER, 1,  false, NULL, &x, NULL));
    proprieties.list.push_back(new Propriety("y", SLIDER, 1,  false, NULL, &y, NULL));
    proprieties.list.push_back(new Propriety("width", SLIDER, 1,  false, NULL, &w, NULL));
    proprieties.list.push_back(new Propriety("height", SLIDER, 1,  false, NULL, &h, NULL));

    proprieties.list.push_back(new Propriety("partId", POINTER, -1, true, NULL, &partId));
    proprieties.list.push_back(new Propriety("partIndex", POINTER, -1, false, NULL, &partIndex));

    proprieties.list.push_back(new Propriety("keepIn", BOOL, 1, true, &keepIn));
    proprieties.list.push_back(new Propriety("strict", BOOL, 1, true, &strict));
    proprieties.list.push_back(new Propriety("toWin", BOOL, 1, true, &toWin));
    proprieties.list.push_back(new Propriety("memory", BOOL, 1, true, &memory));

    type = "ChallengeBox";
}

ChallengeBox::~ChallengeBox()
{

}

void ChallengeBox::Update(b2World *world, InputManager *input)
{
    if(!memory)
    {
        win = false;
        loose = false;
    }

    ShapePart *part = (ShapePart*) allParts->At(partId);
    if(!part)
        return;
    VectorList verts;
    part->GetVertices(&verts);

    float signal = 0;
    for(unsigned i = 0; i < verts.size(); i++)
        if(InsideShape(sf::Vector2f(verts[i].x, verts[i].y)))
            signal++;
    signal /= verts.size();

    if(keepIn)
    {
        if(signal == 1 || (signal > 0 && !strict))
        {
            if(toWin)
                win = true;
            else
                loose = true;
        }
    }
    else
    {
        if(signal == 0 || (signal < 1 && !strict))
        {
            if(toWin)
                win = true;
            else
                loose = true;
        }
    }

    if(loose)
        color = Color(255,0,10,100);
    else if(win)
        color = Color(55,255,10,100);
    else
        color = Color(178,0,255,100);

}

bool ChallengeBox::GetOutput(int index)
{
    return win || loose;
}

String ChallengeBox::ToString()
{
    partIndex = allParts->IndexOf(allParts->At(partId));

    return Part::ToString();
}

void ChallengeBox::UnInit(b2World *world)
{
	if(!isInitted)
        return;
    Rectangle::UnInit(world);
    loose = false;
    win = false;
    color = Color(178,0,255,100);
}
