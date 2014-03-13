#include <QtQuick>

#include "person.h"
#include "company.h"

#include <sailfishapp.h>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>

#include "ConsoleAppender"
#include "ColorConsoleAppender"
#include "SimpleTimeLayout"
#include "Logger"
#include "LogManager"

#include "SystemlogAppender"
#include "Level"
#include "DailyRollingFileAppender"

#include "qmllogger.h"

void initLogging()
{
    // Normally you call it all from .conf properties, but you can instantiate it manually too

    Log4Qt::LogManager::rootLogger();

    // Note that it doesn't work for QML logs from device
    Log4Qt::LogManager::setHandleQtMessages(true);


    Log4Qt::SimpleTimeLayout *p_layout = new Log4Qt::SimpleTimeLayout();

    p_layout->setName(QLatin1String("root layout"));
    p_layout->activateOptions();

    // Create an appender
    Log4Qt::ColorConsoleAppender *p_consoleAppender = new Log4Qt::ColorConsoleAppender(p_layout, Log4Qt::ColorConsoleAppender::STDOUT_TARGET);
    p_consoleAppender->setName(QLatin1String("root console appender"));
    p_consoleAppender->activateOptions();

    Log4Qt::SystemLogAppender *p_syslogAppender = new Log4Qt::SystemLogAppender();
    p_syslogAppender->setServiceName("journalctl");
    p_syslogAppender->setName(QLatin1String("root sysylog appender"));
    p_syslogAppender->setLayout(p_layout);

    // Root logger gets all levels of logs, but let's send only
    // important ones to syslog
    p_syslogAppender->setThreshold(Log4Qt::Level(Log4Qt::Level::ERROR_INT));

    p_syslogAppender->activateOptions();

    Log4Qt::DailyRollingFileAppender *p_fileAppender =
            new Log4Qt::DailyRollingFileAppender();
    p_fileAppender->setFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                            + "/harbour-log4qtdemo/harbour-log4qtdemo.log");
    p_fileAppender->setDatePattern(Log4Qt::DailyRollingFileAppender::DAILY_ROLLOVER);
    p_fileAppender->setLayout(p_layout);
    p_syslogAppender->setName(QLatin1String("root file appender"));
    p_fileAppender->activateOptions();


    // Set appenders on root logger
    Log4Qt::Logger::rootLogger()->addAppender(p_consoleAppender);
    Log4Qt::Logger::rootLogger()->addAppender(p_syslogAppender);
    Log4Qt::Logger::rootLogger()->addAppender(p_fileAppender);

    Log4Qt::Logger::logger(QLatin1String("Main Logger"))->info("Logging started");

    bool handingMessages = Log4Qt::LogManager::handleQtMessages();
    qDebug() << "Intercepting messages from qDebug is " << handingMessages;
    qDebug() << "temp loc" << QStandardPaths::standardLocations(QStandardPaths::TempLocation);
    qDebug() << "config loc" << QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    qDebug() << "cache loc" << QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    qDebug() << "generic cache loc" << QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation);
    qDebug() << "data loc" << QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    qDebug() << "generic data loc" << QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    qDebug() << "app name is " << QCoreApplication::applicationName();
}


int main(int argc, char *argv[])
{

    initLogging();
//    Person billGates("Bill Gates");
//    Person stevenElop("Steven Elop");
//    Company microsoft("Microsoft");
//    microsoft.setCeo(&billGates);
//    microsoft.setCeo(&stevenElop);

    qmlRegisterType<Person>("harbour.log4qtdemo", 0, 1, "Person");
    qmlRegisterType<Company>("harbour.log4qtdemo", 0, 1, "Company");
    qmlRegisterType<QmlLogger>("harbour.log4qtdemo", 0, 1, "Logger");
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    view->setSource(SailfishApp::pathTo("qml/main.qml"));
//    view->rootContext()->setContextProperty("appVersion", APP_VERSION);
//    view->rootContext()->setContextProperty("appBuildNum", APP_BUILDNUM);
//    view->engine()->addImportPath(SailfishApp::pathTo("qml/components").toString());
//    view->engine()->addImportPath(SailfishApp::pathTo("lib").toString());

    view->show();

    return app->exec();
}
