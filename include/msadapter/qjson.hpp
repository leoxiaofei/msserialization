#ifndef QJSONSERIALIZER_HPP__
#define QJSONSERIALIZER_HPP__


#include <QJsonDocument>
#include <QFile>
#include <msserialize/qjsonnode.hpp>
#include <msserialize/msarchive_qt.hpp>
#include <msserialize/msnodeapt_qt.hpp>
#include <msserialize/msnodeapt.hpp>

namespace MSRPC
{
	typedef MSRPC::SeNodeQJson SeNode;
	typedef MSRPC::DeNodeQJson DeNode;
	typedef MSRPC::SeDocQJson SeDoc;
	typedef MSRPC::DeDocQJson DeDoc;

    template<class T, class SEDOC = MSRPC::SeDoc>
    QByteArray ToJsonS(const T& t, unsigned int indent = 0)
    {
        //Serialization

        SEDOC seDoc;
        seDoc << t;

        //Output the JSON string
        return seDoc.Stringify(indent);
    }

    template<class T, class DEDOC = MSRPC::DeDoc>
    bool FromJsonS(T& t, QByteArray& strJson)
    {
        bool bRet(false);

        DEDOC deDoc;

        if (deDoc.Parse(strJson))
        {
            deDoc >> t;
            bRet = true;
        }

        return bRet;
    }

    template<class T, class SEDOC = MSRPC::SeDoc>
    bool ToJsonF(const T& t, const QString& strFilePath, unsigned int indent = 0)
    {
        //Serialization
        SEDOC seDoc;
        seDoc << t;

        return seDoc.Save(strFilePath, indent);
    }

    template<class T, class DEDOC = MSRPC::DeDoc>
    bool FromJsonF(T& t, const QString& strFilePath)
    {
        bool bRet(false);
        DEDOC deDoc;
        if (deDoc.Load(strFilePath))
        {
            deDoc >> t;
            bRet = true;
        }

        return bRet;
    }
}

namespace MSRPC
{
// template<class T>
// QByteArray ToJsonS(const T& t, bool bFormat = false)
// {
// 	MSRPC::SeJsonArc::Node nObjI;
// 	MSRPC::SeJsonArc ia(nObjI);
// 	ia & t;

// 	QJsonDocument doc;
// 	nObjI.finish(&doc);

// 	return doc.toJson(bFormat ? QJsonDocument::Indented 
// 		: QJsonDocument::Compact);
// }

// template<class T>
// bool FromJsonS(T& t, QByteArray& strJson)
// {
// 	QJsonDocument doc = QJsonDocument::fromJson(strJson);

// 	MSRPC::DeJsonArc::Node objO(&doc);
// 	MSRPC::DeJsonArc oa(objO);

// 	if (objO)
// 	{
// 		oa & t;
// 	}

// 	return objO;
// }

// template<class T>
// bool ToJsonFile(const T& t, const QString& strFilePath, bool bFormat = true)
// {
// 	bool bRet(false);

// 	MSRPC::SeJsonArc::Node nObjI;
// 	MSRPC::SeJsonArc ia(nObjI);
// 	ia & t;

// 	QJsonDocument doc;
// 	nObjI.finish(&doc);

// 	QByteArray strJson = doc.toJson(bFormat 
// 		? QJsonDocument::Indented : QJsonDocument::Compact);

// 	QFile f(strFilePath);
// 	if (f.open(QFile::WriteOnly))
// 	{
// 		bRet = f.write(strJson) == strJson.size();
// 	}

// 	return bRet;
// }

// template<class T>
// bool FromJsonFile(T& t, const QString& strFilePath)
// {
// 	QJsonDocument doc;
// 	QFile f(strFilePath);
// 	if (f.open(QFile::ReadOnly))
// 	{
// 		QByteArray strJson = f.readAll();
// 		doc = QJsonDocument::fromJson(strJson);
// 	}

// 	MSRPC::DeJsonArc::Node objO(&doc);
// 	MSRPC::DeJsonArc oa(objO);

// 	if (objO)
// 	{
// 		oa & t;
// 	}

// 	return objO;
// }

}


#endif // QJSONSERIALIZER_HPP__
