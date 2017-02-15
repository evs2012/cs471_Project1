#include "processitem.h"


ProcessItem::ProcessItem()
{
    this->name = "new process";
    this->TimeNeeded = 0;
}

ProcessItem::ProcessItem(QString _name, int _TimeNeeded)
{
    this->name = _name;
    this->TimeNeeded = _TimeNeeded;
    this->UID = newUID++;
}

QString ProcessItem::Display()
{
    return this->name + "\t" + QString::number(this->TimeNeeded);
}

bool ProcessItem::operator== (const ProcessItem &p)
{
    return(p.UID == this->UID);
}

int ProcessItem::newUID = 0;
