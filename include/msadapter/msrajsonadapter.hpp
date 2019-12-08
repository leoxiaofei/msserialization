#ifndef __MSJSONADAPTER_H__
#define __MSJSONADAPTER_H__


namespace MSRPC
{
	template<class T>
	class TBufferAdapter
	{
	public:
		TBufferAdapter(T& ba) 
			: baData(ba) {}

		typedef char Ch;

		void Put(char c) { baData.push_back(c); }
		void Flush() { }

		void Clear() { baData.clear(); }
		void ShrinkToFit() {
			// Push and pop a null terminator. This is safe.
			baData.shrink_to_fit();
		}

		char* Push(int count) {
			int nCurSize = baData.size();
			baData.resize(nCurSize + count);
			return baData.data() + nCurSize;
		}
		void Pop(int count) { baData.resize(baData.size() - count); }

		const char* GetString() {
			// Push and pop a null terminator. This is safe.
			baData.push_back('\0');
			baData.resize(baData.size() - 1);

			return baData.data();
		}

		int GetSize() const { return baData.size(); }
		
	private:
		T& baData;
	};
}


#endif // __MSJSONADAPTER_H__
