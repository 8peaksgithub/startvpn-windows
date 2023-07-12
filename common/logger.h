#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QVector>

namespace Start {

class Logger : public QObject
{
    Q_OBJECT

public:
    enum class MessageType {
        DEBUG,
        NORMAL,
        INFO,
        WARNING,
        CRITICAL
    };
    Q_DECLARE_FLAGS(MessageTypes, MessageType)

    enum class ComponentType {
        OCONNECT,
        GNUTLS,
        VPNC,
        GUI,
        UNKNOWN
    };

    struct Message {
        qint64 timeStamp;
        MessageType messageType;
        ComponentType componentType;
        QString text;
        int id;
        Qt::HANDLE threadId;
    };

    static Logger& instance()
    {
        static Logger logger;
        return logger;
    }

    void addMessage(const QString& message,
                    const MessageType& type = MessageType::NORMAL,
                    const ComponentType& componentType = ComponentType::UNKNOWN);
    QVector<Message> getMessages(int lastKnownId = -1) const;
    void clear();

signals:
    void newLogMessage(const Logger::Message& message);

public slots:

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger m_logger;
    int m_messageCounter;
    QVector<Message> m_messages;
    mutable QReadWriteLock m_lock;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::MessageTypes)

} // namespace Symlex

Q_DECLARE_METATYPE(Start::Logger::Message)

#endif // LOGGER_H
