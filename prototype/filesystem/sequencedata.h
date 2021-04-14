#ifndef SEQUENCEDATA_H
#define SEQUENCEDATA_H

#include <QObject>

class SequenceData : public QObject
{
    Q_OBJECT
public:
    SequenceData(QObject* parent);
    ~SequenceData();
    QStringList files;
    QString path;
    QString name;
    int startIdx;
    int endIdx;
    bool corrupted = false;

    int sequencelength();
    bool CheckIntegrity();
};

#endif // SEQUENCEDATA_H
