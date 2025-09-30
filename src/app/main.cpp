// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "mainwindow.h"
#include "globaldefs.h"

#include "utils/jsonconfigitems.h"
#include "utils/skins.h"
#include "utils/stdpaths.h"
#include "utils/fonts.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFile>
#include <QObject>
#include <QtAssert>
#include <QtLogging>
#if defined(__MINGW32__) || defined(__MINGW64__)
#  include <QLibrary>
#endif // __MINGW32__ || __MINGW64__

#ifdef Q_OS_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif // Q_OS_WIN
#include <rfl/json.hpp>
#include <SingleApplication>

#include <cstdlib>

using namespace Qt::Literals::StringLiterals;

#if defined(__MINGW32__) || defined(__MINGW64__)
// Loader for crash helper: Dr.MinGW
[[nodiscard]] inline bool loadExcHndl()
{
    // Load the DLL
    QLibrary *exchndl{ new QLibrary(u"exchndl.dll"_s) };
    Q_ASSERT(exchndl);

    if (exchndl->load()) {
        typedef void (*ProtoExcHndlInit)();
        typedef bool (*ProtoExcHndlSetLogFileNameA)(const char *);
        ProtoExcHndlInit excHndlInit{ reinterpret_cast<ProtoExcHndlInit>(
                exchndl->resolve("ExcHndlInit")) };
        ProtoExcHndlSetLogFileNameA excHndlSetLogFileNameA{
            reinterpret_cast<ProtoExcHndlSetLogFileNameA>(
                    exchndl->resolve("ExcHndlSetLogFileNameA"))
        };
        if (excHndlInit && excHndlSetLogFileNameA) {
            excHndlInit();
            if (excHndlSetLogFileNameA("crashreport.rpt"))
                return true;
            // Do not free the DLL module
        }
    }
    delete exchndl;
    return false;
}
#endif // __MINGW32__ || __MINGW64__

QtMessageHandler originalHandler{};

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message{ qFormatLogMessage(type, context, msg) };
    static FILE *f{ fopen((Util::getDataDir() % LOG_FILENAME).toUtf8().constData(), "a") };
    if (f) {
        fprintf(f, "%s\n", qPrintable(message));
        fflush(f);
    }

    if (originalHandler)
        originalHandler(type, context, msg);
}

inline void loadTranslations(QApplication *a)
{
    Q_ASSERT(a);
    QTranslator *qtTranslator{ new QTranslator() }, *appTranslator{ new QTranslator() };
    if (qtTranslator->load(QLocale::system(), u"qt"_s, u"_"_s, Util::getTranslationsDir()))
        a->installTranslator(qtTranslator);

    if (appTranslator->load(QLocale::system(), fpsAppName, u"_"_s, Util::getTranslationsDir()))
        a->installTranslator(appTranslator);
    // Do not free translators.
}

[[nodiscard]] inline bool loadConfigurations()
{
    QFile cfgFile(Util::getDataDir() % CONFIG_FILENAME);
    QString jsonCfgStr;
    if (cfgFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream ts(&cfgFile);
        jsonCfgStr = ts.readAll();
    } else {
        QMessageBox::critical(nullptr, fpsAppName,
                              QObject::tr("Error creating/opening configuration file."),
                              QMessageBox::Close);
        return false;
    }

    // First run?
    if (jsonCfgStr.isEmpty())
        Util::setDefConf(appConfig);
    else {
        const auto result{ rfl::json::read<Util::Config>(jsonCfgStr.toStdString()) };
        if (!result) {
            QMessageBox::critical(nullptr, fpsAppName,
                                  QObject::tr("Error parsing configuration file: %1.")
                                          .arg(QString::fromStdString(result.error().what())),
                                  QMessageBox::Close);
            return false;
        }
        appConfig = result.value();

        // Check for version differences, as they will cause weird compatibility problems.
        // We promise not to change the interfaces when updating the micro version.
        if (appConfig.app.majorVersion != fpsVersionMajor
            || appConfig.app.minorVersion != fpsVersionMinor) {
            QMessageBox::warning(nullptr, fpsAppName,
                                 QObject::tr("Configuration file\'s version doesn\'t match, try "
                                             "deleting it after backuping."),
                                 QMessageBox::Close);
            return false;
        }
    }
    return true;
}

[[nodiscard]] inline bool configureApplication(QApplication *a)
{
    Q_ASSERT(a);
#if defined(__MINGW32__) || defined(__MINGW64__)
    if (!loadExcHndl()) {
        QMessageBox::critical(nullptr, fpsAppName,
                              QObject::tr("Error loading module: exchndl.dll."),
                              QMessageBox::Close);
        return false;
    }
#endif // __MINGW32__ || __MINGW64__
    Util::setAppFont(QLocale::system(), a);

    if (!loadConfigurations())
        return false;
    loadTranslations(a);

    // Load styles
    if (!Util::setAppSkin(a, QString::fromStdString(appConfig.app.style))) {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error loading skin: %1.").arg(appConfig.app.style),
                             QMessageBox::Close);
        return false;
    }
    qInfo("Skin loaded.");
    return true;
}

[[nodiscard]] inline bool saveConfigurations()
{
    // Save configuration changes to file
    const QString jsonCfgStr{ QString::fromStdString(rfl::json::write(appConfig)) };
    QFile cfgFile(Util::getDataDir() % CONFIG_FILENAME);
    if (!cfgFile.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text)) {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error writing to configuration file."),
                             QMessageBox::Close);
        return false;
    }
    QTextStream ts(&cfgFile);
    ts << jsonCfgStr;
    cfgFile.close();
    return true;
}

//---------- Main Execution ----------
int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && (!defined(Q_CC_MINGW) || __GNUC__ >= 5)
    // Make console output work on Windows, if running in a console.
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        FILE *dummy{};
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
    }
#endif

    qSetMessagePattern(u"%{time [yyyy-MM-dd hh:mm:ss.z]} [%{type}] %{message}"_s);
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);

    QCoreApplication::setApplicationVersion(fpsVersionFull);
    QCoreApplication::setApplicationName(fpsAppName);
    QCoreApplication::setOrganizationName(u"zxunge"_s);
    QGuiApplication::setApplicationDisplayName(fpsAppName);

    SingleApplication a(argc, argv);
    originalHandler = qInstallMessageHandler(logToFile);
    QGuiApplication::setWindowIcon(QIcon(u":/icons/fps.ico"_s));

    qInfo("---------------");
    if (!configureApplication(&a))
        std::exit(EXIT_FAILURE);

    qInfo("Application Started...");

    MainWindow w;
    w.show();
    int ret{ a.exec() };
    if (!saveConfigurations())
        std::exit(EXIT_FAILURE);

    qInfo("Application has finished successfully.");
    qInfo("---------------");
    return ret;
}
