#ifndef QJSONSERIALIZER_HPP__
#define QJSONSERIALIZER_HPP__


///ͨ��Json���л��ӿ�

#include <QJsonDocument>
#include <QFile>
#include <msserialize/qjsonnode.hpp>
#include <msserialize/msarchive_qt.hpp>
#include <msserialize/msnodeapt_qt.hpp>
#include <msserialize/msnodeapt.hpp>

namespace MSRPC
{

template<class T>
QByteArray ToJsonS(const T& t, bool bFormat = false)
{
	//���л�
	QJsonDocument doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	//���json�ַ���
	return doc.toJson(bFormat ? QJsonDocument::Indented 
		: QJsonDocument::Compact);
}

template<class T>
bool FromJsonS(T& t, QByteArray& strJson)
{
	QJsonDocument doc = QJsonDocument::fromJson(strJson);

	MSRPC::OJsonArc::Node objO(&doc);
	MSRPC::OJsonArc oa(objO);

	if (objO)
	{
		oa & t;
	}

	return objO;
}

template<class T>
bool ToJsonFile(const T& t, const QString& strFilePath, bool bFormat = true)
{
	bool bRet(false);

	//���л�
	QJsonDocument doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	//���json�ַ���
	QByteArray strJson = doc.toJson(bFormat 
		? QJsonDocument::Indented : QJsonDocument::Compact);

	QFile f(strFilePath);
	if (f.open(QFile::WriteOnly))
	{
		bRet = f.write(strJson) == strJson.size();
	}

	return bRet;
}

template<class T>
bool FromJsonFile(T& t, const QString& strFilePath)
{
	QJsonDocument doc;
	QFile f(strFilePath);
	if (f.open(QFile::ReadOnly))
	{
		QByteArray strJson = f.readAll();
		doc = QJsonDocument::fromJson(strJson);
	}

	MSRPC::OJsonArc::Node objO(&doc);
	MSRPC::OJsonArc oa(objO);

	if (objO)
	{
		oa & t;
	}

	return objO;
}

}


#endif // QJSONSERIALIZER_HPP__
