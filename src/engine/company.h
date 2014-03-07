#ifndef COMPANY_H
#define COMPANY_H

#include <QObject>

class Person;

class Company : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Person* ceo READ ceo WRITE setCeo NOTIFY ceoChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit Company(const QString& name = QString(), QObject *parent = 0);
    virtual ~Company();
    Person* ceo() const;

    QString name() const;

signals:
    void ceoChanged(Person* arg);

    void nameChanged(QString arg);

public slots:
    void setCeo(Person* arg);

    void setName(QString arg);

private:
    Person* m_ceo;
    QString m_name;
};

#endif // COMPANY_H
