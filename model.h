#ifndef MODEL_H
#define MODEL_H

#include <QtSql>

class Model : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    Model() : QSqlRelationalTableModel() {}
    void initialize();
public slots:
signals:
protected:
private:
};

#endif
