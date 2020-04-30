#ifndef MSNODEAPT_STL_H__
#define MSNODEAPT_STL_H__

#include <sstream>

namespace MSRPC
{
	template <typename IntType>
	class IPApt;

	template <typename IntType>
	class StrApt<IPApt<IntType> >
	{
	public:
		StrApt(IntType& data) : m_data(data) {}

		char* Get() const
		{
			std::ostringstream ss;

			ss << ((m_data >> 0) & 0xFF) << '.' << ((m_data >> 8) & 0xFF) << '.' 
				<< ((m_data >> 16) & 0xFF) << '.' << ((m_data >> 24) & 0xFF);

			m_strTmp = ss.str();

			return (char*)m_strTmp.c_str();;
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_strTmp.assign(tValue, sSize);
			std::istringstream iss(m_strTmp);

			uint16_t uS1 = 0, uS2 = 0, uS3 = 0, uS4 = 0;
			char p;

			iss >> uS4 >> p >> uS3 >> p >> uS2 >> p >> uS1;

			m_data = uS1;
			m_data <<= 8;
			m_data |= uS2;
			m_data <<= 8;
			m_data |= uS3;
			m_data <<= 8;
			m_data |= uS4;
		}

	private:
		IntType& m_data;
		mutable std::string m_strTmp;
	};

}

#endif // MSNODEAPT_STL_H__