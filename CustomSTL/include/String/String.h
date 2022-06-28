#pragma once

#if _HAS_CXX20
#define CONSTEXPR20 constexpr
#define AllocStr(VAR, SIZE) new char[SIZE]{}
#define DeallocStr(VAR) delete[] VAR; VAR = nullptr
#else
#define CONSTEXPR20 inline
#define AllocStr(VAR, SIZE) static_cast<char*>(_malloca(SIZE)); memset(VAR, 0, SIZE)
#define DeallocStr(VAR) _freea(VAR)
#endif

#include <iostream>
#include <cstring>

/*
*
*	CONSTRUCTORS
*
*/
namespace CTL
{
	constexpr size_t length(const char* str)
	{
		return *str ? 1 + length(str + 1) : 0;
	}

	class String
	{
		friend std::ostream& operator<<(std::ostream& stream, const String& data);

	public:
		CONSTEXPR20 String() : m_Length(0), m_Size(sizeof(char) * 15)
		{
			m_Buffer = AllocStr(m_Buffer, m_Size);
			if (!m_Buffer)
				throw std::bad_alloc();
		}

		CONSTEXPR20 String(const String& other) : m_Length(other.m_Length), m_Size(other.m_Size)
		{
			m_Buffer = AllocStr(m_Buffer, m_Size);
			if (m_Buffer)
				strcpy_s(m_Buffer, m_Size, other.m_Buffer);
			else
				throw std::bad_alloc();
		}


		/*
		*
		* EXPLICIT CONSTRUCTORS
		*
		*/

		CONSTEXPR20 String(size_t requiredLength) : m_Length(0), m_Size(sizeof(char)* (requiredLength + 1))
		{
			m_Buffer = AllocStr(m_Buffer, m_Size);
			if (!m_Buffer)
				throw std::bad_alloc();
		}

		CONSTEXPR20 String(const char* string)
		{
			m_Length = length(string);
			m_Size = (m_Length + 1) * sizeof(char);
			m_Buffer = AllocStr(m_Buffer, m_Size);

			if (m_Buffer)
				strcpy_s(m_Buffer, m_Size, string);
			else
				throw std::bad_alloc();
		}

		CONSTEXPR20 String(const std::string& string) : m_Length(string.size()), m_Size(m_Length + 1)
		{
			m_Buffer = AllocStr(m_Buffer, m_Size);
			if (m_Buffer)
				strcpy_s(m_Buffer, m_Size, string.data());
			else
				throw std::bad_alloc();
		}

		/*
		*
		* .reserve method
		*
		*/
		CONSTEXPR20 bool reserve(const size_t size)
		{
			if (size < m_Size)
				return false;

			if (m_Length == 0)
			{
				DeallocStr(m_Buffer);
				m_Buffer = AllocStr(m_Buffer, size);

				if (m_Buffer)
					m_Size = size;
				else
					throw std::bad_alloc();
			}
			else
			{
				char* oldStr{ m_Buffer };
				m_Buffer = AllocStr(m_Buffer, size);

				if (m_Buffer)
				{
					strcpy_s(m_Buffer, size, oldStr);
					m_Size = size;

					DeallocStr(oldStr);
				}
				else
				{
					DeallocStr(oldStr);
					throw std::bad_alloc();
				}
			}

			return true;
		}

		/*
		*
		* .append method
		*
		*/
		CONSTEXPR20 void append(const char* string, const size_t requiredSize = 0)
		{
			size_t strLength{ length(string) };
			size_t strSize{ (m_Length + strLength + 1) * sizeof(char) };

			if (m_Size <= strSize)
			{
				char* oldStr = m_Buffer;
				size_t newStrSize{ (requiredSize != 0) ? requiredSize : strSize * 2 - 1 };

				std::cout << "New strSize: " << newStrSize << '\n';

				m_Buffer = AllocStr(m_Buffer, newStrSize);
				if (m_Buffer)
				{
					strcpy_s(m_Buffer, newStrSize, oldStr);
					strcat_s(m_Buffer, newStrSize, string);

					m_Length += strLength;
					m_Size = newStrSize;

					DeallocStr(oldStr);
				}
				else
				{
					DeallocStr(oldStr);
					throw std::bad_alloc();
				}
			}
			else
			{
				strcat_s(m_Buffer, m_Size, string);
				m_Length += strLength;
			}
		}

		CONSTEXPR20 void append(const std::string& string, const size_t requiredSize = 0)
		{
			append(string.data(), requiredSize);
		}

		CONSTEXPR20 void append(const String& string, const size_t requiredSize = 0)
		{
			append(string.m_Buffer, requiredSize);
		}

		CONSTEXPR20 bool Has(const char character) const
		{
			for (size_t i{}; i < m_Length; ++i)
				if (m_Buffer[i] == character)
					return true;

			return false;
		}

		CONSTEXPR20 bool Has(const char* string) const
		{
			size_t strCount{ length(string) };

			for (size_t i{}; i < m_Length; ++i)
			{
				size_t j{};
				while (j < strCount && m_Buffer[i] == string[j]) ++i, ++j;

				if (j == strCount)
					return true;
			}
			return false;
		}

		CONSTEXPR20 bool Has(const std::string& string) const
		{
			return Has(string.data());
		}

		CONSTEXPR20 bool Has(const String& string) const
		{
			return Has(string.m_Buffer);
		}

		CONSTEXPR20 char* begin()
		{
			return m_Buffer;
		}

		CONSTEXPR20 char* end()
		{
			return m_Buffer + m_Length;
		}

		CONSTEXPR20 char* Data()
		{
			return m_Buffer;
		}

		CONSTEXPR20 const char* begin() const
		{
			return m_Buffer;
		}

		CONSTEXPR20 const char* end() const
		{
			return m_Buffer + m_Length;
		}

		CONSTEXPR20 const char* Data() const
		{
			return m_Buffer;
		}

		CONSTEXPR20 const size_t Length() const
		{
			return m_Length;
		}

		CONSTEXPR20 const size_t Size() const
		{
			return m_Size;
		}

		/*
		*
		* operator[](index)
		*
		*/
		CONSTEXPR20 char& operator[](size_t index)
		{
			if (m_Length > index)
				return m_Buffer[index];

			throw std::out_of_range("Index out of bounds!");
		}

		/*
		*
		* operator[](index)
		*
		*/
		CONSTEXPR20 const char& operator[](size_t index) const
		{
			if (m_Length > index)
				return m_Buffer[index];

			throw std::out_of_range("Index out of bounds!");
		}

		/*
		*
		*	operator+(string)
		*
		*/
		CONSTEXPR20 String operator+(const char* string) const
		{
			String newStr{ m_Length + length(string) };

			newStr.append(m_Buffer);
			newStr.append(string);

			return newStr;
		}

		CONSTEXPR20 String operator+(const std::string& string) const
		{
			return (*this + string.data());
		}

		CONSTEXPR20 String operator+(const String& other) const
		{
			return (*this + other.m_Buffer);
		}

		/*
		*
		*	operator*(string)
		*
		*/

		CONSTEXPR20 String operator*(size_t count) const
		{
			if (m_Length == 0)
				return String{ 1 };

			String newStr{ count * m_Length };
			char* thisStr{ this->m_Buffer };

			while (count-- > 0)
				newStr.append(thisStr);

			return newStr;
		}

		/*
		*
		*	operator==()
		*
		*/

		CONSTEXPR20 bool operator==(const char* string) const
		{
			if (m_Length != length(string))
				return false;

			for (size_t i{}; i < m_Length; ++i)
				if (m_Buffer[i] != string[i])
					return false;

			return true;
		}

		CONSTEXPR20 bool operator==(const std::string& string) const
		{
			return (*this == string.data());
		}

		CONSTEXPR20 bool operator==(const String& other) const
		{
			return (*this == other.m_Buffer);
		}

		/*
		*
		*	operator!=()
		*
		*/

		CONSTEXPR20 bool operator!=(const char* string) const
		{
			return !(*this == string);
		}

		CONSTEXPR20 bool operator!=(const std::string& string) const
		{
			return !(*this == string.data());
		}

		CONSTEXPR20 bool operator!=(const String& other) const
		{
			return !(*this == other.m_Buffer);
		}

		/*
		*
		*	operator>()
		*
		*/

		CONSTEXPR20 bool operator>(const char* string) const
		{
			for (size_t i{}; i < m_Length; ++i)
				if (m_Buffer[i] < string[i])
					return false;

			return true;
		}

		CONSTEXPR20 bool operator>(const std::string& string) const
		{
			return (*this > string.data());
		}

		CONSTEXPR20 bool operator>(const String& other) const
		{
			return (*this > other.m_Buffer);
		}

		/*
		*
		*	operator<()
		*
		*/

		CONSTEXPR20 bool operator<(const char* string) const
		{
			return !(*this >= string);
		}

		CONSTEXPR20 bool operator<(const std::string& string) const
		{
			return !(*this >= string);
		}

		CONSTEXPR20 bool operator<(const String& other) const
		{
			return !(*this >= other);
		}

		/*
		*
		*	operator>=()
		*
		*/

		CONSTEXPR20 bool operator>=(const char* string) const
		{
			const String& self{ *this };

			return (self > string || self == string);
		}

		CONSTEXPR20 bool operator>=(const std::string& string) const
		{
			return (*this >= string.data());
		}

		CONSTEXPR20 bool operator>=(const String& other) const
		{
			return (*this >= other.m_Buffer);
		}

		/*
		*
		*	operator<=()
		*
		*/

		CONSTEXPR20 bool operator<=(const char* string) const
		{
			return !(*this > string);
		}

		CONSTEXPR20 bool operator<=(const std::string& string) const
		{
			return !(*this > string.data());
		}

		CONSTEXPR20 bool operator<=(const String& other) const
		{
			return !(*this > other.m_Buffer);
		}

		CONSTEXPR20 ~String()
		{
			DeallocStr(m_Buffer);
		}

		private:
			size_t m_Length;
			size_t m_Size;
			char* m_Buffer;
	};

	/*
	*
	*	operator<<(cout)
	*
	*/
	std::ostream& operator<<(std::ostream& stream, const String& data)
	{
		stream << data.m_Buffer;
		return stream;
	}
};