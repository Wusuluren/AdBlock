#pragma once
#include "afx.h"

class CAdBlockFile :
	public CFile
{
public:
	CAdBlockFile(void);
	~CAdBlockFile(void);
	bool WriteLine(TCHAR *line);
	bool ReadLine(TCHAR *line, int &size);
};
