#include "../../headers/player/pmplayermodel.h"

PMPlayerModel::PMPlayerModel(QObject *parent)
    : QObject{parent}
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
}

QMediaPlayer *PMPlayerModel::player() const
{
    return m_player;
}

void PMPlayerModel::setPlayer(QMediaPlayer *newPlayer)
{
    if (m_player == newPlayer)
        return;
    m_player = newPlayer;
    emit playerChanged();
}

QAudioOutput *PMPlayerModel::audioOutput() const
{
    return m_audioOutput;
}

void PMPlayerModel::setAudioOutput(QAudioOutput *newAudioOutput)
{
    if (m_audioOutput == newAudioOutput)
        return;
    m_audioOutput = newAudioOutput;
    emit audioOutputChanged();
}
