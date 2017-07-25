#ifndef MODEL_H
#define MODEL_H

#include <QtSql>

class Model : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    Model() : QSqlRelationalTableModel() {}
    void initialize();
    void fileOpen();
public slots:
signals:
protected:
private:
    void Model::readDxccTextFile();
};

#endif
