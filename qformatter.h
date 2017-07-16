#ifndef QFORMATTER_H
#define QFORMATTER_H

#include <QFile>       // Pour utiliser le fichier
#include <QString>     // Stocke le contenu du fichier
#include <QTextStream> // Flux sortant du fichier
#include <QMap>

class QFormatter
{
public:
    bool removeCmt = true;
    bool includeString = false;
    bool includeInclude = false;
    bool useAlphaNumeriqueSeparator = true;

    QFormatter();

    QString executeInFile(QString const& filePath);
    QMap<QString, int> getMap(QString const& data);

private:
    bool commentOpen = false;
    void genereMap(QMap<QString, int>& map,QString const& data, QChar separator);
    void useAlphaNumerique(QMap<QString, int>& map, QString const&data, QChar separator);

    bool filterFile(QString& fileLignes,QString const& filePath);

    void formatLigne(QString& ligne);
    bool isEmpty(QString ligne);
    void cleanLigne(QString& ligne);

    bool isInclude(QString const& ligne);
    bool isComment(QString const& ligne);
    bool isUsingNamespace(QString const& ligne);
    void removeComment(QString& ligne);
    void filtreGuillemet(QString& ligne);
};

#endif // QFORMATTER_H
