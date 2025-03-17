// QJSONConfig -- A Qt JSON config parser
// By: zxunge 
// Available at: https://github.com/zxunge/QJSONConfig

#include <QString>
#include <QSettings>
#include <QVariant>
#include <QVector>
#include <QPair>
#include <QIODevice>
#include <QObject>
#include <QMetaType>
#include <QDebug>
#include <QMessageLogger>

#define QJCFG_WARNING QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).warning

class QJSONConfig
{
private:
    QString m_cfgFile;
    QSettings *m_interSettings;

#ifdef QJSONCFG_PROXY_USED        // Unsafe; incomplete; everywhere goes a []
public:
    // Client class: distinguish between lvalue and rvalue
    class Proxy 
    {
    public:
        Proxy(QJSONConfig& container, QString cfgKey) 
            : container(container), key(cfgKey) {}

        // Convert into value (rvalue)
        operator QVariant() const 
        {
            return container.m_interSettings->value(key);
        }

        operator QString() const 
        {
            return container.m_interSettings->value(key).toString();
        }

        operator int() const 
        {
            if(container.m_interSettings->value(key).
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                typeId()
#else
                type()
#endif
                != QMetaType::Int)
                QJCFG_WARNING() << QObject::tr("Incorrect type.");          
            return container.m_interSettings->value(key).toInt();
        }

        operator double() const 
        {
            if(container.m_interSettings->value(key).
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                typeId()
#else
                type()
#endif
                != QMetaType::Double)
                QJCFG_WARNING() << QObject::tr("Incorrect type.");
            return container.m_interSettings->value(key).toDouble();
        }

        operator bool() const 
        {
            if(container.m_interSettings->value(key).
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                typeId()
#else
                type()
#endif
                != QMetaType::Bool)
                QJCFG_WARNING() << QObject::tr("Incorrect type.");
            return container.m_interSettings->value(key).toBool();
        }

        // Assignment (lvalue)
        Proxy& operator=(QVariant value) 
        {
            container.m_interSettings->setValue(key, value);
            return *this;
        }

    private:
        QJSONConfig& container;
        QString key;
    }; 
#endif

public:
    explicit QJSONConfig(const QString &filePath = "config.json")
        : m_cfgFile(filePath) { QSettings::Format format { QSettings::registerFormat("json", &readFunc, &writeFunc) }; m_interSettings = new QSettings(filePath, format); }
    ~QJSONConfig() { delete m_interSettings; }
    
    void sync();
    void clear();
    bool empty() const;

    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setValue(const QString& key, const QVariant& value = QVariant());
#ifdef QJSONCFG_PROXY_USED
    Proxy operator[](const QString &cfgKey) { return Proxy(*this, cfgKey); }
#endif
    QVariant operator[](const QString &cfgKey) const { return m_interSettings->value(cfgKey); }

    // Key-val related functions
    QStringList allFinalKeys() const;
    QStringList allKeys() const;
    QStringList childKeys(const QString &parent) const;
    QStringList findKey(const QString &key) const;
    QVector<QPair<QString, QVariant>> findValue(const QVariant &val) const;
    void remove(const QString &key);
    bool contains(const QString &key) const;
    
    // Config file related functions
    QString	fileName() const;

    // Fallback functions

private:
    static bool readFunc(QIODevice &device, QSettings::SettingsMap &map);
    static bool writeFunc(QIODevice &device, const QSettings::SettingsMap &map);
};
