#include <QApplication>
#include <QtGlobal>
#include "application.h"
#include "repowindow.h"
#include "repository.h"
#include "config_to_json.h"
#include "repositorychooser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(qgetenv("GIT_ANNEX_GUI_EDITOR") == "config_to_json") {
        return config_to_json(a.arguments().value(1));
    }

    Application app;
    return app.exec();
}
