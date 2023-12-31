#include "../../GUI/headers/player/pmplayerview.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("YMBuddy");
    QCoreApplication::setOrganizationDomain("github.com/jurikimurik");
    QCoreApplication::setApplicationName("PMP");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PMP_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    PMPlayerView w;
    w.show();
    return a.exec();
}
