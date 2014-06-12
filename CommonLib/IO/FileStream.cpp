#include <stdafx.h>
#include "FileStream.h"


namespace Dream
{
	FileStream::FileStream()
	{

	}

	FileStream::~FileStream()
	{
		SAFE_DELETE_ARRAY(m_raw);
	}

	bool FileStream::Open()
	{
		return true;
	}

	void FileStream::Close()
	{
	}

	int FileStream::Read(void* buf, const int bytes)
	{
		return 0;
	}

	int FileStream::Write(const void* buf, const int bytes)
	{
		return 0;
	}

	void FileStream::Flush()
	{

	}

	void FileStream::Seek(int offset, SeekOrigin orig)
	{

	}

	int FileStream::Tell() const
	{
		return 0;
	}

	bool FileStream::Eof() const
	{
		return true;
	}

}