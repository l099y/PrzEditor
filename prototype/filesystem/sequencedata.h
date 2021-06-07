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

    // used on loading

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

    // identify is all the files required for the sequences are present in the content of the directory(path)
    bool checkIntegrity();

    // this is now executed async in the mainwindow... coudnt use part of the Qconcurrent api in children objects
    bool checkIntegrityAsync();

    // returns the required information to indentify a sequence pattern
    fileInf getReleventInfo(QString* path);

    // while validating its integrity, if missing subsequences are found, they are stored in this collection ... value as end

    QHash<int,int> corruptedSubSequences;
    QJsonObject generateJson();
    QJsonObject generateJsonForExport();

    // this function is used to produce the frame in the json for export
    QJsonArray generateFrames();
};
#endif // SEQUENCEDATA_H


// std lib -> les pointers ne sont plus défnit comme pointeurs mais uniquepointers (ils possèdent déjà une mécanique de construction et de destruction )

// mémoire virtuelle. le programme s'aloue x bloc de mémoire physique, si nécessaire il requière d'autre blocs, tant que tout le bloc n'est pas vide ou d'autres circontances
// fragmentation interne? parfois on aloue plus d'espace que nécessaire . -> possibilité de reserver un bloc de mémoire spécific virtuel.

//Utiliser la fonction malloc de stdlib, avec en paramêtre une taille de block en byte.
 // recherche virtual alloc memory map file -> pour aloué de la mémoire virtuelle à du data.
//cool advices
