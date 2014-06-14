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
		// check if end-of-stream is near
		size_t readBytes = bytes <= this->m_size - this->m_position ? bytes : this->m_size - this->m_position;
		if (bytes > 0)
		{
			memcpy( buf, m_raw + m_position, bytes);
			this->m_position += readBytes;
		}
		return readBytes;
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
		switch (orig)
		{
		case eBegin:
			m_position = offset;
			break;
		case eCurrent:
			m_position += offset;
			break;
		case eEnd:
			m_position = m_size + offset;
			break;
		}
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