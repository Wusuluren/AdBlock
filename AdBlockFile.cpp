#include "stdafx.h"
#include "AdBlockFile.h"

#define BUF_LINE_SIZE	1024

CAdBlockFile::CAdBlockFile(void)
{
}

CAdBlockFile::~CAdBlockFile(void)
{
}

bool CAdBlockFile::WriteLine(TCHAR *line)
{
	DWORD bytes, written;
	TCHAR buf[BUF_LINE_SIZE];
	_tcscpy_s(buf, line);
	strcat_s(buf, "\r\n");
	bytes = strlen(buf);
	if(!WriteFile(this->m_hFile, buf, bytes, &written, NULL))
	{
		CString tmp;
		tmp.Format("%d", ::GetLastError());
		AfxMessageBox(tmp);
		return false;
	}
	if(written < bytes)
		return false;
	return true;
}

bool CAdBlockFile::ReadLine(TCHAR *line, int &size)
{
	DWORD bytes, read, i;
	bytes = size;
	if(!ReadFile(this->m_hFile, line, bytes, &read, NULL))
		return false;
	for(i = 0; i < read; i++)
	{
		if(line[i] == '\n' || line[i] == '\r')
			break;
	}
	if(line[i+1] == '\n' || line[i+1] == '\r')
		size = i+1;
	else
		size = i;
	line[i] = '\0';
	if(i == 0)
		return false;
	this->Seek(i - read, CAdBlockFile::current);
	return true;
}
