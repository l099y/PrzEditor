#ifndef SEQUENCEDATA_H
#define SEQUENCEDATA_H

#include <QObject>
#include <QJsonObject>

class SequenceData : public QObject
{
    Q_OBJECT
public:
    SequenceData(QObject* parent);
    ~SequenceData();
    QString sequencefilename;
    QString path;
    QString name;
    int startIdx;
    int endIdx;
    bool corrupted = false;

    int sequencelength();
    bool CheckIntegrity();

    QJsonObject generateJson();
};
#endif // SEQUENCEDATA_H
