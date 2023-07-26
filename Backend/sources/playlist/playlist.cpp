#include "../../headers/playlist/playlist.h"


Playlist::Playlist(const QString &name, const QVector<PlaylistMediaElement> &data) :
    m_mediaElements(data), m_playlistName(name)
{

}

//Adding new media to playlist
void Playlist::add(const QUrl &url)
{
    PlaylistMediaElement newElement(url);

    if(!m_mediaElements.contains(newElement) && newElement.isPathValid() && !newElement.isEmpty())
    {
        m_mediaElements.push_back(newElement);
    }
}

//Removing one media from playlist using url
void Playlist::remove(const QUrl &url)
{
    for(int i = 0; i < m_mediaElements.size(); ++i)
    {
        const PlaylistMediaElement &element = m_mediaElements[i];

        if(element.mediaPath()== url)
        {
            m_mediaElements.removeAt(i);
            return;
        }
    }
}

void Playlist::remove(const QList<QUrl> &urls)
{
    for(const QUrl &url : urls)
        remove(url);
}

int Playlist::count() const
{
    return m_mediaElements.size();
}

int Playlist::positionOf(const PlaylistMediaElement &element) const
{
    for(int index = 0; index < m_mediaElements.size(); ++index)
    {
        if(element == m_mediaElements.at(index))
            return index;
    }

    return -1;
}

//Getting copy of playlist element by url
PlaylistMediaElement Playlist::get(const QUrl &source) const
{
    for(const PlaylistMediaElement &element : m_mediaElements)
    {
        if(element.mediaPath() == source)
            return element;
    }
    return PlaylistMediaElement();
}

PlaylistMediaElement Playlist::get(const int &index) const
{
    return m_mediaElements.value(index);
}


QString Playlist::playlistName() const
{
    return m_playlistName;
}

void Playlist::setPlaylistName(const QString &newPlaylistName)
{
    m_playlistName = newPlaylistName;
}
