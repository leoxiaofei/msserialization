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

	template <class NODE, typename T, typename Enable = void>
	class ISerialize
	{
	public:
		static void serialize(NODE& vNewNode, const T& tValue)
		{
			vNewNode.set_object();
			IArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, const_cast<T&>(tValue));
		}
	};

	template <class NODE, class T>
	class ISerialize<NODE, T&>
	{
	public:
		static void serialize(NODE& vNewNode, const T& tValue)
		{
			ISerialize<NODE, T>::serialize(vNewNode, tValue);
		}
	};


	template<class NODE, typename T, int N>
	class ISerialize<NODE, T[N]>
	{
	public:
		static void serialize(NODE& vNewNode, const T(&tValue)[N])
		{
			vNewNode.set_array();

			for (int ix = 0; ix != N; ++ix)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, typename T>
	class ISerialize<NODE, T*>
	{
	public:
		static void serialize(NODE& vNewNode, const T* tValue)
		{
			if (tValue)
			{
				ISerialize<NODE, T>::serialize(vNewNode, *tValue);
			}
		}
	};

	template<class NODE>
	class ISerialize<NODE, bool>
	{
	public:
		static void serialize(NODE& vNewNode, const bool& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, char>
	{
	public:
		static void serialize(NODE& vNewNode, const char& tValue)
		{
			vNewNode.in_serialize(static_cast<short>(tValue));
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned char>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned char& tValue)
		{
			vNewNode.in_serialize(static_cast<unsigned short>(tValue));
		}
	};

	template<class NODE>
	class ISerialize<NODE, short>
	{
	public:
		static void serialize(NODE& vNewNode, const short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned short>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, int>
	{
	public:
		static void serialize(NODE& vNewNode, const int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned int>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, long>
	{
	public:
		static void serialize(NODE& vNewNode, const long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned long>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, long long>
	{
	public:
		static void serialize(NODE& vNewNode, const long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned long long>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, double>
	{
	public:
		static void serialize(NODE& vNewNode, const double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, float>
	{
	public:
		static void serialize(NODE& vNewNode, const float& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, char*>
	{
	public:
		static void serialize(NODE& vNewNode, char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}

	};

	template<class NODE>
	class ISerialize<NODE, const char*>
	{
	public:
		static void serialize(NODE& vNewNode, const char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE, int N>
	class ISerialize<NODE, char[N]>
	{
	public:
		static void serialize(NODE& vNewNode, const char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, StrApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const StrApt<T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE, class S, class T>
	class ISerialize<NODE, BaseTypeApt<S, T> >
	{
	public:
		static void serialize(NODE& vNewNode, const BaseTypeApt<S, T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

#if ANY_CPP11_OR_GREATER
	template<class NODE, class ENUM>
	class ISerialize<NODE, ENUM, typename std::enable_if<std::is_enum<ENUM>::value>::type>
	{
	public:
		static void serialize(NODE& vNewNode, const ENUM& tValue)
		{
			using UnderlyingType = typename std::underlying_type<ENUM>::type;
			ISerialize<NODE, UnderlyingType>::serialize(vNewNode, static_cast<UnderlyingType>(tValue));
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
			ISerialize<NODE, T>::serialize(m_vCurNode, tValue);
			return *this;
		}

		template <class T>
		IArchiveHelper& io(const char* strName, const T& tValue)
		{
			NODE vNewNode = m_vCurNode.new_node();
			ISerialize<NODE, T>::serialize(vNewNode, tValue);
			m_vCurNode.add_member(strName, vNewNode);

			return *this;
		}

	};

	template <class NODE>
	class OArchiveHelper;

	template<class NODE, typename T, typename Enable = void>
	class OSerialize
	{
	public:
		static void serialize(const NODE& vNewNode, T& tValue)
		{
			OArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, tValue);
		}
	};

	template<class NODE, typename T, int N>
	class OSerialize<NODE, T[N]>
	{
	public:
		static void serialize(const NODE& vNewNode, T(&tValue)[N])
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (int ix = 0; ix != N && itor; ++itor, ++ix)
			{
				OSerialize<NODE, T>::serialize(*itor, tValue[ix]);
			}
		}

	};

	template<class NODE, typename T>
	class OSerialize<NODE, T*>
	{
	public:
		static void serialize(const NODE& vNewNode, T*& tValue)
		{
			tValue = new T;
			OSerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, T&>
	{
	public:
		static void serialize(const NODE& vNewNode, T& tValue)
		{
			OSerialize<NODE, T>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, bool>
	{
	public:
		static void serialize(const NODE& vNewNode, bool& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, char>
	{
	public:
		static void serialize(const NODE& vNewNode, char& tValue)
		{
			int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<char>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned char>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned char& tValue)
		{
			unsigned int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<unsigned char>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, short>
	{
	public:
		static void serialize(const NODE& vNewNode, short& tValue)
		{
			int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<short>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned short>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned short& tValue)
		{
			unsigned int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<unsigned short>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, int>
	{
	public:
		static void serialize(const NODE& vNewNode, int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned int>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, long>
	{
	public:
		static void serialize(const NODE& vNewNode, long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned long>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, long long>
	{
	public:
		static void serialize(const NODE& vNewNode, long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned long long>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, double>
	{
	public:
		static void serialize(const NODE& vNewNode, double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, float>
	{
	public:
		static void serialize(const NODE& vNewNode, float& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, char*>
	{
	public:
		static void serialize(const NODE& vNewNode, const char*& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		static void serialize(const NODE& vNewNode, char*& tValue)
		{
			vNewNode.in_serialize(const_cast<const char*&>(tValue));
		}

	};

	template<class NODE, int N>
	class OSerialize<NODE, char[N]>
	{
	public:
		static void serialize(const NODE& vNewNode, char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue, N);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, StrApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, StrApt<T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE, class S, class T>
	class OSerialize<NODE, BaseTypeApt<S, T> >
	{
	public:
		static void serialize(const NODE& vNewNode, BaseTypeApt<S, T>&& tValue)
		{
			vNewNode.in_serialize((BaseTypeApt<S, T>&)tValue);
		}
	};

#if ANY_CPP11_OR_GREATER
	template<class NODE, class ENUM>
	class OSerialize<NODE, ENUM, typename std::enable_if<std::is_enum<ENUM>::value>::type>
	{
	public:
		static void serialize(NODE& vNewNode, ENUM& tValue)
		{
			using UnderlyingType = typename std::underlying_type<ENUM>::type;
			OSerialize<NODE, UnderlyingType>::serialize(vNewNode, *reinterpret_cast<UnderlyingType*>(&tValue));
		}
	};
#endif

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
			OSerialize<NODE, T>::serialize(m_vCurNode, tValue);
			return *this;
		}

		template <class T>
		OArchiveHelper& io(const char* strName, const T& tValue)
		{
			if (NODE vNewNode = m_vCurNode.sub_member(strName))
			{
				OSerialize<NODE, T>::serialize(vNewNode, const_cast<T&>(tValue));
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
	template <class NODE> \
	class ISerialize<NODE, EX> \
	{ \
	public: \
		static void serialize(NODE& vNewNode, EX const& tValue) \
		{ \
			vNewNode.set_object(); \
			IArchiveHelper<NODE> oh(vNewNode); \
			ex_serialize(oh, const_cast<EX&>(tValue)); \
		} \
	}; \
	template<class NODE> \
	class OSerialize<NODE, EX> \
	{ \
	public: \
		static void serialize(const NODE& vNewNode, EX& tValue) \
		{ \
			OArchiveHelper<NODE> oh(vNewNode); \
			ex_serialize(oh, tValue); \
		} \
	}; \
	}
}

#endif // MSARCHIVE_H__
