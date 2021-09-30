#include <string.h>
#include <stdio.h>
#include <afx.h>

/*
MSH|^~\&|Mindray|Gateway|||||ORU^R01|2|P|2.3.1| <CR>
PID|||M1015_00010||John^||20091112|M|||^^^^||| <CR>
PV1||I|^^ICU&Bed5&3232241659&0&0|||||||||||||||A||||||||||||||||||||||||||20091201111211 <CR>
OBR||||Mindray Monitor|||20091203121631| <CR>
OBX||NM|52^Height||169.0||||||F <CR>
OBX||NM|51^Weight||59.0||||||F <CR>
OBX||CE|2302^BloodType||1^A||||||F <CR>
OBX||CE|2303^PACE_Switch||0^Off||||||F <CR>
OBX||NM|101^HR|2101|60||||||F <CR>
OBX||NM|151^RR|2102|20||||||F <CR>
OBX||NM|200^T1|2104|37.00||||||F <CR>
*/
int SplitString(char* source, CString split, CStringArray& strArray)
{
	strArray.RemoveAll();
	//const char * split = "\r\n";
	char* p;
	p = strtok(source, split);
	while (p != NULL) {
		CString str(p);
		strArray.Add(str);
		p = strtok(NULL, split);
	}
	return strArray.GetCount();
}

//splilit by "|"
int ExtString(CString str, CString split, CStringArray& strArray)
{
	CString strTemp;
	int nCount = 0;
	strArray.RemoveAll();
	while (str.Find(_T(split)) != -1)
		//detect|
	{
		strTemp = str.Left(str.Find(_T(split)));
		//extract string
		str = str.Right(str.GetLength() - str.Find(_T(split)) - 1);
		//rest string
		strArray.Add(strTemp);
		//extract string to string array
		nCount++;
	}
	//last string
	if (str.GetLength() > 0)
	{
		strArray.Add(str);//extract string to string array
		nCount++;
	}
	return nCount;
}

CString GetMessageByHead(CString strFilePath, CString strHead, int num)
{
	CFile file;
	char* pBuf;
	CString strResult = "";
	CStringArray strSegMentArrary;
	CStringArray strDataFieldArrary;

	if (file.Open(strFilePath, CFile::modeRead, NULL))
	{
		int iLen = file.GetLength();
		pBuf = new char[iLen + 1];
		file.Read(pBuf, iLen);
		pBuf[iLen] = 0;
		file.Close();
	}
	else
	{
		//Log::WriteLog("fall open the fail£¡");
		return "";
	}

	if (SplitString(pBuf, "\r\n", strSegMentArrary) > 0)
	{
		if (strHead != "")
		{
			for (int i = 0; i < strSegMentArrary.GetCount(); i++)
			{
				CString strTemp = strSegMentArrary.GetAt(i);
				if (strTemp.Find(strHead) != -1)
				{
					ExtString(strTemp, "|", strDataFieldArrary);
					strResult = strDataFieldArrary.GetAt(num);
				}
			}
		}
	}

	delete[] pBuf;
	return  strResult;
}

