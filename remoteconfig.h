#ifndef REMOTECONFIG_H
#define REMOTECONFIG_H

#include <QWidget>
#include <QPointer>

class Repository;
class WantedExpr;
namespace Ui {
class RemoteConfig;
}

class RemoteConfig : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteConfig(QWidget *parent = 0);
    ~RemoteConfig();

    void setRemote(Repository *repository, QString remoteUUID = "");

public slots:
    void refresh();

private slots:
    void setRemoteDescription(QString description);
    void setRemoteTrustStatus(int level);
    void setRemoteWantedExpression(QString expr);
    void addGroup(QString group);
    void removeGroup(QString group);

private:
    Ui::RemoteConfig *ui;
    QString              remoteUUID;
    QPointer<Repository> repository;
    QPointer<WantedExpr> wantedExpr;
    QString              oldWantedExpr;
};

#endif // REMOTECONFIG_H
