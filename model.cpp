#include "model.h"

void Model::readDxccTextFile()
{
    QSqlQuery query;
    query.exec("create table if not exists entity ("
               "id int primary key,"
               "prefix varchar(20),"
               "entity varchar(20)"
               ")");

    QFile inputFile("dxcc.txt");
    QTextStream input(&inputFile);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        // skip header lines
       input.readLine();
       input.readLine();
       input.readLine();

       while (!input.atEnd())
       {
           QString line = input.readLine();
           QString prefix = line.mid(0, 24).trimmed();
           QString entity = line.mid(24, 35).trimmed();
           int id = line.mid(77, 3).toInt();
           query.exec(QString("insert into entity (id, prefix, entity)"
                "values(%1, '%2', '%3')").arg(id).arg(prefix.toStdString().c_str()).arg(entity.toStdString().c_str()));
       }
       inputFile.close();
    }
}

void Model::initialize()
{
    readDxccTextFile();
    QSqlQuery query;
    query.exec("create table if not exists dxcc ("
               "id int primary key,"
               "prefix varchar(20),"
               "entity varchar(20),"
               "deleted integer,"
               "mixed integer,"
               "phone integer,"
               "cw integer,"
               "data integer,"
               "sat integer,"
               "m160 integer,"
               "m80 integer,"
               "m40 integer,"
               "m30 integer,"
               "m20 integer,"
               "m17 integer,"
               "m15 integer,"
               "m12 integer,"
               "m10 integer,"
               "m6 integer,"
               "m2 integer"
               ")");

    setTable("dxcc");
    setRelation(1, QSqlRelation("entity", "id", "prefix"));
    setRelation(2, QSqlRelation("entity", "id", "entity"));
    setEditStrategy(QSqlTableModel::OnFieldChange);
    query.exec(QString("insert into dxcc (id, prefix, entity)"
                       "values(1, 1, 1)"));
    select();
}
