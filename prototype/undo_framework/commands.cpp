#include "commands.h"
#include "sequence_elements/shot.h"
#include "sequence_elements/timelinescene.h"
#include "filesystem/sequencedata.h"
#include <QDebug>
#include <QDataStream>
#include <QHash>
#include <mainwindow.h>
#include <QJsonObject>
#include <QGraphicsItem>

DeleteShotsCommand::DeleteShotsCommand(QList<Shot*> deletedShots, QList<SoundTrack*> deletedSounds, TimelineScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    timeline = scene;
    this->deletedShots = deletedShots;
    this->deletedSounds = deletedSounds;
    movedShots=timeline->getShotsPastSelection();
    foreach (Shot* current, deletedShots){
        totalwidthremoved+=current->rect().width();
    }
    setText(QObject::tr("Delete %1")
            .arg(" created "));
}

void DeleteShotsCommand::undo()
{
    foreach (Shot* current, deletedShots){
        timeline->addItem(current);
    }
    foreach (SoundTrack* current, deletedSounds){
        timeline->addItem(current);
    }
    foreach (Shot* current, movedShots){
        current->setX(current->scenePos().x()+totalwidthremoved);
        current->setPreviousToCurrent();
    }
    timeline->ExtendSceneWidth(totalwidthremoved);
    timeline->scaleViewToScene();
}

void DeleteShotsCommand::redo()
{
    foreach (Shot* current, deletedShots){
        timeline->removeItem(current);
    }
    foreach (SoundTrack* current, deletedSounds){
        timeline->removeItem(current);
    }
    foreach (Shot* current, movedShots){
        current->setX(current->scenePos().x()-totalwidthremoved);
        current->setPreviousToCurrent();
    }
    timeline->ExtendSceneWidth(-totalwidthremoved);
    timeline->scaleViewToScene();
}


AddCommand::AddCommand(QList<SequenceData*> seq, int xpos, int length, TimelineScene *scene, QVector<Shot*> movedShots ,QUndoCommand *parent)
    : QUndoCommand(parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->previousxpos);
        movedShotNewPos.insert(current, current->scenePos().x());
    }
    timeline = scene;
    shot = new Shot();

    this->seqs = seq;
    this->xpos = xpos;
    this->length = length;
    shot->seqs.append(seqs[0]);

    QJsonObject newparam = shot->templateParams.value("Shot designation");
    newparam.insert("value", seqs[0]->name);
    shot->templateParams.insert("Shot designation", newparam);


    scene->update();
    setText(QObject::tr("Add %1")
            .arg(seqs[0]->name));
}

AddCommand::~AddCommand()
{
    if (!shot->scene())
        delete shot;
}

void AddCommand::undo()
{
    timeline->removeItem(shot);
    timeline->update();
    QHash<Shot*, int>::const_iterator i = movedShotOldPos.constBegin();
    while (i != movedShotOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,timeline->sceneRect().width()-shot->rect().width(),50);
    timeline->setPreviousToCurrent();
    if (timeline->sceneRect().width()>10000)
    timeline->scaleViewToScene();
//    timeline->sceneRect().setWidth(timeline->sceneRect().width()-shot->rect().width());
//    timeline->previousSceneWidth = timeline->sceneRect().width();
}

void AddCommand::redo()
{
    QHash<Shot*, int>::const_iterator i = movedShotNewPos.constBegin();
    while (i != movedShotNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->addItem(shot);
    shot->setXToFrame(xpos);
    shot->setRect(0, 0, length, 100);
    shot->setY(160);
    qDebug()<<shot->rect() << "in redo"<< shot->scenePos().y();;
    shot->setPreviousToCurrent();
    timeline->clearSelection();
    shot->setSelected(true);
    timeline->setSceneRect(0,0,timeline->sceneRect().width()+shot->rect().width(),400);
    timeline->setPreviousToCurrent();
    timeline->update();
    timeline->scaleViewToScene();

//    timeline->sceneRect().setWidth(timeline->sceneRect().width()+shot->rect().width());
//    timeline->previousSceneWidth = timeline->sceneRect().width();

}

MoveCommand::MoveCommand(TimelineScene* timeline, QVector<Shot *> movedShots, int prevscenepos, int currentscenepos, QUndoCommand *parent) : QUndoCommand(parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->scenePos().x());
        movedShotNewPos.insert(current, current->previousxpos);
    }
    this->timeline = timeline;
    this->prevscenesize = prevscenepos;
    this->currentscenesize = currentscenepos;
    setText(QObject::tr("shots mouvements"));
}

void MoveCommand::undo()
{
    QHash<Shot*, int>::const_iterator i = movedShotNewPos.constBegin();
    while (i != movedShotNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,prevscenesize,400);
    timeline->setPreviousToCurrent();
    timeline->scaleViewToScene();
}

void MoveCommand::redo()
{
    QHash<Shot*, int>::const_iterator i = movedShotOldPos.constBegin();
    while (i != movedShotOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,currentscenesize,400);
    timeline->setPreviousToCurrent();
}



ClearCommand::ClearCommand(TimelineScene* timeline, QVector<Shot *> removedshots, int prevtimelinewidth, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->timeline = timeline;
    foreach(QGraphicsItem* current, this->timeline->items()){
        auto sound = dynamic_cast<SoundTrack*>(current);
        if (sound)
            removedSound = sound;
    }
    this->removedShots = removedshots;
    this->prevtimelinesize = prevtimelinewidth;
    setText(QObject::tr("clear command"));
}

void ClearCommand::undo()
{
    foreach (Shot* current, removedShots){
        timeline->addItem(current);
    }
    timeline->setSceneRect(0,0,prevtimelinesize,400);

    if (removedSound != nullptr){
        timeline->addItem(removedSound);
    }
    timeline->setPreviousToCurrent();
    timeline->scaleViewToScene();

}


void ClearCommand::redo()
{
    timeline->clearSelection();
    foreach (Shot* current, removedShots)
    {
        timeline->removeItem(current);
    }
    if (removedSound != nullptr){
        timeline->removeItem(removedSound);
    }
    timeline->setSceneRect(0,0, 10000, 400);
    timeline->setPreviousToCurrent();
    timeline->scaleViewToScene();
    timeline->newRect();
}

LoadCommand::LoadCommand(TimelineScene* timeline, QString path, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->timeline = timeline;
    timelinesize = this->timeline->sceneRect().width();
    foreach (QGraphicsItem* current, this->timeline->items()){
        Shot* sh = dynamic_cast<Shot*>(current);
        if (sh){
            shots.append(sh);
        }
    }
    setText(QObject::tr("load %1").arg(path));
}

void LoadCommand::undo()
{
    foreach (QGraphicsItem* current, timeline->items())
    {
        auto sh = dynamic_cast<Shot*>(current);
        if (sh){
            timeline->removeItem(sh);
        }
    }
    timeline->setSceneRect(0,0, 2000000, 400);
    timeline->ruler.setSize(2000000);
    timeline->przreg->usedSequences.clear();
    timeline->newRect();
}
void LoadCommand::redo(){

    foreach (Shot* current, shots){
        timeline->addItem(current);
    }
}

ChangeParameterInShotCommand::ChangeParameterInShotCommand(MainWindow* app, QList<Shot*>shot, QJsonObject obj, QUndoCommand *parent) : QUndoCommand(parent)
{
   this->app = app;
   QString name = obj.value("name").toString();
   foreach (Shot* current, shot){
       oldconfig.insert(current, current->templateParams.value(name));
       newconfig.insert(current, obj);
   }


   setText(QObject::tr("change parameter %1").arg(name));
}

void ChangeParameterInShotCommand::undo()
{
    Shot* sh = nullptr;
    QHash<Shot*, QJsonObject>::const_iterator i = oldconfig.constBegin();
    while (i != oldconfig.constEnd()) {
        auto  shot =i.key();
        if (sh == nullptr)
            sh = shot;
        shot->templateParams.insert(i.value().value("name").toString(), i.value());
        i++;
    }

   sh->scene()->clearSelection();

   i = newconfig.constBegin();
      while (i != newconfig.constEnd()) {
          auto  shot =i.key();
          i++;
   }
}
void ChangeParameterInShotCommand::redo()
{

    Shot* sh = nullptr;
    QHash<Shot*, QJsonObject>::const_iterator i = newconfig.constBegin();
    while (i != newconfig.constEnd()) {
        auto  shot =i.key();
        if (sh == nullptr)
            sh = shot;
        shot->templateParams.insert(i.value().value("name").toString(), i.value());
        i++;
    }
    sh->scene()->clearSelection();

    i = newconfig.constBegin();
       while (i != newconfig.constEnd()) {
           auto  shot =i.key();
           shot->setSelected(true);
           i++;
    }
}



AddSoundCommand::AddSoundCommand(SoundTrack* suppressedSound,TbeSoundData * sounddata, int xpos, int length, TimelineScene *timeline, QVector<SoundTrack * > movedSounds, QUndoCommand *parent)
{
    foreach(SoundTrack* current, movedSounds){
        movedSoundsOldPos.insert(current, current->previousxpos);
        movedSoundsNewPos.insert(current, current->scenePos().x());
    }
    this->suppressedSound = suppressedSound;
    this->timeline = timeline;
    sound = new SoundTrack();
    this->sounddata = sounddata;
    this->xpos = xpos;
    this->length = length;

    setText(QObject::tr("Add %1")
            .arg(sounddata->filename));
}

AddSoundCommand::~AddSoundCommand()
{
    if (!sound->scene())
        delete sound;
}

void AddSoundCommand::undo()
{
    timeline->przreg->usedSoundFiles.remove(sounddata->filename);
    timeline->removeItem(sound);
    timeline->addItem(suppressedSound);
    timeline->update();
    QHash<SoundTrack*, int>::const_iterator i = movedSoundsOldPos.constBegin();
    while (i != movedSoundsOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
}

void AddSoundCommand::redo()
{

    QHash<SoundTrack*, int>::const_iterator i = movedSoundsNewPos.constBegin();
    while (i != movedSoundsNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->przreg->usedSoundFiles.insert(sounddata->filename, sounddata);
    timeline->removeItem(suppressedSound);
    timeline->addItem(sound);
    sound->setXToFrame(xpos);
    sound->setRect(0, 0, 10000000, 20);
    sound->setY(260);
    sound->setPreviousToCurrent();
    sound->soundfile=sounddata;
    timeline->clearSelection();
    sound->setSelected(true);
    timeline->update();
}

MoveSoundsCommand::MoveSoundsCommand(TimelineScene* timeline, QVector<SoundTrack *> movedShots, int prevscenepos, int currentscenepos, QUndoCommand *parent) : QUndoCommand(parent)
{
    foreach(SoundTrack* current, movedShots){
        movedSoundsOldPos.insert(current, current->scenePos().x());
        movedSoundsNewPos.insert(current, current->previousxpos);
    }
    this->timeline = timeline;
    this->prevscenesize = prevscenepos;
    this->currentscenesize = currentscenepos;
    setText(QObject::tr("soundtracks mouvements"));
}

void MoveSoundsCommand::undo()
{
    QHash<SoundTrack*, int>::const_iterator i = movedSoundsNewPos.constBegin();
    while (i != movedSoundsNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,prevscenesize,100);
}

void MoveSoundsCommand::redo()
{
    QHash<SoundTrack*, int>::const_iterator i = movedSoundsOldPos.constBegin();
    while (i != movedSoundsOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    timeline->setSceneRect(0,0,currentscenesize,100);
}

ResizeShotCommand::ResizeShotCommand(TimelineScene *timeline, QVector<Shot *> movedShots, Shot* shot,int newShotSize, QUndoCommand *parent)
{
    foreach(Shot* current, movedShots){
        movedShotOldPos.insert(current, current->scenePos().x());
        movedShotNewPos.insert(current, current->previousxpos);
    }
    this->resizedShot = shot;
    this->timeline = timeline;

    this->previousShotWidth = shot->previousboxwidth;
    this->newShotWidth = newShotSize;

    this->prevscenesize = timeline->sceneRect().width();
    this->currentscenesize =  timeline->sceneRect().width()+ (newShotWidth-previousShotWidth);

    setText(QObject::tr("shot resize"));

        previousGlowFromFade = resizedShot->templateParams.value("Fade From Black Frame Out").value("value").toString().toInt();
        previousGlowToFade = resizedShot->templateParams.value("Fade To Black Frame Out").value("value").toString().toInt();

    if (previousGlowFromFade>=newShotSize/10){
        newGlowFromFade = newShotSize/20;
    }
    if (previousGlowToFade>=newShotSize/10){
        newGlowToFade = newShotSize/20;
    }
}

void ResizeShotCommand::undo()
{
    QHash<Shot*, int>::const_iterator i = movedShotNewPos.constBegin();
    while (i != movedShotNewPos.constEnd()) {

        auto  sh =i.key();
        sh->setXToFrame(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    if (newGlowFromFade != 0){
        qDebug()<<"redone";
        auto obj = resizedShot->templateParams.value("Fade From Black Frame Out");
        obj.insert("value", QString::number(previousGlowFromFade));
        resizedShot->templateParams.insert("Fade From Black Frame Out", obj);
        qDebug()<<resizedShot->templateParams.value("Fade From Black Frame Out");
    }
    if (newGlowToFade != 0){
        auto obj = resizedShot->templateParams.value("Fade To Black Frame Out");
        obj.insert("value", QString::number(previousGlowToFade));
        resizedShot->templateParams.insert("Fade To Black Frame Out", obj);
    }
    resizedShot->setSize(previousShotWidth);
    resizedShot->setPreviousToCurrent();
    timeline->setSceneRect(0,0,prevscenesize,400);
    timeline->clearSelection();
    timeline->scaleViewToScene();
    resizedShot->setSelected(true);
}

void ResizeShotCommand::redo()
{
    QHash<Shot*, int>::const_iterator i = movedShotOldPos.constBegin();
    while (i != movedShotOldPos.constEnd()) {

        auto  sh =i.key();
        sh->setX(i.value());
        sh->setPreviousToCurrent();
        i++;
    }
    if (newGlowFromFade != 0){
        auto obj = resizedShot->templateParams.value("Fade From Black Frame Out");
        obj.insert("value", QString::number(newGlowFromFade));
        resizedShot->templateParams.insert("Fade From Black Frame Out", obj);


    }
    if (newGlowToFade != 0){
        auto obj = resizedShot->templateParams.value("Fade To Black Frame Out");
        obj.insert("value", QString::number(newGlowToFade));
        resizedShot->templateParams.insert("Fade To Black Frame Out", obj);
    }
    resizedShot->setSize(newShotWidth);
    resizedShot->setPreviousToCurrent();
    resizedShot->update();
    timeline->setSceneRect(0,0,currentscenesize,400);
    timeline->clearSelection();
    timeline->scaleViewToScene();
    resizedShot->setSelected(true);
}

changeFrameInCommand::changeFrameInCommand(Shot * shot, int newvalue, QUndoCommand *parent)
{
    this->shot = shot;
    this->oldValue = shot->frameIn;
    this->newValue = newvalue;
    setText(QObject::tr("shot frame in changed"));
}

void changeFrameInCommand::undo()
{
    shot->frameIn = oldValue;
    shot->scene()->clearSelection();
    shot->setSelected(true);
}

void changeFrameInCommand::redo()
{
    shot->frameIn = newValue;
}

AddBackgroundInShotCommand::AddBackgroundInShotCommand(Shot *sh, BackgroundPrz *bg, QUndoCommand *parent)
{
    this->shot = sh;
    this->background =  new BackgroundPrz();
    this->background->filename = bg->filename;
    this->background->path = bg->path;
    this->previousBackground = sh->background;
}

void AddBackgroundInShotCommand::undo()
{

    previousBackground != nullptr ? shot->background = previousBackground : shot->background = nullptr;
    shot->update();
}

void AddBackgroundInShotCommand::redo()
{
    shot->background = background;
    shot->tempBackground = nullptr;
    shot->update();
}
