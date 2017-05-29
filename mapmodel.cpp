#include "mapmodel.h"

using namespace std;

MapModel::MapModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_map = NULL;
}

int MapModel::rowCount(const QModelIndex& parent) const
{
    if (m_map)
        return m_map->count();
    return 0;
}

int MapModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}

QVariant MapModel::data(const QModelIndex& index, int role) const
{
    if (!m_map)
        return QVariant();
    if (index.row() < 0 ||
        index.row() >= m_map->count() ||
        role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0)
        return m_map->keys().at(index.row());
    if (index.column() == 1)
        return m_map->values().at(index.row());
    return QVariant();
}

void MapModel::setMap(map<string, int>* myMap)
{
    for( const auto& kv : *myMap)
    {
        string key = kv.first;
        QString k = QString::fromUtf8(key.c_str(), key.size());
        int value = kv.second;
        m_map->insert(k,value);
    }
}
