#include "PartList.h"
#include "Part.h"
#include "Rectangle.h"
#include "Circle.h"
#include "FixedJoint.h"
#include "RevoluteJoint.h"
#include "Thruster.h"
#include "ChallengeBox.h"

PartList::PartList()
{
    groupList.push_back(&selected);
    groupList.push_back(&loaded);
    groupList.push_back(&candidate);
    groupList.push_back(&temp);
    groupList.push_back(&drawOrder);

    indexOfGroup = NULL;
}

PartList::~PartList()
{
    for(PartList::It i = list.begin(); i != list.end(); ++i)
    {
        i->second->Disconnect();
        delete i->second;
    }
}

void PartList::Delete(unsigned id)
{
    It it = list.find(id);
    if(it == list.end())
        return;
    SafeDelete(it->second);
    RemoveFromGroups(id);
}

void PartList::Delete(Group *group)
{

    Group partsToDelete;
    partsToDelete.insert(partsToDelete.begin(), group->begin(), group->end());
    for(GroupIt i = partsToDelete.begin(); i != partsToDelete.end(); ++i)
    {
        It it = list.find(*i);
        if(it != list.end())
        {
            SafeDelete(it->second);
        }
    }
}

void PartList::SafeDelete(Part* p)
{
    Group d;
    p->GetAttachedParts(&d, false);
    p->Disconnect();
    if(p->ShouldDeleteConnections())
    {
        for(GroupIt i = d.begin(); i != d.end(); ++i)
        {
            It it = list.find(*i);
            if(it != list.end())
            {

                RemoveFromGroups(*i);
                delete it->second;
                list.erase(it);
            }
        }
    }
    else
    {
        RemoveFromGroups(p->id);
        list.erase(p->id);
        delete p;
    }
}

int PartList::IndexOf(Part *part)
{
    int index = 0;
    /*if(indexOfGroup)
    {
        for(unsigned i = 0; i < indexOfGroup->size(); i++)
        {
            if(indexOfGroup->at(i) == part->id)
                return index;
            if(!At(indexOfGroup->at(i))->isSandbox && At(indexOfGroup->at(i))->valid)
                index++;
        }
    }*/
    for(unsigned i = 0; i < drawOrder.size(); ++i)
    {
        Part *it = At(drawOrder[i]);
        if(!it)
            continue;
        if(it == part)
        {
            return index;
        }
        if(!it->isSandbox && it->valid)
        {
            if(indexOfGroup)
            {
                if(InsideGroup(it->id, indexOfGroup))
                    index++;
            }
            else
                index++;
        }
    }

    return -1;
}

void PartList::Add(Part* p, Group *group)
{
    It it = list.find(p->id);
    if(it != list.end())
        return;

    p->allParts = this;

    list[p->id] = p;
    if(group)
        group->push_back(p->id);

    drawOrder.push_back(p->id);
}

Part* PartList::At(unsigned id)
{
    It it = list.find(id);
    if(it == list.end())
        return NULL;
    return it->second;
}

void PartList::AddGroup(Group *group)
{
    for(unsigned i = 0; i < groupList.size(); i++)
        if(groupList[i] == group)
            return;
    groupList.push_back(group);
}

void PartList::RemoveGroup(Group *group)
{
    for(int i = groupList.size() -1; i >= 0; i--)
        if(groupList[i] == group)
            groupList.erase(groupList.begin()+i);
}

bool PartList::InsideGroup(unsigned id, Group *group)
{
    for(unsigned i = 0; i < group->size(); ++i)
        if(group->at(i) == id)
            return true;

    return false;
}

void PartList::RemoveFromGroups(unsigned id)
{
    for(unsigned i = 0; i < groupList.size(); ++i)
    {
        GroupIt it = std::find(groupList[i]->begin(), groupList[i]->end(), id);
        if(it != groupList[i]->end())
            groupList[i]->erase(it);
    }
}

void PartList::ToString(std::vector<String> &contents)
{
    indexOfGroup = NULL;
    for(unsigned i = 0; i < drawOrder.size(); i++)
    {
        Part *part = At(drawOrder[i]);
        if(part)
            if(!part->isSandbox && part->valid)
                contents.push_back(part->ToString());
    }
}

void PartList::Load(FileContents &contents)
{
    loaded.clear();
    temp.clear();
    JointList toConnect;
    for(unsigned i = 0; i < contents.size(); ++i)
    {
        if(!contents[i].size())
            continue;
        if(contents[i][0] == "Rectangle")
        {
            Rectangle *rect = new Rectangle(0, 0, 0, 0);
            rect->Load(contents[++i]);
            Add(rect, &loaded);
        }
        else if(contents[i][0] == "Circle")
        {
            Circle *cir = new Circle(sf::Vector2f(0, 0), 0);
            cir->Load(contents[++i]);
            Add(cir, &loaded);
        }
        else if(contents[i][0] == "Thruster")
        {
            Thruster *thr = new Thruster(NULL, sf::Vector2f(0,0), true);
            thr->Load(contents[++i]);

            /*if(loaded.size() > thr->part1Index)
                thr->Connect((ShapePart*) At(loaded[thr->part1Index]));
            else
                thr->valid = false;*/
            Add(thr, &loaded);
            toConnect.push_back(thr);
        }
        else if(contents[i][0] == "FixedJoint")
        {
            FixedJoint *fix = new FixedJoint(NULL, NULL, sf::Vector2f(0,0), true);
            fix->Load(contents[++i]);
            /*if(loaded.size() > fix->part1Index && loaded.size() > fix->part2Index)
                fix->Connect((ShapePart*) At(loaded[fix->part1Index]), (ShapePart*) At(loaded[fix->part2Index]));
            else
                fix->valid = false;*/
            Add(fix, &loaded);
            toConnect.push_back(fix);
        }
        else if(contents[i][0] == "RevoluteJoint")
        {
            RevoluteJoint *rev = new RevoluteJoint(NULL, NULL, sf::Vector2f(0,0), true);
            rev->Load(contents[++i]);
            /*if(loaded.size() > rev->part1Index && loaded.size() > rev->part2Index)
                rev->Connect((ShapePart*) At(loaded[rev->part1Index]), (ShapePart*) At(loaded[rev->part2Index]));
            else
                rev->valid = false;*/
            Add(rev, &loaded);
            toConnect.push_back(rev);
        }
        else if(contents[i][0] == "ChallengeBox")
        {
            ChallengeBox *box = new ChallengeBox(sf::Vector2f(0,0), sf::Vector2f(0,0));
            box->Load(contents[++i]);
            box->partId = loaded[box->partIndex];
            Add(box, &loaded);
        }
        else
        {

        }
    }
    for(unsigned i = 0; i < toConnect.size(); i++)
    {
        if(loaded.size() > toConnect[i]->part1Index && loaded.size() > toConnect[i]->part2Index)
        {
            ShapePart *p1 = (ShapePart*) At(loaded[toConnect[i]->part1Index]);
            ShapePart *p2 = (ShapePart*) At(loaded[toConnect[i]->part2Index]);
            toConnect[i]->Connect(p1, p2);
        }

        else
            toConnect[i]->valid = false;
    }
}


void PartList::Copy(Group *group)
{
    clipboard.clear();
    indexOfGroup = group;
    //SortShapesFirst(indexOfGroup);
    for(unsigned i = 0; i < drawOrder.size(); i++)
    {
        if(!InsideGroup(drawOrder[i], group))
            continue;
        StringList contents;
        StringList type;
        Part *p = At(drawOrder.at(i));
        if(p)
            if(!p->isSandbox && p->valid)
            {
                type.push_back(p->type);

                std::stringstream line(p->ToString());
                std::istream_iterator<String> begin(line);
                std::istream_iterator<String> end;
                std::vector<String> vstrings(begin, end);
                //TODO: apagando o tipo no comeco para dar numero par de content
                vstrings.erase(vstrings.begin());

                clipboard.push_back(type);
                clipboard.push_back(vstrings);
            }
    }
    indexOfGroup = NULL;
}

void PartList::Paste()
{
    selected.clear();
    Load(clipboard);
    selected.insert(selected.end(), loaded.begin(), loaded.end());

}

void PartList::MoveToFront(Group *group)
{
    if(!group || group == &drawOrder)
        return;

    for(unsigned i = 0; i < group->size(); ++i)
        drawOrder.erase(std::remove(drawOrder.begin(), drawOrder.end(), group->at(i)), drawOrder.end());
    drawOrder.insert(drawOrder.end(), group->begin(), group->end());
}

void PartList::MoveToBack(Group *group)
{
    if(!group || group == &drawOrder)
        return;

    for(unsigned i = 0; i < group->size(); ++i)
        drawOrder.erase(std::remove(drawOrder.begin(), drawOrder.end(), group->at(i)), drawOrder.end());
    drawOrder.insert(drawOrder.begin(), group->begin(), group->end());
}


void PartList::Rotate(Group *group, Num angle, sf::Vector2f center)
{
    temp.clear();
    for(unsigned i = 0; i < group->size(); i++)
    {
        Part *part = At(group->at(i));
        part->GetAttachedParts(&temp);
    }
    for(unsigned i = 0; i < temp.size(); i++)
    {
        Part *part = At(temp.at(i));
        if(part)
            part->RotateAround(center, angle);
    }
    temp.clear();
}

void PartList::SortShapesFirst(Group *g)
{
    Group sorted;
    for(unsigned i = 0; i < g->size(); ++i)
    {
        if(At(g->at(i))->IsShapePart())
            sorted.insert(sorted.begin(), g->at(i));
        else
            sorted.push_back(g->at(i));
    }
    g->clear();
    g->insert(g->begin(), sorted.begin(), sorted.end());
}
