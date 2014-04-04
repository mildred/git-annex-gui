#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "config_to_json.h"

int config_to_json(QString filename)
{
    static QRegExp configLine("(\\S*) (\\S*) = (.*)");

    QFile f(filename);
    Q_ASSERT(f.open(QFile::ReadOnly));
    QJsonObject root;

    while(!f.atEnd()) {
        QByteArray line = f.readLine();

        if(line[0] == '#') continue;

        if(configLine.exactMatch(line)) {
            QString config = configLine.cap(1);
            QString key    = configLine.cap(2);
            QString value  = configLine.cap(3).trimmed();

            QJsonValue configValue = root[config];
            QJsonObject configObject = (configValue.isObject()) ?
                        configValue.toObject():
                        QJsonObject();

            if(config == "group") {
                configObject[key] = QJsonArray::fromStringList(value.split(QRegExp("\\s+"), QString::SkipEmptyParts));
            } else {
                configObject[key] = QJsonValue(value);
            }

            root[config] = configObject;
        }
    }

    QTextStream out(stdout);
    QJsonDocument doc(root);
    out << doc.toJson();

    return 0;
}
