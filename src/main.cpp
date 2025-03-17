#include "fpswindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "qjsonconfig.h"

QJSONConfig cfg("conf.json");

int main(int argc, char *argv[])
{
    QGoodWindow::setup();

    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("FreePictureSplitter");
    QCoreApplication::setOrganizationName("zxunge (Grit Clef)");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FreePictureSplitter_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // First time?
    if (cfg.empty())
    {
        cfg.setValue("app/name", "FreePictureSplitter");
        cfg.setValue("app/author", "zxunge (Grit Clef)");
        cfg.setValue("style/isDark", true);
    }

    fpsWindow w;
    if (cfg["style/isDark"].toBool())
        w.setAppDarkTheme();
    w.show();

    int ret = a.exec();
    cfg.sync();
    return ret;
}
