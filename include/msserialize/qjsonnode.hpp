#ifndef QJSONNODE_H__
#define QJSONNODE_H__

#include "msarchive.hpp"
#include "msserialize/msbasetypeapt_qt.hpp"

#include <QScopedPointer>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define NODE_WITH_QT 1

namespace MSRPC
{
	// class NJVBase
	// {
	// protected:
	// 	enum { NJVType = 100 };

	// public:
	// 	virtual ~NJVBase() {}
	// 	virtual QJsonValue data() const = 0;
	// 	virtual int type() const = 0;
	// 	virtual void setDoc(QJsonDocument* doc) = 0;
	// };

	// class NJValue : public NJVBase
	// {
	// protected:
	// 	QJsonValue m_data;

	// public:
	// 	virtual QJsonValue data() const
	// 	{
	// 		return m_data;
	// 	}

	// 	virtual void setDoc(QJsonDocument* doc)
	// 	{
	// 		// qt is not supported. 
	// 		Q_ASSERT(false);
	// 	}

	// 	QJsonValue& value()
	// 	{
	// 		return m_data;
	// 	}

	// 	enum {Type = NJVType + 1};
	// 	virtual int type() const { return Type; }

	// };

	class SeNodeQJson
	{
	protected:
		QJsonValueRef m_node;

	public:
		template <class T>
		void in_serialize(const T& tValue)
		{
			m_node = tValue;
		}

		template<class S, class T>
		void in_serialize(const BaseTypeApt<S, T>& tValue)
		{
			in_serialize((S)tValue);
		}

		void in_serialize(const unsigned int& tValue)
		{
			m_node = (int)tValue;
		}

		void in_serialize(const unsigned long long& tValue)
		{
			m_node = (qint64)tValue;
		}

		void in_serialize(const long& tValue)
		{
			m_node = (qint64)tValue;
		}

		void in_serialize(const unsigned long& tValue)
		{
			m_node = (qint64)tValue;
		}

		// void in_serialize(const char* tValue)
		// {
		// 	NJValue* nj = set_value();
		// 	nj->value() = tValue;
		// }

		template <typename T>
		void in_serialize(const StrApt<T>& tValue)
		{
			in_serialize(tValue.Get());
		}


		class NJArray
		{
		private:
			SeNodeQJson* m_parent;
			QJsonArray m_value;

		public:
			NJArray(NJArray&& other) noexcept
				: m_value(std::move(other.m_value))
				, m_parent(other.m_parent)
			{
				other.m_parent = nullptr;
			}

			NJArray(SeNodeQJson* parent)
			: m_parent(parent)
			{
			}

			~NJArray()
			{
				if (m_parent)
				{
					m_parent->in_serialize(m_value);
				}
			}

			SeNodeQJson add_element()
			{
				m_value.append(QJsonValue());
				return SeNodeQJson(m_value[m_value.size() - 1]);
			}

		};
		typedef NJArray ArrApt;

		NJArray set_array()
		{
			return NJArray(this);
		}

		class NJObject
		{
		private:
			SeNodeQJson* m_parent;
			QJsonObject m_value;

		public:
		    NJObject(NJObject&& other) noexcept
				: m_value(std::move(other.m_value))
				, m_parent(other.m_parent)
			{
				other.m_parent = nullptr;
			}

			NJObject(SeNodeQJson* parent)
			: m_parent(parent)
			, m_value(m_parent->m_node.toObject())
			{
			}

			~NJObject()
			{
				if (m_parent)
				{
					m_parent->in_serialize(m_value);
				}
			}

			NJObject& operator = (NJObject&& other) noexcept
			{
				if (this != &other)
				{
					if (m_parent)
					{
						m_parent->in_serialize(m_value);
					}
					m_value = std::move(other.m_value);
					m_parent = other.m_parent;
					other.m_parent = nullptr;
				}
				return *this;
			}

			SeNodeQJson add_member(const char* strName)
			{
				return SeNodeQJson(m_value[strName]);
			}

		};

		typedef NJObject ObjApt;
		ObjApt set_object()
		{
			return ObjApt(this);
		}

	public:

		SeNodeQJson(const QJsonValueRef& node)
			: m_node(node)
		{
		}

	};

	class DeNodeQJson
	{
	protected:
		QJsonValueRef m_node;

	public:
		template <class T>
		void in_serialize(T& tValue) const
		{
			tValue = m_node.toVariant().value<T>();
		}

		template<class S, class T>
		void in_serialize(BaseTypeApt<S, T>& tValue) const
		{
			S strValue;
			in_serialize(strValue);
			tValue = strValue;
		}

		void in_serialize(char*& tValue) const
		{
			QByteArray baBuffer = m_node.toString().toUtf8();

			auto nSize = baBuffer.size() + 1;
			tValue = new char[nSize];
			std::copy(baBuffer.data(), baBuffer.data() + nSize, tValue);
		}

		template <typename T>
		void in_serialize(StrApt<T>& tValue) const
		{
			QByteArray strValue = m_node.toString().toUtf8();
			tValue.Set(strValue.data(), strValue.size());
		}

		void in_serialize(char *tValue, size_t nSize) const
		{
			QByteArray baBuffer = m_node.toString().toUtf8();
			if (nSize > baBuffer.size() + 1)
			{
				nSize = baBuffer.size() + 1;
			}

			std::copy(baBuffer.data(), baBuffer.data() + nSize, tValue);
		}

		class DeNodeArrIter
		{
		public:
			QJsonArray m_node;
			int m_idx;

			DeNodeArrIter(const QJsonArray& node)
				: m_node(node)
				, m_idx(0) {}

		public:
			DeNodeQJson operator *()
			{
				return DeNodeQJson(m_node[m_idx]);
			}

			operator bool() const
			{
				return m_idx != m_node.size();
			}

			DeNodeArrIter& operator ++ ()
			{
				++m_idx;
				return *this;
			}

			size_t array_size() const
			{
				return m_node.size();
			}
		};

		typedef DeNodeArrIter ArrIter;

		ArrIter sub_elements() const
		{
			return ArrIter(m_node.toArray());
		}

		class DeNodeObjIter
		{
			QJsonObject m_node;
			QJsonObject::iterator m_citCur;
			QJsonObject::iterator m_citEnd;
			
		public:
			DeNodeObjIter(const QJsonObject& node)
				: m_node(node)
				, m_citCur(m_node.begin()) 
				, m_citEnd(m_node.end())
			{}

			DeNodeQJson find_member(const char* strName) 
			{
				return DeNodeQJson(m_node[strName]);
			}

			DeNodeQJson operator *() 
			{
				return DeNodeQJson(*m_citCur);
			}

			template <typename T>
			T key() const
			{
				T t = m_citCur.key().toUtf8().data();
				return t;
			}

			operator bool() const
			{
				return m_citCur != m_citEnd;
			}

			DeNodeObjIter& operator ++ ()
			{
				++m_citCur;
				return *this;
			}
		};

		typedef DeNodeObjIter ObjIter;

		ObjIter sub_members() const
		{
			return ObjIter(m_node.toObject());
		}

		operator bool() const
		{
			return !m_node.isNull();
		}

	public:
		DeNodeQJson(const QJsonValueRef& node)
			: m_node(node) {}

	};

	class DeDocQJson
	{
		QJsonDocument m_doc;
	public:
		DeDocQJson()
		{ }

		bool Parse(QByteArray& strJson)
		{
			QJsonParseError err;
			m_doc = QJsonDocument::fromJson(strJson, &err);
			return err.error == QJsonParseError::NoError;
		}

		bool Load(const QString& strFilePath)
		{
			bool bRet = false;
			QFile f(strFilePath);
			if (f.open(QFile::ReadOnly))
			{
				QByteArray strJson = f.readAll();
				bRet = Parse(strJson);
			}

			return bRet;
		}

		template <class T>
		void operator >> (T &tValue)
		{
			QJsonArray array;
			if(m_doc.isObject())
			{
				array.append(m_doc.object());
			}
			else if(m_doc.isArray())
			{
				array.append(m_doc.array());
			}

			DeNodeQJson deNode(QJsonValueRef(&array, 0));
			Serializer<T>::deserialize(deNode, tValue);
		}
	};

	class SeDocQJson
	{
		QJsonDocument m_doc;
	public:
		SeDocQJson()
		{ }

		QByteArray Stringify(unsigned int indent = 0)
		{
			return m_doc.toJson(indent ? QJsonDocument::Indented : QJsonDocument::Compact);
		}

		bool Save(const QString& strFilePath, unsigned int indent = 0)
		{
			bool bRet = false;

			QFile f(strFilePath);
			if (f.open(QFile::WriteOnly))
			{
				QByteArray strJson = Stringify(indent);
				bRet = f.write(strJson) == strJson.size();
			}

			return bRet;
		}

		template <class T>
		void operator<<(const T &tValue)
		{
			QJsonArray array;
			array.append(QJsonValue());
			QJsonValueRef node = QJsonValueRef(&array, 0);

			SeNodeQJson seNode(node);

			Serializer<T>::serialize(seNode, tValue);

			if(node.isObject())
			{
				m_doc.setObject(node.toObject());
			}
			else if(node.isArray())
			{
				m_doc.setArray(node.toArray());
			}
		}
	};
}

#endif // QJSONNODE_H__
