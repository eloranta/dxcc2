#ifndef MODEL_H
#define MODEL_H

#include <QtSql>
class SocketTest;

class Model : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    Model();
    void initialize();
    void fileOpen();
public slots:
signals:
protected:
private:
    void readDxccTextFile();
    SocketTest *socket;
};

#endif
