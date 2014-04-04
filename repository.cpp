#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>
#include <QDebug>
#include <QApplication>
#include <QEventLoop>
#include "repository.h"

static void run_command_terminal(QString command, QDir workingDirectory) {
    QString shell = QString("sh -xc '%1; read'").arg(command.replace("'", "'\"'\"'"));
    QProcess::startDetached("gnome-terminal", QStringList() << "-e" << shell, workingDirectory.absolutePath());
}

Repository::Repository(QString path, QObject *parent) :
    QObject(parent)
{
    m.path = path;
    m.path.makeAbsolute();
    m.refreshTimer.setSingleShot(false);
    m.refreshTimer.setInterval(1000);
    m.refreshTimer.start();

    connect(&m.watcher, SIGNAL(fileChanged(QString)), this, SLOT(refreshAssistantRunning()));
    connect(&m.watcher, SIGNAL(directoryChanged(QString)), this, SLOT(refreshAssistantRunning()));
    connect(&m.refreshTimer, SIGNAL(timeout()), this, SLOT(refreshAssistantRunning()));

    refreshAssistantRunning();
}

bool Repository::isAssistantRunning()
{
    refreshAssistantRunning();
    return m.assistantRunning;
}

bool Repository::isDirectMode()
{
    QJsonObject info = annexInfo().object();
    return info["repository mode"].toString() == "direct";
}

QDir Repository::path() const
{
    return m.path;
}

int Repository::numCopies() const
{
    QProcess p;
    QEventLoop loop;
    connect(&p, SIGNAL(finished(int)), &loop, SLOT(quit()));

    p.setWorkingDirectory(m.path.path());
    QStringList args;
    args << "annex" << "numcopies";
    p.start("git", args);

    loop.exec();

    bool ok;
    int numcopies = p.readAll().toInt(&ok);
    if(!ok) numcopies = 1;
    return numcopies;
}

QString Repository::currentRemoteUUID() const
{
    return remoteUUID();
}

QString Repository::remoteUUID(QString remoteUUID) const
{
    QJsonObject info = annexInfo().object();
    QJsonArray a;
    foreach(QJsonValue v, info["trusted repositories"].toArray()) a.append(v);
    foreach(QJsonValue v, info["semitrusted repositories"].toArray()) a.append(v);
    foreach(QJsonValue v, info["untrusted repositories"].toArray()) a.append(v);
    foreach(QJsonValue v, a) {
        if ((remoteUUID.isEmpty() && v.toObject()["here"].toBool())
                || (v.toObject()["uuid"] == remoteUUID)
                || (v.toObject()["description"] == remoteUUID))
            return v.toObject()["uuid"].toString();
    }
    return "";
}

QString Repository::remoteDescription(QString remoteUUID) const
{
    QJsonObject info = annexInfo().object();
    QJsonArray a;
    foreach(QJsonValue v, info["trusted repositories"].toArray()) a.append(v);
    foreach(QJsonValue v, info["semitrusted repositories"].toArray()) a.append(v);
    foreach(QJsonValue v, info["untrusted repositories"].toArray()) a.append(v);
    foreach(QJsonValue v, a) {
        if ((remoteUUID.isEmpty() && v.toObject()["here"].toBool())
                || (v.toObject()["uuid"] == remoteUUID)
                || (v.toObject()["description"] == remoteUUID))
            return v.toObject()["description"].toString();
    }
    return "";
}

QString Repository::remoteWantedExpr(QString remoteUUID) const
{
    if(remoteUUID.isEmpty()) remoteUUID = "here";

    QProcess p;
    QEventLoop loop;
    connect(&p, SIGNAL(finished(int)), &loop, SLOT(quit()));

    p.setWorkingDirectory(m.path.path());
    QStringList args;
    args << "annex" << "wanted" << remoteUUID;
    p.start("git", args);

    loop.exec();

    if(p.exitCode() != 0) return "";
    else return QString(p.readAll()).trimmed();
}

int Repository::remoteTrustStatus(QString remoteUUID) const
{
    QJsonObject info = annexInfo().object();
    foreach(QJsonValue v, info["trusted repositories"].toArray()) {
        if ((remoteUUID.isEmpty() && v.toObject()["here"].toBool())
                || (v.toObject()["uuid"] == remoteUUID)
                || (v.toObject()["description"] == remoteUUID))
            return TRUSTED;
    }

    foreach(QJsonValue v, info["semitrusted repositories"].toArray()) {
        if ((remoteUUID.isEmpty() && v.toObject()["here"].toBool())
                || (v.toObject()["uuid"] == remoteUUID)
                || (v.toObject()["description"] == remoteUUID))
            return SEMITRUSTED;
    }

    foreach(QJsonValue v, info["untrusted repositories"].toArray()) {
        if ((remoteUUID.isEmpty() && v.toObject()["here"].toBool())
                || (v.toObject()["uuid"] == remoteUUID)
                || (v.toObject()["description"] == remoteUUID))
            return UNTRUSTED;
    }

    return DEAD;
}

QStringList Repository::remoteGroups(QString remoteUUID) const
{
    if(remoteUUID.isEmpty()) {
        remoteUUID = currentRemoteUUID();
    }

    QStringList res;
    QJsonObject groupConfig = annexConfig().object()["group"].toObject();
    foreach(QJsonValue v, groupConfig[remoteUUID].toArray()) {
        res << v.toString();
    }

    return res;
}

void Repository::runWebApp()
{
    QStringList args;
    args << "annex" << "webapp";
    QProcess::startDetached("git", args, m.path.path());
}

bool Repository::runAssistant(bool run)
{
    QProcess p;
    QEventLoop loop;
    connect(&p, SIGNAL(finished(int)), &loop, SLOT(quit()));

    p.setWorkingDirectory(m.path.path());
    QStringList args;
    args << "annex" << "assistant";
    if (!run) args << "--stop";
    p.start("git", args);

    loop.exec();

    return p.exitCode() == 0;
}

void Repository::setDirectMode(bool direct)
{
    run_command_terminal(direct ? "git annex direct" : "git annex indirect", m.path.path());
}

void Repository::setRemoteDescription(QString description, QString remote)
{
    if(remote.isEmpty()) remote = "here";
    QProcess::startDetached("git", QStringList() << "annex" << "describe" << remote << description, m.path.path());
}

void Repository::setRemoteTrustStatus(int status, QString remote)
{
    QString statusCommand;
    switch(status) {
    case TRUSTED:     statusCommand = "trust"; break;
    case SEMITRUSTED: statusCommand = "semitrust"; break;
    case UNTRUSTED:   statusCommand = "untrust"; break;
    case DEAD:        statusCommand = "dead"; break;
    default: return;
    }

    if(remote.isEmpty()) remote = "here";
    QProcess::startDetached("git", QStringList() << "annex" << statusCommand << remote, m.path.path());
}

void Repository::setRemoteWantedExpression(QString expr, QString remote)
{
    if(remote.isEmpty()) remote = "here";
    QProcess::startDetached("git", QStringList() << "annex" << "wanted" << remote << expr, m.path.path());
}

void Repository::addRemoteGroup(QString group, QString remote)
{
    if(remote.isEmpty()) remote = "here";
    qDebug() << "group" << remote << group;
    QProcess::startDetached("git", QStringList() << "annex" << "group" << remote << group, m.path.path());
}

void Repository::removeRemoteGroup(QString group, QString remote)
{
    if(remote.isEmpty()) remote = "here";
    qDebug() << "ungroup" << remote << group;
    QProcess::startDetached("git", QStringList() << "annex" << "ungroup" << remote << group, m.path.path());
}

void Repository::setNumCopies(int copies)
{
    QProcess::startDetached("git", QStringList() << "annex" << "numcopies" << QString::number(copies), m.path.path());
}

void Repository::refreshAssistantRunning()
{
    QString pidpath = m.path.filePath(".git/annex/daemon.pid");
    m.watcher.removePath(pidpath);
    if (!m.watcher.addPath(pidpath)) qWarning() << "Could not watch" << pidpath;

    QFile pidfile(pidpath);
    if(!pidfile.exists()) {
        if (m.assistantRunning) {
            m.assistantRunning = false;
            emit assistantStopped();
            emit assistantStateChanged(false);
        }
    } else {
        if(!pidfile.open(QIODevice::ReadOnly)) qWarning() << "Could not open" << pidfile.fileName();
        else {
            long pid = pidfile.readAll().toInt();
            QString procdir = QDir("/proc").filePath(QString::number(pid));
            bool run = QFile::exists(procdir);
            if(run != m.assistantRunning) {
                m.assistantRunning = run;
                emit assistantStateChanged(run);
                if (run) emit assistantStarted();
                else     emit assistantStopped();
            }
        }
    }
}

QJsonDocument Repository::annexInfo() const
{
    QProcess p;
    QEventLoop loop;
    connect(&p, SIGNAL(finished(int)), &loop, SLOT(quit()));

    p.setWorkingDirectory(m.path.path());
    QStringList args;
    args << "annex" << "info" << "--fast" << "-j";
    p.start("git", args);

    loop.exec();

    if(p.exitCode() != 0) return QJsonDocument();
    return QJsonDocument::fromJson(p.readAll());
}

QJsonDocument Repository::annexConfig() const
{
    QProcess p;
    QEventLoop loop;
    connect(&p, SIGNAL(finished(int)), &loop, SLOT(quit()));

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("EDITOR", qApp->applicationFilePath());
    env.insert("GIT_ANNEX_GUI_EDITOR", "config_to_json");

    p.setProcessEnvironment(env);
    p.setWorkingDirectory(m.path.path());
    QStringList args;
    args << "annex" << "vicfg";
    p.start("git", args);

    loop.exec();

    if(p.exitCode() != 0) {
        qWarning() << "Failed to read configuration using git annex vicfg";
        return QJsonDocument();
    }
    QJsonDocument doc = QJsonDocument::fromJson(p.readAll());
    qWarning() << "configuration" << doc;
    return doc;
}
