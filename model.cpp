#include "model.h"

void Model::readDxccTextFile()
{
    QFile inputFile("dxcc.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
           // skip header lines
           in.readLine();
           in.readLine();
           in.readLine();
           QString line = in.readLine();
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
    select();
    setEditStrategy(QSqlTableModel::OnFieldChange);

    query.exec(QString("insert into dxcc (id, prefix, entity, deleted, mixed, phone, cw, data, sat, m160, m80, m40, m30, m20, m17, m15, m12, m10, m6, m2)"
                           "values(1, '', 'SPRATLY ISLANDS', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)"));
}
