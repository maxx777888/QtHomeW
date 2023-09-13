#include <QCoreApplication>
#include <QTcpSocket>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpSocket socket;
    QSqlQuery query;

    return a.exec();
}
