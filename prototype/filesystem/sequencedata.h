#ifndef SEQUENCEDATA_H
#define SEQUENCEDATA_H

#include <QObject>
#include <QJsonObject>
#include <QHash>


struct fileInf{
    QString name;
    int idx;
    int padding;
};

class SequenceData : public QObject
{
    Q_OBJECT
public:
    SequenceData(QObject* parent);
    SequenceData(QJsonObject, QObject* parent);
    ~SequenceData();
    QString sequencefilename;
    QString path;
    QString name;
    int startIdx;
    int padding;
    int endIdx;
    bool corrupted = false;

    int sequencelength();
    bool checkIntegrity();

    fileInf getReleventInfo(QString* path);

    QHash<int,int> corruptedSubSequences;
    QJsonObject generateJson();
};
#endif // SEQUENCEDATA_H


// std lib -> les pointers ne sont plus défnit comme pointeurs mais uniquepointers (ils possèdent déjà une mécanique de construction et de destruction )

// mémoire virtuelle. le programme s'aloue x bloc de mémoire physique, si nécessaire il requière d'autre blocs, tant que tout le bloc n'est pas vide ou d'autres circontances
// fragmentation interne? parfois on aloue plus d'espace que nécessaire . -> possibilité de reserver un bloc de mémoire spécific virtuel.

//Utiliser la fonction malloc de stdlib, avec en paramêtre une taille de block en byte.
 // recherche virtual alloc memory map file -> pour aloué de la mémoire virtuelle à du data.
