/*
 * Description : Cette classe représente chaque client connecté au serveur.
 *               C’est ici que l’objet QTcpSocket se trouve et également ici
 *               qu'on envoie les paquets au client qui lui sont assignés.
 * Version     : 1.0.0
 * Date        : 25.01.2021
 * Auteurs     : Prétat Valentin, Badel Kevin et Margueron Yasmine
*/

#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QReadWriteLock>
#include <QTcpSocket>

class ServerWorker : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerWorker)

public:
    ServerWorker(QObject *parent = nullptr);
    void sendJson(const QJsonObject &jsonData);

    // Getters / setters
    qintptr getSocketDescriptor();
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString getUsername() const;
    void setUsername(const QString &username);
    bool getReady() const;
    void setReady(const bool ready);
    int getGender();
    void setGender(int gender);
    int getTeam();
    void setTeam(int gender);

private:
    // Les  propriétés d'un client
    QTcpSocket *socket;         // Son socket
    QString username;           // Son nom d'utilisateur
    bool ready;                 // S'il est prêt
    int gender;                 // Son genre
    int team;                   // Sa team

    // Les mutable pour les threads
    mutable QReadWriteLock usernameLock;
    mutable QReadWriteLock readyLock;
    mutable QReadWriteLock genderLock;
    mutable QReadWriteLock teamLock;


public slots:
    void disconnectFromClient();

private slots:
    void receiveJson();

signals:
    void jsonRecieved(const QJsonObject &jsonDoc);
    void disconnectedFromClient();
    void error();
    void logMessage(const QString &msg);
};

#endif // SERVERWORKER_H
