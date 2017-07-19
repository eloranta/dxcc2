#include <QtSql>
#include <QtWidgets>
#include "database.h"

bool createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dxcc.sqlite");
    if (!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),  // TODO: error handling
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    return true;
}

