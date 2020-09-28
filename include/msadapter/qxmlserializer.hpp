#ifndef QJSONSERIALIZER_HPP__
#define QJSONSERIALIZER_HPP__


///ͨ��Json���л��ӿ�

#include <QDomDocument>
#include <QFile>
#include <msserialize/qxmlnode.hpp>
#include <msserialize/msarchive_qt.hpp>
#include <msserialize/msnodeapt_qt.hpp>
#include <msserialize/msnodeapt.hpp>

namespace MSRPC
{

template<class T>
QByteArray ToXmlS(const T& t, const QString& strRootName, int nIndent = -1)
{
	//���л�
	QDomDocument doc;
	MSRPC::IXmlArc::Node nObjI(&doc, strRootName);
	MSRPC::IXmlArc ia(nObjI);
	ia & t;

	//���json�ַ���
	return doc.toByteArray(nIndent);
}

template<class T>
bool FromXmlS(T& t, QByteArray& strXml)
{
	bool bRet(false);
	QDomDocument doc;
	if (doc.setContent(strXml))
	{
		MSRPC::OXmlArc::Node objO(&doc);
		MSRPC::OXmlArc oa(objO);

		if (objO)
		{
			oa & t;
			bRet = true;
		}
	}

	return bRet;
}

template<class T>
bool ToJsonFile(const T& t, const QString& strFilePath, const QString& strRootName, int nIndent = -1)
{
	bool bRet(false);

	//���л�
	QDomDocument doc;
	MSRPC::IXmlArc::Node nObjI(&doc, doc.createElement(strRootName));
	MSRPC::IXmlArc ia(nObjI);
	ia & t;

	//���json�ַ���
	QByteArray strXml = doc.toByteArray(nIndent);

	QFile f(strFilePath);
	if (f.open(QFile::WriteOnly))
	{
		bRet = f.write(strXml) == strXml.size();
	}

	return bRet;
}

template<class T>
bool FromJsonFile(T& t, const QString& strFilePath)
{
	bool bRet(false);

	QDomDocument doc;
	QFile f(strFilePath);
	if (f.open(QFile::ReadOnly) && doc.setContent(f))
	{
		MSRPC::OXmlArc::Node objO(&doc);
		MSRPC::OXmlArc oa(objO);

		if (objO)
		{
			oa& t;
			bRet = true;
		}
	}

	return bRet;
}

}


#endif // QJSONSERIALIZER_HPP__
