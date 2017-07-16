#include "qformatter.h"

QFormatter::QFormatter()
{
}

QString QFormatter::executeInFile(const QString &filePath)
{
    QString data("");
    filterFile(data, filePath);
    return data;
}

QMap<QString, int> QFormatter::getMap(const QString &data)
{
    QMap<QString, int> map;
    genereMap(map, data, ' ');
    return map;
}

void QFormatter::genereMap(QMap<QString, int> &map, const QString &data, QChar separator)
{
    if(useAlphaNumeriqueSeparator) {
        useAlphaNumerique(map,data,separator);
    } else {
        QStringList listWord = data.split(separator, QString::SkipEmptyParts);
        for(int i(0); i < listWord.size(); i++){
            map[listWord.at(i)] += 1;
        }
    }
}

void QFormatter::useAlphaNumerique(QMap<QString, int> &map, const QString &data, QChar separator)
{
    QString currentWord("");
    for(int i(0); i< data.size(); i++){
        QChar c = data.at(i);

        if (c.isLetterOrNumber()){
            currentWord += c;
        } else {
            map[currentWord] += 1;
            currentWord = "";

            if (c != separator){
                currentWord += c;
                map[currentWord] += 1;
                currentWord = "";
            }
        }
    }
}

bool QFormatter::filterFile(QString &fileLignes,const QString &filePath)
{
    QFile fichier(filePath);
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        QString ligne;
        while(!flux.atEnd()){
            ligne = flux.readLine();
            formatLigne(ligne);
            if (ligne.isEmpty()){
                continue;
            }

            if (!includeString) {
                if (!isComment(ligne)){
                    filtreGuillemet(ligne);
                }
            }
            fileLignes += ligne;
        }
        fichier.close();
        return true;
    } else {
        return false;
    }
}

void QFormatter::filtreGuillemet(QString &ligne){

    while (ligne.contains("\"")) {
        QStringList list1 = ligne.split("\"");

        QString final("");

        for (int i(0); i< list1.size() ; i++)
        {
            if (i%2 == 0) {
                final += list1.at(i);
            }
        }

        ligne = final;
    }

    while (ligne.contains("'")) {
        QStringList list1 = ligne.split("'");

        QString final("");

        for (int i(0); i< list1.size() ; i++)
        {
            if (i%2 == 0) {
                final += list1.at(i);
            }
        }

        ligne = final;
    }
}

void QFormatter::formatLigne(QString &ligne)
{
    if (!isEmpty(ligne)) {
        if (removeCmt) {
            removeComment(ligne);
        }
        cleanLigne(ligne);

        if (isUsingNamespace(ligne)){
            ligne = "";
        }

        if (!includeInclude && isInclude(ligne)){
            ligne = "";
        }
    } else {
        ligne = "";
    }
}

void QFormatter::cleanLigne(QString &ligne)
{
    while (ligne.contains("	") || ligne.contains("  ")) {// tab or space
        ligne = ligne.replace("	", " ");//\t
        ligne = ligne.replace("  ", " ");
    }
}

bool QFormatter::isEmpty(QString ligne)
{
    if (ligne.isEmpty()){
        return true;
    }

    ligne = ligne.replace("	", " ");//\t
    ligne = ligne.replace("  ", " ");

    if ((ligne.size() == 1 && ligne.contains(" ")) || ligne.isEmpty()){
        return true;
    }

    return false;
}

bool QFormatter::isInclude(const QString &ligne){
    return ligne.startsWith("#include ");
}

bool QFormatter::isUsingNamespace(const QString &ligne){
    return ligne.startsWith("using namespace");
}

bool QFormatter::isComment(const QString &ligne){
    if(ligne.startsWith("\/*")){
        commentOpen = true;
    }

    if (ligne.endsWith("*\/")) {
        commentOpen = false;
        return true;
    }
    return commentOpen;
}

void QFormatter::removeComment(QString &ligne){
    if(ligne.startsWith("\/*")){
        commentOpen = true;
    }

    if (commentOpen) {
        if (ligne.endsWith("*\/")) {
            commentOpen = false;
        }
        ligne = "";
    }
    ligne = ligne.split("\\\\").first();
}
