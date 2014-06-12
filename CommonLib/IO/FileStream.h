#pragma once
#include "IStream.hpp"

namespace Dream
{
	class FileStream : public IStream
	{
	public:
		FileStream();
		virtual ~FileStream();

		virtual bool Open();
		virtual void Close();

		virtual int  Read(void* buf, const int bytes);
		virtual int  Write(const void* buf, const int bytes);
		virtual void Flush(); 

		virtual void Seek(int offset, SeekOrigin orig);
		virtual int  Tell() const; 
		virtual bool Eof() const;  

	protected:
	};
}