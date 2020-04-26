#ifndef MSNODEAPT_STL_H__
#define MSNODEAPT_STL_H__

#include <vector>


namespace MSRPC
{
	template <typename IntType>
	class IPApt
	{
	public:
		IPApt(IntType& en)
			: m_en(en)
		{

		}

		operator char* () const
		{
			std::ostringstream ss;

			ss << ((m_en >> 24) & 0xFF) << '.' << ((m_en >> 16) & 0xFF) << '.' 
				<< ((m_en >> 8) & 0xFF) << '.' << ((m_en >> 0) & 0xFF);

			m_str = ss.str();

			return (char*)m_str.c_str();
		}

		void operator = (const char* szText)
		{
			std::istringstream iss(szText);

			uint16_t uS1 = 0, uS2 = 0, uS3 = 0, uS4 = 0;
			char p;

			iss >> uS1 >> p >> uS2 >> p >> uS3 >> p >> uS4;

			m_en = uS1;
			m_en <<= 8;
			m_en |= uS2;
			m_en <<= 8;
			m_en |= uS3;
			m_en <<= 8;
			m_en |= uS4;
		}

	private:
		IntType& m_en;
		mutable std::string m_str;
	};

	template <typename IntType>
	class StrApt<IPApt<IntType> >
	{
	public:
		IPApt<IntType>& m_data;

	public:
		StrApt(IPApt<IntType>& data) : m_data(data) {}
		StrApt(const IPApt<IntType>& data)
			: m_data(const_cast<IPApt<IntType>&>(data)) {}

		char* Get() const
		{
			return m_data;
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_data = tValue;
		}
	};


	template<class NODE, class T>
	class ISerialize<NODE, IPApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const IPApt<T>& tValue)
		{
			vNewNode.in_serialize(StrApt<IPApt<T> >(tValue));
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, IPApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, IPApt<T>& tValue)
		{
			vNewNode.in_serialize(StrApt<IPApt<T> >(tValue));
		}
	};


}

#endif // MSNODEAPT_STL_H__