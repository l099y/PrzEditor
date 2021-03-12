#include "emitter.h"

Emitter::Emitter(QObject *parent) : QObject(parent)
{

}
Emitter::~Emitter()
{

}

void Emitter::emitleftxtndactivated()
{
    emit leftxtndactivated();
}

void Emitter::emitrightxtndactivated()
{
    emit rightxtndactivated();
}

void Emitter::emitxtendDeactived()
{
    emit xtendDeactived();
}
