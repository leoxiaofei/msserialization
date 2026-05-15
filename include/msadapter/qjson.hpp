#ifndef QJSONSERIALIZER_HPP__
#define QJSONSERIALIZER_HPP__


#include <QJsonDocument>
#include <QFile>
#include <msserialize/qjsonnode.hpp>
#include <msserialize/msarchive_qt.hpp>
#include <msserialize/msnodeapt_qt.hpp>
#include <msserialize/msnodeapt.hpp>

// ============================================================================
// QJson adapter namespace
// ============================================================================
namespace MSRPC
{
namespace QJson
{
	typedef MSRPC::SeNodeQJson SeNode;
	typedef MSRPC::DeNodeQJson DeNode;
	typedef MSRPC::SeDocQJson SeDoc;
	typedef MSRPC::DeDocQJson DeDoc;

	template<class T>
	QByteArray ToJsonS(const T& t, unsigned int indent = 0)
	{
		SeDoc seDoc;
		seDoc << t;
		return seDoc.Stringify(indent);
	}

	template<class T>
	bool FromJsonS(T& t, QByteArray& strJson)
	{
		bool bRet(false);
		DeDoc deDoc;
		if (deDoc.Parse(strJson))
		{
			deDoc >> t;
			bRet = true;
		}
		return bRet;
	}

	template<class T>
	bool ToJsonF(const T& t, const QString& strFilePath, unsigned int indent = 0)
	{
		SeDoc seDoc;
		seDoc << t;
		return seDoc.Save(strFilePath, indent);
	}

	template<class T>
	bool FromJsonF(T& t, const QString& strFilePath)
	{
		bool bRet(false);
		DeDoc deDoc;
		if (deDoc.Load(strFilePath))
		{
			deDoc >> t;
			bRet = true;
		}
		return bRet;
	}
}
}

// ============================================================================
// Backward compatibility: skip if already defined by another adapter
// ============================================================================
#ifndef MSRPC_JSON_COMPAT_DEFINED
#define MSRPC_JSON_COMPAT_DEFINED

namespace MSRPC
{
	using namespace MSRPC::QJson;
}

#endif // MSRPC_JSON_COMPAT_DEFINED


#endif // QJSONSERIALIZER_HPP__