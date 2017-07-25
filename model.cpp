#include "model.h"
#include <QFileDialog>
#include <QDebug>
#include <fstream>

namespace
{
    std::string value(std::string &line)
    {
        return line.substr(line.find('>') + 1);
    }
}

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

    query.exec("select id from entity");
    int id = 1;
    while (query.next())
    {
        int entity_id = query.value(0).toInt();
        QSqlQuery query2;
        query2.exec(QString("insert into dxcc (id, prefix, entity) values(%1, %2, %3)").arg(id++).arg(entity_id).arg(entity_id));
    }

    select();
}

void Model::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"), "c:\\ZOWN\\dxcc", tr("Log File (*.adi)"));
//    QString fileName = "c:\\ZOWN\\dxcc\\DXCC_QSLs_20170619_195452.adi"; // TODO:
    std::ifstream inFile;
    inFile.open(fileName.toStdString());
    if (!inFile)
    {
        qDebug() << "Unable to open file " << fileName;
        return;
    }

    // skip header
    std::string line;
    while (std::getline(inFile, line) && line != "<eoh>")
        ;

    // skip empty line
    std::getline(inFile, line);

    int count = 0;

    while (std::getline(inFile, line))
    {
        std::string call = value(line);
        qDebug() << "call: " << QString::fromStdString(call);

        std::getline(inFile, line);
        std::string date = value(line);
        qDebug() << "date: " << QString::fromStdString(date);

        std::getline(inFile, line);
        std::string modeGroup = value(line);
        qDebug() << "mode group: " << QString::fromStdString(modeGroup);

        std::getline(inFile, line);
        std::string mode = value(line);
        qDebug() << "mode: " << QString::fromStdString(mode);

        std::getline(inFile, line);
        std::string band = value(line);
        qDebug() << "band: " << QString::fromStdString(band);

        std::getline(inFile, line);
        std::string dxcc = value(line);
        qDebug() << "dxcc: " << QString::fromStdString(dxcc);
        int dxcc1 = QString::fromStdString(dxcc).toInt();

        std::getline(inFile, line);
        std::string country = value(line);
        qDebug() << "country: " << QString::fromStdString(country);

        std::getline(inFile, line);
        std::string deleted = value(line);
        qDebug() << "deleted: " << QString::fromStdString(deleted);

        QSqlQuery query;
        query.exec(QString("update dxcc set mixed = 1 where entity = %1").arg(dxcc1));
        if (mode == "PHONE")
            query.exec(QString("update dxcc set phone = 1 where entity = %1").arg(dxcc1));
        else if (mode == "CW")
            query.exec(QString("update dxcc set cw = 1 where entity = %1").arg(dxcc1));
        else
            query.exec(QString("update dxcc set data = 1 where entity = %1").arg(dxcc1));

        if (band == "160M")
            query.exec(QString("update dxcc set m160 = 1 where entity = %1").arg(dxcc1));
        else if (band == "80M")
            query.exec(QString("update dxcc set m80 = 1 where entity = %1").arg(dxcc1));
        else if (band == "80M")
            query.exec(QString("update dxcc set m80 = 1 where entity = %1").arg(dxcc1));
        else if (band == "40M")
            query.exec(QString("update dxcc set m40 = 1 where entity = %1").arg(dxcc1));
        else if (band == "30M")
            query.exec(QString("update dxcc set m30 = 1 where entity = %1").arg(dxcc1));
        else if (band == "20M")
            query.exec(QString("update dxcc set m20 = 1 where entity = %1").arg(dxcc1));
        else if (band == "17M")
            query.exec(QString("update dxcc set m17 = 1 where entity = %1").arg(dxcc1));
        else if (band == "15M")
            query.exec(QString("update dxcc set m15 = 1 where entity = %1").arg(dxcc1));
        else if (band == "12M")
            query.exec(QString("update dxcc set m12 = 1 where entity = %1").arg(dxcc1));
        else if (band == "10M")
            query.exec(QString("update dxcc set m10 = 1 where entity = %1").arg(dxcc1));
        else if (band == "6M")
            query.exec(QString("update dxcc set m6 = 1 where entity = %1").arg(dxcc1));
        else if (band == "2M")
            query.exec(QString("update dxcc set m2 = 1 where entity = %1").arg(dxcc1));

        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);

        count++;
    }
    select();

    qDebug() << "count: " << count;

    inFile.close();
}
