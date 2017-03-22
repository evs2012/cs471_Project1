#ifndef PROCESSITEM_H
#define PROCESSITEM_H
#include <QString>

class ProcessItem
{
public:
    ProcessItem();
    ProcessItem(QString _name, int _TimeNeeded);
    QString Display();

    QString name;
    int TimeNeeded;
    int TimeRecieved;

    bool operator== (const ProcessItem &p);
    int UID;

private:

    static int newUID;
};

#endif // PROCESSITEM_H
