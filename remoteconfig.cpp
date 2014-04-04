#include "remoteconfig.h"
#include "repository.h"
#include "wantedexpr.h"
#include "ui_remoteconfig.h"

RemoteConfig::RemoteConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteConfig),
    wantedExpr(new WantedExpr(this))
{
    ui->setupUi(this);
    connect(ui->slimLineEdit_description, SIGNAL(editStarting()), this, SLOT(refresh()));
    connect(ui->slimLineEdit_description, SIGNAL(textChanged(QString)), this, SLOT(setRemoteDescription(QString)));
    connect(ui->comboBox_trust, SIGNAL(currentIndexChanged(int)), this, SLOT(setRemoteTrustStatus(int)));
    //connect(wantedExpr, SIGNAL(expressionChanged(QString)), this, SLOT(setRemoteWantedExpression(QString)));
    connect(ui->remoteWantedEdit, SIGNAL(wantedExprModified(QString)), this, SLOT(setRemoteWantedExpression(QString)));
    //connect(ui->tags_groups, SIGNAL(tagAdded(QString)), this, SLOT(addGroup(QString)));
    //connect(ui->tags_groups, SIGNAL(tagRemoved(QString)), this, SLOT(removeGroup(QString)));
}

RemoteConfig::~RemoteConfig()
{
    delete ui;
}

void RemoteConfig::setRemote(Repository *repository, QString remoteUUID)
{
    this->remoteUUID = remoteUUID;
    this->repository = repository;

    if(remoteUUID.isEmpty()) {
        ui->checkBox_ignore->hide();
        ui->checkBox_sync->hide();
        ui->checkBox_readonly->hide();
    }
}

void RemoteConfig::refresh()
{
    wantedExpr->setExpression(oldWantedExpr = repository->remoteWantedExpr(remoteUUID));
    ui->remoteWantedEdit->setWantedExpr(wantedExpr);
    ui->slimLineEdit_description->setText(repository->remoteDescription(remoteUUID));
    ui->comboBox_trust->setCurrentIndex(repository->remoteTrustStatus(remoteUUID));
    ui->tags_groups->setTags(repository->remoteGroups(remoteUUID));
}

void RemoteConfig::setRemoteDescription(QString description)
{
    repository->setRemoteDescription(description, remoteUUID);
}

void RemoteConfig::setRemoteTrustStatus(int level)
{
    repository->setRemoteTrustStatus(level, remoteUUID);
}

void RemoteConfig::setRemoteWantedExpression(QString expr)
{
    if(oldWantedExpr == expr) return;
    oldWantedExpr = expr;
    repository->setRemoteWantedExpression(expr, remoteUUID);
}

void RemoteConfig::addGroup(QString group)
{
    repository->addRemoteGroup(group, remoteUUID);
}

void RemoteConfig::removeGroup(QString group)
{
    repository->removeRemoteGroup(group, remoteUUID);
}
