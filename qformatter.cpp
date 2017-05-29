#include "qformatter.h"

QFormatter::QFormatter(QString filePath)
{
    if (exec(filePath)){
        genereMap();
    }
}

void QFormatter::genereMap()
{
    QString currentWord("");
    for(int i(0); i< m_data.size(); i++){
        QChar c = m_data.at(i);

        if (c.isLetterOrNumber()){
            currentWord += c;
        } else {
            m_map[currentWord] += 1;
            currentWord = "";

            if (!c.isSpace()){
                currentWord += c;
                m_map[currentWord] += 1;
                currentWord = "";
            }
        }
    }
}

bool QFormatter::exec(QString filePath)
{
    QFile fichier(filePath);
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_data = "";
        QTextStream flux(&fichier);
        QString ligne;
        while(!flux.atEnd()){
            ligne = flux.readLine();
            formatLigne(ligne);
            if (ligne.isEmpty()){
                continue;
            }

            filtreGuillemet(ligne);
            m_data += ligne;
        }
        fichier.close();
        return true;
    } else {
        return false;
    }
}

void QFormatter::filtreGuillemet(QString &ligne){
    while (ligne.contains("\"")) {
        ligne.replace(QString("\\\""),QString(""));
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
}

void QFormatter::formatLigne(QString &ligne)
{
    if (!isEmpty(ligne)) {
        removeComment(ligne);
        cleanLigne(ligne);

        if (isInclude(ligne) || isUsingNamespace(ligne)){
            ligne = "";
        }
    } else {
        ligne = "";
    }
}

void QFormatter::cleanLigne(QString &ligne)
{
    while (ligne.contains(QString("	")) || ligne.contains(QString("  "))) {
        ligne = ligne.replace(QString("	"), QString(" "));
        ligne = ligne.replace(QString("  "), QString(" "));
    }
}

bool QFormatter::isEmpty(QString ligne)
{
    if (ligne.isEmpty()){
        return true;
    }

    ligne = ligne.replace(QString("	"), QString(" "));
    ligne = ligne.replace(QString("  "), QString(" "));

    if ((ligne.size() == 1 && ligne.contains(QString(" "))) || ligne.isEmpty()){
        return true;
    }

    return false;
}

bool QFormatter::isInclude(const QString &ligne){
    return ligne.contains(QString("#include "));
}

bool QFormatter::isUsingNamespace(const QString &ligne){
    return ligne.contains(QString("using namespace"));
}

void QFormatter::removeComment(QString &ligne){
    ligne = ligne.split("\\\\").first();
}
