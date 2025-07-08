#ifndef MSARCHIVE_H__
#define MSARCHIVE_H__

#include <cstddef>
#include "cplusplusmacros.h"
#include "msbasetypeapt.hpp"
#include "siexse.hpp"

#if ANY_CPP11_OR_GREATER
#include <type_traits>
#endif

namespace MSRPC
{
	template<typename T>
	class StrApt
	{
	public:
		/// Choose between two Get methods: If you can obtain a char string constant, 
		/// use the const version to avoid an extra copy under RapidJSON. 
		/// If you cannot obtain a constant string, use the non-const Get method.
		/*
		const char* Get() const
		{
			return 0;
		}

		char* Get() const
		{
			return 0;
		}

		void Set (const char* tValue, const size_t& sSize)
		{
		}
		*/

		StrApt(T& str)
			: m_str(str) {}

		StrApt(const T& str)
			: m_str(const_cast<T&>(str)) {}

		const char* Get() const
		{
			return m_str.c_str();
		}

		size_t Size() const
		{
			return m_str.size();
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_str.assign(tValue, sSize);
		}

	private:
		T& m_str;

	};

	template <class NODE>
	class IArchiveHelper;

	template <class NODE>
	class OArchiveHelper;

	template <typename T, typename IsEnum = void>
	class Serializer 
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const T& tValue)
		{
			vNewNode.set_object();
			IArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, const_cast<T&>(tValue));
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, T& tValue)
		{
			OArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, tValue);
		}
	};

	template<typename T>
	class Serializer<T&>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const T& tValue)
		{
			Serializer<T>::serialize(vNewNode, tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, T& tValue)
		{
			Serializer<T>::deserialize(vNewNode, tValue);
		}
	};

	template<typename T>
	class Serializer<T*>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const T* tValue)
		{
			if (tValue)
			{
				Serializer<T>::serialize(vNewNode, *tValue);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, T*& tValue)
		{
			tValue = new T;
			Serializer<T>::deserialize(vNewNode, *tValue);
		}
	};

	template<typename T, int N>
	class Serializer<T[N]>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const T(&tValue)[N])
		{
			vNewNode.set_array();
			for (int ix = 0; ix != N; ++ix)
			{
				NODE vNode = vNewNode.new_node();
				Serializer<T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, T(&tValue)[N])
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (int ix = 0; ix != N && itor; ++itor, ++ix)
			{
				Serializer<T>::deserialize(*itor, tValue[ix]);
			}
		}
	};

#define IN_SERIALIZER(T) \
	template<> \
	class Serializer<T> \
	{ \
	public: \
		template<class NODE>
		static void serialize(NODE& vNewNode, const T& tValue) \
		{ \
			vNewNode.in_serialize(tValue); \
		} \
		template<class NODE>
		static void deserialize(const NODE& vNewNode, T& tValue) \
		{ \
			vNewNode.in_serialize(tValue); \
		} \
	};

IN_SERIALIZER(bool);
IN_SERIALIZER(int);
IN_SERIALIZER(unsigned int);
IN_SERIALIZER(long);
IN_SERIALIZER(unsigned long);
IN_SERIALIZER(long long);
IN_SERIALIZER(unsigned long long);
IN_SERIALIZER(float);
IN_SERIALIZER(double);

	template<>
	class Serializer<char>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const char& tValue)
		{
			vNewNode.in_serialize(static_cast<short>(tValue));
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, char& tValue)
		{
			int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<char>(uValue);
		}
	};

	template<>
	class Serializer<unsigned char>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const unsigned char& tValue)
		{
			vNewNode.in_serialize(static_cast<unsigned short>(tValue));
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, unsigned char& tValue)
		{
			unsigned int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<unsigned char>(uValue);
		}
	};

	template<>
	class Serializer<short>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, short& tValue)
		{
			int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<short>(uValue);
		}
	};

	template<>
	class Serializer<unsigned short>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const unsigned short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, unsigned short& tValue)
		{
			unsigned int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<unsigned short>(uValue);
		}
	};

	template<>
	class Serializer<char*>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, char*& tValue)
		{
			vNewNode.in_serialize(const_cast<const char*&>(tValue));
		}
	};

	template<>
	class Serializer<const char*>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, const char*& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<int N>
	class Serializer<char[N]>
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue, N);
		}
	};

	template<class T>
	class Serializer<StrApt<T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const StrApt<T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, StrApt<T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class S, class T>
	class Serializer<BaseTypeApt<S, T> >
	{
	public:
		template<class NODE>
		static void serialize(NODE& vNewNode, const BaseTypeApt<S, T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<class NODE>
		static void deserialize(const NODE& vNewNode, BaseTypeApt<S, T>&& tValue)
		{
			vNewNode.in_serialize((BaseTypeApt<S, T>&)tValue);
		}
	};

#if ANY_CPP11_OR_GREATER
	template<class ENUM>
	class Serializer<ENUM, typename std::enable_if<std::is_enum<ENUM>::value>::type>
	{
	public:
		using UnderlyingType = typename std::underlying_type<ENUM>::type;

		template<class NODE>
		static void serialize(NODE& vNewNode, const ENUM& tValue)
		{
			Serializer<UnderlyingType>::serialize(vNewNode, static_cast<UnderlyingType>(tValue));
		}

		static void deserialize(NODE& vNewNode, ENUM& tValue)
		{
			Serializer<UnderlyingType>::deserialize(vNewNode, *reinterpret_cast<UnderlyingType*>(&tValue));
		}
	};
#endif

	template <class NODE>
	class IArchiveHelper
	{
	private:
		NODE& m_vCurNode;

	public:
		typedef NODE Node;

	public:
		IArchiveHelper(NODE &vNode)
			: m_vCurNode(vNode)
		{
		}

		template <class T>
		IArchiveHelper &operator&(const T &tValue)
		{
			Serializer<T>::serialize(m_vCurNode, tValue);
			return *this;
		}

		template <class T>
		IArchiveHelper& io(const char* strName, const T& tValue)
		{
			NODE vNewNode = m_vCurNode.new_node();
			Serializer<T>::serialize(vNewNode, tValue);
			m_vCurNode.add_member(strName, vNewNode);

			return *this;
		}
	};

	template <class NODE>
	class OArchiveHelper
	{
	private:
		const NODE& m_vCurNode;

	public:
		typedef NODE Node;

	public:
		OArchiveHelper(const NODE& vNode)
			: m_vCurNode(vNode) {}

		template <class T>
		const OArchiveHelper& operator & (T& tValue) const
		{
			Serializer<T>::deserialize(m_vCurNode, tValue);
			return *this;
		}

		template <class T>
		OArchiveHelper& io(const char* strName, const T& tValue)
		{
			if (NODE vNewNode = m_vCurNode.sub_member(strName))
			{
				Serializer<T>::deserialize(vNewNode, const_cast<T&>(tValue));
			}

			return *this;
		}

		const NODE& node() const
		{
			return m_vCurNode;
		}

	};

#define DiExSe(EX) \
	namespace MSRPC { \
	template <> \
	class Serializer<EX> \
	{ \
	public: \
		template<class NODE> \
		static void serialize(NODE& vNewNode, EX const& tValue) \
		{ \
			vNewNode.set_object(); \
			IArchiveHelper<NODE> oh(vNewNode); \
			ex_serialize(oh, const_cast<EX&>(tValue)); \
		} \

		template<class NODE> \
		static void deserialize(const NODE& vNewNode, EX& tValue) \
		{ \
			OArchiveHelper<NODE> oh(vNewNode); \
			ex_serialize(oh, tValue); \
		} \
	}; \
	}
}

#endif // MSARCHIVE_H__
