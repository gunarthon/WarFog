#ifndef PARTLIST_H
#define PARTLIST_H

#include "Utils.h"
#include <map>

class Part;

typedef std::vector<unsigned> Group;
typedef Group::iterator GroupIt;


class PartList
{
    public:
        typedef std::map<unsigned, Part*>::iterator It;
        typedef std::map<unsigned, Part*>::reverse_iterator RIt;

        PartList();
        virtual ~PartList();
        void Delete(unsigned id);
        void Delete(Group *group);
        void Add(Part* p, Group *group = NULL);
        Part* At(unsigned id);
        void AddGroup(Group *group);
        void RemoveGroup(Group *group);
        void RemoveFromGroups(unsigned id);
        int IndexOf(Part *part);
        bool InsideGroup(unsigned id, Group *group);
        void ToString(std::vector<String> &contents);
        void Load(FileContents &contents);
        void Copy(Group *group);
        void Paste();
        void MoveToFront(Group *group);
        void MoveToBack(Group *group);
        void Rotate(Group *group, Num angle, sf::Vector2f center);

        std::map<unsigned, Part*> list;
        FileContents clipboard;

        Group selected;
        Group loaded;
        Group candidate;
        Group temp;
        Group drawOrder;

    protected:
        std::vector<Group*> groupList;
        Group *indexOfGroup;
    private:
        void SafeDelete(Part* p);
        void SafeDelete(Group *d);
        void SortShapesFirst(Group *g);
};
#endif // PARTLIST_H
