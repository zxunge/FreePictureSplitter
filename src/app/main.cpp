// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "mainwindow.h"
#include "app_version.h"

#include "utils/jsonconfigitems.h"
#include "utils/thememanager.h"
#include "utils/languagemanager.h"
#include "utils/stdpaths.h"
#include "utils/fonts.h"
#include "utils/misc.h"

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

#include <cstdlib>

using namespace Qt::Literals::StringLiterals;

#if defined(__MINGW32__) || defined(__MINGW64__)
// Loader for crash helper: Dr.MinGW
[[nodiscard]] inline bool loadExcHndl()
{
    // Load the DLL
    QLibrary *exchndl = new QLibrary(u"exchndl.dll"_s);
    Q_ASSERT(exchndl);

    if (exchndl->load()) {
        typedef void (*ProtoExcHndlInit)();
        typedef bool (*ProtoExcHndlSetLogFileNameA)(const char *);
        ProtoExcHndlInit excHndlInit =
                reinterpret_cast<ProtoExcHndlInit>(exchndl->resolve("ExcHndlInit"));
        ProtoExcHndlSetLogFileNameA excHndlSetLogFileNameA =
                reinterpret_cast<ProtoExcHndlSetLogFileNameA>(
                        exchndl->resolve("ExcHndlSetLogFileNameA"));
        if (excHndlInit && excHndlSetLogFileNameA) {
            excHndlInit();
            if (excHndlSetLogFileNameA(Util::CRASHREPORT_FILE_NAME))
                return true;
            // Do not free the DLL module
        }
    }
    delete exchndl;
    return false;
}
#endif // __MINGW32__ || __MINGW64__

QtMessageHandler originalHandler = nullptr;

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen((Util::dataDir() % '/' % Util::LOG_FILE_NAME).toUtf8().constData(), "a");
    if (f) {
        fprintf(f, "%s\n", qPrintable(message));
        fflush(f);
    }

    if (originalHandler)
        originalHandler(type, context, msg);
}

[[nodiscard]] inline bool loadConfigurations()
{
    QFile cfgFile(Util::dataDir() % '/' % Util::CONFIG_FILE_NAME);
    QString jsonCfgStr;
    if (cfgFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream ts(&cfgFile);
        jsonCfgStr = ts.readAll();
    } else {
        QMessageBox::critical(nullptr, qAppName(),
                              QObject::tr("Error creating/opening configuration file."),
                              QMessageBox::Close);
        return false;
    }

    // First run?
    if (jsonCfgStr.isEmpty())
        Util::setDefConf(g_appConfig);
    else {
        const auto result = rfl::json::read<Util::Config>(jsonCfgStr.toStdString());
        if (!result) {
            QMessageBox::critical(nullptr, qAppName(),
                                  QObject::tr("Error parsing configuration file: %1.")
                                          .arg(QString::fromStdString(result.error().what())),
                                  QMessageBox::Close);
            return false;
        }
        g_appConfig = result.value();

        // Check for version differences, as they will cause weird compatibility problems.
        // We promise not to change the interfaces when updating the micro version.
        if (g_appConfig.app.majorVersion != APP_VERSION_MAJOR
            || g_appConfig.app.minorVersion != APP_VERSION_MINOR) {
            QMessageBox::warning(nullptr, qAppName(),
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
        QMessageBox::critical(nullptr, qAppName(),
                              QObject::tr("Error loading module: exchndl.dll."),
                              QMessageBox::Close);
        return false;
    }
#endif // __MINGW32__ || __MINGW64__
    Util::setAppFont(QLocale::system(), a);

    if (!loadConfigurations())
        return false;
    Util::LanguageManager::instance().installTranslators();
    return true;
}

[[nodiscard]] inline bool saveConfigurations()
{
    // Save configuration changes to file
    const QString jsonCfgStr = QString::fromStdString(rfl::json::write(g_appConfig));
    QFile cfgFile(Util::dataDir() % '/' % Util::CONFIG_FILE_NAME);
    if (!cfgFile.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text)) {
        QMessageBox::warning(nullptr, qAppName(),
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
        FILE *dummy = nullptr;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
    }
#endif

    qSetMessagePattern(u"%{time [yyyy-MM-dd hh:mm:ss.z]} [%{type}] %{message}"_s);
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);

    QCoreApplication::setApplicationVersion(App::Constants::APP_VERSION_STR);
    QCoreApplication::setApplicationName(App::Constants::APP_NAME);
    QCoreApplication::setOrganizationName(u"zxunge"_s);
    QGuiApplication::setApplicationDisplayName(App::Constants::APP_NAME);

    QApplication a(argc, argv);
    originalHandler = qInstallMessageHandler(logToFile);
    QGuiApplication::setWindowIcon(QIcon(u":/icons/fps.ico"_s));

    qInfo("---------------");
    if (!configureApplication(&a))
        std::exit(EXIT_FAILURE);

    qInfo("Application Started...");

    MainWindow w;
    // Load styles after the main window is created
    if (!Util::ThemeManager::instance().setAppSkin(g_appConfig.app.skin)) {
        QMessageBox::warning(nullptr, qAppName(),
                             QObject::tr("Could NOT load skin %1.").arg(g_appConfig.app.skin),
                             QMessageBox::Close);
        std::exit(EXIT_FAILURE);
    }
    qInfo("Skin loaded.");
    w.show();
    int ret = a.exec();
    if (!saveConfigurations())
        std::exit(EXIT_FAILURE);

    qInfo("Application has finished successfully.");
    qInfo("---------------");
    return ret;
}