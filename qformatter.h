#ifndef QFORMATTER_H
#define QFORMATTER_H

#include <QFile>       // Pour utiliser le fichier
#include <QString>     // Stocke le contenu du fichier
#include <QTextStream> // Flux sortant du fichier
#include <QMap>

class QFormatter
{
public:
    QFormatter(QString filePath);

    QMap<QString, int> getMap() { return m_map; }

private:
    QString m_data;
    QMap<QString, int> m_map;

    bool exec(QString filePath);

    void genereMap();

    void formatLigne(QString& ligne);
    bool isEmpty(QString ligne);
    void cleanLigne(QString& ligne);

    bool isInclude(QString const& ligne);
    bool isUsingNamespace(QString const& ligne);
    void removeComment(QString& ligne);
    void filtreGuillemet(QString& ligne);
};

#endif // QFORMATTER_H
