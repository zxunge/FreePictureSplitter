// QJSONConfig -- A Qt JSON config parser
// By: zxunge 
// Available at: https://github.com/zxunge/QJSONConfig

#include "qjsonconfig.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

// Merge QJsonObject recursively
// If there is an item that is a child of something, 
// merge it with the existing one rather than just covering.
static void mergeJsonObjects(QJsonObject &target, const QJsonObject &source) 
{
    for (auto it { source.begin() }; it != source.end(); ++it) 
    {
        QString key { it.key() };
        QJsonValue sourceValue { it.value() };
        if (target.contains(key)) 
        {
            QJsonValue targetValue { target.value(key) };
            // If the current key values are objects in both target and source, then merge them recursively
            if (targetValue.isObject() && sourceValue.isObject()) 
            {
                QJsonObject mergedObject { targetValue.toObject() };
                mergeJsonObjects(mergedObject, sourceValue.toObject());
                target.insert(key, mergedObject);
            } 
            else
                // Else, cover target with source
                target.insert(key, sourceValue);
        } 
        else
            // Directly insert a non-existing key
            target.insert(key, sourceValue);
    }
}

// For recursive reading
static void read(QString finalKey, const QJsonObject &obj, QSettings::SettingsMap &map)
{
    for (const QString &key : obj.keys())
    {
        if (obj[key].isObject())
        {
            finalKey += key + "/";
            read(finalKey, obj[key].toObject(), map);
        }
        else
        {
            finalKey += key;
            map.insert(finalKey, obj[key].toVariant());
        }
    }
}

/* static */ bool QJSONConfig::readFunc(QIODevice &device, QSettings::SettingsMap &map)
{
    QTextStream stream(&device);
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    stream.setEncoding(QStringConverter::Utf8);
#else
    stream.setCodec("UTF-8");
#endif
    QString data { stream.readAll() };
    QJsonParseError jsonError;
    QJsonDocument jsonDoc { QJsonDocument::fromJson(data.toUtf8(), &jsonError) };

    if (jsonDoc.isNull())
    {
        QJCFG_WARNING() << QObject::tr("Null config json.");
        return false;
    }
    if (jsonError.error != QJsonParseError::NoError)
    {
        QJCFG_WARNING() << QObject::tr("JSON parse error:") << jsonError.errorString();
        return false;
    }
    if (jsonDoc.isEmpty())
    {
        map = QSettings::SettingsMap();
        return true;
    }
    // We just read from an object.
    if (!jsonDoc.isObject()) 
    {
        QJCFG_WARNING() << QObject::tr("json's not object.");
        return false;
    }
    
    read("", jsonDoc.object(), map);
    return true;
}

/* static */ bool QJSONConfig::writeFunc(QIODevice &device, const QSettings::SettingsMap &map)
{
    QTextStream stream(&device);
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    stream.setEncoding(QStringConverter::Utf8);
#else
    stream.setCodec("UTF-8");
#endif
    QJsonObject rootObj;
    QJsonDocument jsonDoc;
    
    // Generate JSON data
    // We also want a nested structure
    for (QMap<QString, QVariant>::const_iterator itor = map.constBegin(); itor != map.constEnd(); ++itor)
    {
        // Already root?
        if (itor.key().contains('/'))
        {
            QStringList keys {itor.key().split('/')};
            QJsonObject currentObj, superObj, tempObj;

            // Build the deepest QJsonObject
            currentObj.insert(keys.last(), QJsonValue::fromVariant(itor.value()));

            // Build the outside QJsonObject
            for (
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                qsizetype
#else
                int
#endif
                i {keys.size() - 2}; i > 0; --i) 
            {
                superObj[keys[i]] = currentObj;
                currentObj.swap(superObj);
                superObj = QJsonObject();
            }
            tempObj.insert(keys[0], currentObj);
            mergeJsonObjects(rootObj, tempObj);
        }
        else
            rootObj.insert(itor.key(), QJsonValue::fromVariant(itor.value()));
    }
        
    jsonDoc.setObject(rootObj);
    stream << jsonDoc.toJson(QJsonDocument::Indented);
    return true;
}

QVariant QJSONConfig::getValue(const QString& key, const QVariant& defaultValue) const
{
    return m_interSettings->value(key, defaultValue);
}

void QJSONConfig::setValue(const QString& key, const QVariant& value)
{
    m_interSettings->setValue(key, value);
}

void QJSONConfig::sync()
{
    m_interSettings->sync();
}

void QJSONConfig::clear()
{
    m_interSettings->clear();
}

bool QJSONConfig::empty() const
{
    return m_interSettings->allKeys().empty();
}

// It is the original keys stored in QSettingsMap, with '/' in it.
QStringList QJSONConfig::allFinalKeys() const
{
    return m_interSettings->allKeys();
}

// All primary keys.
QStringList QJSONConfig::allKeys() const
{
    QStringList finalKeys { m_interSettings->allKeys() },
                existingKeys, results;
    for (const QString &key : finalKeys)
    {
        if (key.contains('/'))
        {
            QString mainKey { key.split('/')[0] };
            if (existingKeys.contains(mainKey))
                continue;
            else
            {
                existingKeys << mainKey;
                results << mainKey;
            }
        }
        else
            results << key;
    }
    return results;
}

// Something's children.
QStringList QJSONConfig::childKeys(const QString &parent) const
{
    QStringList finalKeys { m_interSettings->allKeys() },
                children;
    for (const QString &key : finalKeys)
    {
        // parent is a root key?
        if (key.startsWith(parent + "/"))
            children << key.mid(QString(parent + "/").size());
        // parent is a sub-key?
        else if (key.contains("/" + parent + "/"))
            children << key.mid(key.indexOf("/" + parent + "/") + QString("/" + parent + "/").size());
        // No children...
        else if (key == parent || key.endsWith("/" + parent))
            continue;
        // No such key!
        else
            continue;
    }
    return children;
}

QString	QJSONConfig::fileName() const
{
    return m_interSettings->fileName();
}

void QJSONConfig::remove(const QString &key)
{
    m_interSettings->remove(key);
}

bool QJSONConfig::contains(const QString &key) const
{
    return m_interSettings->contains(key);
}
