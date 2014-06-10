#pragma once

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

		enum WHENCE
		{
			eBegin = 0,
			eCurrent,
			eEnd,
		};

		IStream(){}
		virtual ~IStream(){}

		virtual bool    IsValid() const = 0;
		virtual const char* GetURI() const { return _uri.c_str(); }
		virtual int GetSize() const { return _stream_size; }
		virtual AccessMode GetAccessMode() const { return _access_mode; }

		virtual bool CanRead() const { return (_access_mode & kReadAccess) != 0; }
		virtual bool CanWrite() const { return (_access_mode & kWriteAccess) != 0; }
		virtual bool CanSeek() const = 0;

		virtual int  Read(void* buf, const int bytes) = 0;
		virtual int  Write(const void* buf, const int bytes) = 0;
		virtual void Flush() = 0;

		virtual void Seek(int offset, SeekOrigin orig) = 0;
		virtual int  Tell() const = 0; 
		virtual bool Eof() const = 0;  
		virtual void Close() = 0;

	protected:

	};
}