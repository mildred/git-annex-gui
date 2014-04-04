#ifndef WANTEDEXPR_H
#define WANTEDEXPR_H

#include <QObject>

class WantedExpr : public QObject
{
    Q_OBJECT
public:
    explicit WantedExpr(QObject *parent = 0);
    explicit WantedExpr(QString expr, QObject *parent = 0);

    enum {
        MANUAL,
        ALL,
        NONE,
        CUSTOM
    };

    QString expression() const;
    int     mode() const;
    QString customExpression() const;
    bool    includeOldVersions() const;
    bool    includeFileCopies() const;

    QString description();

signals:
    void expressionChanged(QString expr);

public slots:
    void setExpression(QString expr);
    void setMode(int mode, QString customExpr = "");
    void setIncludeOldVersions(bool include);
    void setExcludeOldVersions(bool exclude);
    void setIncludeFileCopies(bool include);
    void setExcludeFileCopies(bool exclude);

private:
    static int modeForExpression(QString expr);
    static QString expressionForMode(int mode, QString customExpr, bool excludeOldVersions);

    struct {
        int     mode;
        QString customExpr;
        bool    excludeOldVersions;
        bool    includeFileCopies;
    } m;
};

#endif // WANTEDEXPR_H
