#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QJsonDocument>
#include <QDir>
#include <QTimer>
#include <QFileSystemWatcher>

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QString path, QObject *parent = 0);

    enum {
        TRUSTED,
        SEMITRUSTED,
        UNTRUSTED,
        DEAD
    };

    bool isAssistantRunning();
    bool isDirectMode();
    QDir path() const;
    int  numCopies() const;
    QString currentRemoteUUID() const;
    QString remoteUUID(QString remoteUUID = "") const;
    QString remoteDescription(QString remoteUUID = "") const;
    QString remoteWantedExpr(QString remoteUUID = "") const;
    int  remoteTrustStatus(QString remoteUUID = "") const;
    QStringList remoteGroups(QString remoteUUID = "") const;

public slots:
    void runWebApp();
    bool runAssistant(bool run);
    void setDirectMode(bool direct);
    void setRemoteDescription(QString description, QString remote = "");
    void setRemoteTrustStatus(int status, QString remote = "");
    void setRemoteWantedExpression(QString expr, QString remote = "");
    void addRemoteGroup(QString group, QString remote = "");
    void removeRemoteGroup(QString group, QString remote = "");
    void setNumCopies(int copies);
    void refreshAssistantRunning();

signals:
    void assistantStarted();
    void assistantStopped();
    void assistantStateChanged(bool);

private:
    QJsonDocument annexInfo() const;
    QJsonDocument annexConfig() const;

    struct {
        QFileSystemWatcher watcher;
        QTimer refreshTimer;
        QDir path;
        bool assistantRunning;
    } m;
};

#endif // REPOSITORY_H
