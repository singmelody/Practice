#pragma once
#include <string>
#include <EngineConfig.h>

namespace Dream
{
	class IStream
	{
	public:
		enum ACCESS
		{
			eReadAccess = 0x01,
			eWriteAccess = 0x02,
		};

		enum SeekOrigin
		{
			eBegin = 0,
			eCurrent,
			eEnd,
		};

		IStream() : m_size(0), m_position(0), m_uri(""), m_raw(NULL){}
		virtual ~IStream(){ SAFE_DELETE_ARRAY(m_raw); }

		virtual bool Open() = 0;
		virtual void Close() = 0;

		virtual const char* GetURI() const { return m_uri.c_str(); }
		virtual void SetURI(const char* uri){ m_uri = uri; }

		virtual int  Read(void* buf, const int bytes) = 0;
		virtual int  Write(const void* buf, const int bytes) = 0;
		virtual void Flush() = 0;

		virtual void SetSize(size_t s) { m_size = s; }
		virtual size_t GetSize() const { return m_size; }

		virtual void SetRaw(unsigned char* raw) { m_raw = raw; }
		virtual unsigned char* GetRaw() { return m_raw;}

		virtual void Seek(int offset, SeekOrigin orig) = 0;
		virtual int  Tell() const = 0; 
		virtual bool Eof() const = 0;  

	protected:
		int m_size;
		int m_position;
		std::string m_uri;

		unsigned char* m_raw;
	};
}