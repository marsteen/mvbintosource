//***************************************************************************
//
//
// @PROJECT  :	Diercke Globus Online
// @VERSION  :	3.0
// @FILENAME :	CFileIO.cpp
// @DATE     :	18.8.2014
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifdef __WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <NStringTool.h>
#include <CFileIO.h>

using namespace std;





//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteFromFile
//
//
//---------------------------------------------------------------------------

int CFileIO::WriteFromFile(const char* Filename)
{
	int FileSize = 0;
	const char* FileData = (const char*) ReadFile(Filename, &FileSize);

	if (FileData != NULL)
	{
		WriteBytes(FileData, FileSize);
		delete[] FileData;
	}
	return FileSize;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   FileCopy
//
//
//---------------------------------------------------------------------------

bool CFileIO::FileCopy(const std::string FileSrc, const std::string FileDst)
{
	bool r;

	int FileSize;
	const char* FileData = (const char*) ReadFile(FileSrc.c_str(), &FileSize);
	if (FileData != NULL)
	{
		if (WriteFile(FileDst.c_str(), FileData, FileSize))
		{
			r = true;
		}
		delete[] FileData;
	}
	else
	{
		r = false;
	}
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetInt
//
//
//---------------------------------------------------------------------------

int CFileIO::GetInt(const char* str)
{
	if (str != NULL)
	{
		int x;
		sscanf(str, "%d", &x);
		return x;
	}
	return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetChar
//
// Parameter: char* Filename - Name der Datei
//
// Return:    int - Groesse der Datei in Bytes
//
//---------------------------------------------------------------------------

int CFileIO::GetChar()
{
  char ch;
  mFs->get(ch);
  return ch;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   IsEof
//
//
//---------------------------------------------------------------------------

bool CFileIO::IsEof()
{
	//if (mFs == NULL) return true;
  return mFs->eof();
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   Clear
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::Clear()
{
	mFs->clear();
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SkipBytes
//
// Parameter: int nBytes - Anzahl an Bytes ueberspringen (Eingabedatei)
//
//
//---------------------------------------------------------------------------

void CFileIO::SkipBytes(int nBytes)
{
	mFs->seekg(nBytes, ios_base::cur);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetFileSizestat
//
// Parameter: Filename - Name der Datei
// Return:    Groesse der Datei in Bytes oder
//            -1 = Datei nicht vorhanden
//
// http://linux.die.net/man/2/stat
//
//---------------------------------------------------------------------------

#if 0
int CFileIO::GetFileSize(const char* Filename)
{
	struct stat StatBuffer;
	int Filesize;

	if (stat(Filename, &StatBuffer) == 0)
	{
		Filesize = StatBuffer.st_size;
	}
	else
	{
		Filesize = -1;
	}
	return Filesize;
}
#endif

#if 1
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetFileSize
//
// Parameter: char* Filename - Name der Datei
//
// Return:    Groesse der Datei in Bytes
//
//---------------------------------------------------------------------------

int CFileIO::GetFileSize(const char* Filename)
{
  streampos FileSize = 0;
  ifstream fin(Filename, ios::in | ios::binary);
  if (fin.good())
  {
  	fin.seekg(0, ios::end);
  	FileSize = fin.tellg();
  	fin.close();
	}
	else
	{
		FileSize = -1;
	}

  return FileSize;
}
#endif

#if 0
//--------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   FileExists
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Ueberpruefen, ob Datei existiert
//
//---------------------------------------------------------------------------

bool CFileIO::FileExists(const char* Filename)
{
  ifstream fin(Filename, ios::in | ios::binary);
  bool s = fin.good();

  if (s)
  {
    fin.close();
  }
  return s;
}
#endif

#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		FileAttribut
//
// return: 0 = Datei nicht gefunden
//         1 = "normale" Datei
//         2 = Ordner
//
//---------------------------------------------------------------------------

int CFileIO::FileAttribut(const char* Filename)
{
	int r = EFA_FILE;

#ifdef __WIN32__

	DWORD fa = GetFileAttributes(Filename);
	if (fa == 0xFFFFFFFF)
	{
		r = EFA_NOT_FOUND;
	}
	else
	if (fa & FILE_ATTRIBUTE_DIRECTORY)
	{
		r = EFA_FOLDER;
	}
#endif

	return r;

}
#endif


//--------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileRead
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Oeffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileRead(const char* Filename, OPENMODE OpenMode)
{
  if (mFs != NULL) delete mFs;
  mFs = new fstream(Filename, ios::in | OpenMode);

  mOpenRead = mFs->good();
  return mOpenRead;
}


bool CFileIO::OpenFileRead(const std::string Filename, OPENMODE OpenMode)
{
	return OpenFileRead(Filename.c_str(), OpenMode);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileWrite
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Oeffnen einer Datei zum Schreiben
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileWrite(const char* Filename, OPENMODE OpenMode)
{
  if (mFs != NULL) delete mFs;
  mFs = new fstream(Filename, ios::out | OpenMode);

  mOpenWrite = mFs->good();
  return mOpenWrite;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileWrite
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Oeffnen einer Datei zum Schreiben
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileWrite(const std::string Filename, OPENMODE OpenMode)
{
  return OpenFileWrite(Filename.c_str(), OpenMode);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileAppend
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Oeffnen einer Datei zum Anhaengen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileAppend(const char* Filename, OPENMODE OpenMode)
{
  if (mFs != NULL) delete mFs;
  mFs = new fstream(Filename, ios::app | ios::out| OpenMode);

  mOpenWrite = mFs->good();
  return mOpenWrite;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileChange
//
// Parameter: char* filename - Name der Datei
//            int pos        - Schreibposition
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Oeffnen einer Datei zum Aendern
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileChange(const char* Filename, int Position)
{
	bool suc = OpenFileWrite(Filename, ios::out | ios::binary | ios::in);

	if (suc)
	{
		SetReadPos(Position);
		SetWritePos(Position);
	}
	return suc;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO (static)
// Methode:   ReadFile
//
// Parameter: char* Filename - Name der Datei
//
//            int* nBytes    - Anzahl der Bytes die gelesen wurden (Groesse
//                             der Datei).
//
// Return:    void* - Zeiger auf Puffer, in den die Daten gelesen wurden,
//                    oder NULL (Fehlerfall)
//
//
//
// Lesen des Inhalts einer Datei in einen Puffer. Der Speicher fuer den
// Puffer wird reserviert. Zurueckgegeben werden ein Zeiger auf den
// Puffer sowie die Anzahl der eingelesenen Bytes
//
//---------------------------------------------------------------------------

void* CFileIO::ReadFile(const char* Filename, int* nBytes)
{
  char* buffer = nullptr;
  *nBytes = CFileIO::GetFileSize(Filename);
  if (nBytes != nullptr)
  {
		fstream fs(Filename, ios::in | ios::binary);

    if (fs.good())
    {
      buffer = new char[*nBytes+1];
      if (buffer != nullptr)
      {
				fs.read((char*) buffer, *nBytes);
				fs.close();
				buffer[*nBytes] = 0;
      }
    }
  }
  return buffer;
}


void* CFileIO::ReadFile(const string Filename, int* nBytes)
{
	return ReadFile(Filename.c_str(), nBytes);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO (static)
// Methode:   WriteFile
//
// Parameter: char* Filename - Name der Datei
//
//            int   nBytes    - Anzahl der Bytes die gelesen wurden (Groesse
//                             der Datei).
//
//            const void* - Zeiger auf Puffer, dessen Daten geschrieben
//                          werden
//
//
//---------------------------------------------------------------------------

bool CFileIO::WriteFile(const char* Filename, const void* buffer, int nBytes)
{
	bool r = false;

  if (nBytes > 0)
  {
		fstream fs(Filename, ios::out | ios::binary);

		if (fs.good())
		{
			fs.write((const char*) buffer, nBytes);
			fs.close();

			r = true;
    }
  }
  return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteBytes
//
//
//
// Einlesen einer Anzahl von Bytes aus einer geoeffneten Datei
//
//---------------------------------------------------------------------------

bool CFileIO::WriteBytes(const void* buffer, int nBytes)
{
  if (mOpenWrite)
  {
    mFs->write((const char*) buffer, nBytes);
  }
  return mOpenWrite;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteString
//
//
//---------------------------------------------------------------------------

bool CFileIO::WriteString(const char* str)
{
 	return WriteBytes(str, strlen(str));
}

#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReplaceAllStrings
//
//
//---------------------------------------------------------------------------

class CStrpos : public CListElem<CStrpos>
{
	public:

		char* mPos;
};

class CStrposList : public CList<CStrpos>
{
};


static void CopyStringBytes(char* dst, char* src, int num)
{
	int i;
	for (i = 0; (i < num) && (*src != 0); i++)
	{
		*(dst++) = *(src++);
	}
	*dst = 0;
}

char* CFileIO::ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count)
{
	CStrposList slist;
	int   cnt = 0;
	char* pos;
	char* StartPos  = SrcString;
	int   LenSearch = strlen(SearchString);
	int   LenSub    = strlen(SubString);
	int   LenNew;
	char* NewString = NULL;

	do
	{
		pos = strstr(StartPos, SearchString);
		if (pos != NULL)
		{
			CStrpos* se = slist.NewListElem();
			se->mPos = pos;
			StartPos = pos + LenSearch;
			cnt++;
		}
	}
	while (pos != NULL);

	*Count = cnt;

	if (cnt > 0)
	{
		LenNew = strlen(SrcString) + (cnt * (LenSub - LenSearch));

		NewString = new char[LenNew+1];
		char* NewStringPos = NewString;

		StartPos = SrcString;
		for (CStrpos* se = slist.GetFirst();
									se != NULL;
									se = slist.GetNext())
		{

			int len = se->mPos - StartPos;
			CopyStringBytes(NewStringPos, StartPos, len);
			NewStringPos += len;
			CopyStringBytes(NewStringPos, SubString, LenSub);
			NewStringPos += LenSub;

			StartPos = se->mPos + LenSearch;
		}
		CopyStringBytes(NewStringPos, StartPos, strlen(StartPos));
		slist.ClearList();
	}
	else
	{
		NewString = new char[strlen(SrcString) + 1];
		strcpy(NewString, SrcString);
	}

	return NewString;
}
#endif

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadBytes
//
// Parameter: void* buffer - Zeiger auf Puffer, in den die Datei eingelesen
//                           werden
//
//            int nBytes   - Anzahl der Bytes, die eingelesen werden sollen
//
// Return:    true  - Daten wurden eingelesen
//            false - Daten konnten nicht eingelesen werden
//
//
//
// Einlesen einer Anzahl von Bytes aus einer geoeffneten Datei
//
//---------------------------------------------------------------------------

bool CFileIO::ReadBytes(void* buffer, int nBytes)
{
  if (mOpenRead)
  {
    mFs->read((char*) buffer, nBytes);
  }
  return mOpenRead;
}

#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadObject
//
// Parameter: Tobj* obj - Zeicer auf Objekt, das gelesen werden soll
//
//
// Einlesen eines Objekts aus einer geoeffneten Datei
//
//---------------------------------------------------------------------------

template <typename Tobj>
bool CFileIO::ReadObject(Tobj* obj)
{
	return ReadBytes(obj, sizeof(Tobj));
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteObject
//
// Parameter: Tobj* obj - Zeiger auf Objekt, das geschrieben (const)
//
//
// Schreiben eines Objekts in eine goeffnete Datei
//
//---------------------------------------------------------------------------

template <typename Tobj>
bool CFileIO::WriteObject(const Tobj* obj)
{
	return WriteBytes(obj, sizeof(Tobj));
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteObject
//
// Parameter: Tobj& obj - Zeiger auf Objekt, das geschrieben (const)
//
//
// Schreiben eines Objekts in eine goeffnete Datei
//
//---------------------------------------------------------------------------

template <typename Tobj>
bool CFileIO::WriteObject(const Tobj& obj)
{
	return WriteBytes(&obj, sizeof(Tobj));
}
#endif


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binaer-Modus geoeffnet sein
//
//---------------------------------------------------------------------------

bool CFileIO::ReadLine(vector<char>* Line)
{
	char ch;
	bool EndLine = false;
	bool IsEof = false;

	Line->clear();
	do
	{
		mFs->get(ch);
		if (!mFs->eof())
		{
			if ((ch == 0x0D) || (ch == 0x0A))
			{
				EndLine = true;
			}
			else
			{
				Line->push_back(ch);
			}
		}
		else
		{
			EndLine = true;
			IsEof   = true;
		}
	}
	while (!EndLine);

	cout << endl;

	return IsEof;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binaer-Modus geoeffnet sein
//
//---------------------------------------------------------------------------

bool CFileIO::ReadLine(string* Line)
{
	char ch;
	bool EndLine = false;
	bool IsEof = false;

	Line->clear();
	do
	{
		mFs->get(ch);
		if (!mFs->eof())
		{
			if (ch != 0x0D)
			{
				if (ch != 0x0A)
				{
        	Line->push_back(ch);
				}
				else
				{
					EndLine = true;
				}
			}
			else
			{
				EndLine = true;
			}
		}
		else
		{
			EndLine = true;
			IsEof   = true;
		}
	}
	while (!EndLine);

	return !IsEof;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadAllLines
//
// Nur Zeilen, die nicht leer sind, werden gespeichert
//
//---------------------------------------------------------------------------


void CFileIO::ReadAllLines(vector<string>& LineVec, char CommentSign)
{
	bool eof;
	string Line;
	do
	{
		eof = !ReadLine(&Line);
		if (Line.size() > 0)
		{
			if (Line.at(0) != CommentSign) // kommentarzeichen
			{
				LineVec.push_back(Line);
			}
		}
	}
	while (!eof);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadSplitLine
//
//
//---------------------------------------------------------------------------

bool CFileIO::ReadSplitLine(stringvector* SplitVector, char SplitChar)
{

	bool r = false;

	if (SplitVector != NULL)
	{
		string LineBuffer;

		r = ReadLine(&LineBuffer);
		SplitVector->clear();
		NStringTool::Split(LineBuffer, SplitVector, SplitChar);
	}
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadBytes
//
// Parameter:
//
// Return:
//
//
// Lesen von Bytes aus einer Datei
//
//---------------------------------------------------------------------------

void* CFileIO::ReadBytes(int nBytes)
{
	char* buffer = NULL;
  if (mOpenRead)
  {
		buffer = new char[nBytes];
    mFs->read(buffer, nBytes);
  }
  return buffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CloseFile
//
// Parameter:
//
// Return:
//
//
// Schliessen einer Datei
//
//---------------------------------------------------------------------------

void CFileIO::CloseFile()
{
  if (mOpenWrite || mOpenRead)
  {
    mFs->close();
    mOpenRead = false;
    mOpenWrite = false;
    delete mFs;
 	  mFs = NULL;
  }
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

bool CFileIO::SetReadPos(int pos)
{
	mFs->seekg(pos, ios::beg);
	return (int) mFs->tellg() == pos;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

bool CFileIO::SetReadPos2(int pos, SEEKDIR where)
{
	mFs->seekg(pos, where);
	return (int) mFs->tellg() == pos;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::SetWritePosEnd()
{
	mFs->seekp(0, ios_base::end);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::SetWritePos(unsigned int pos)
{
	mFs->seekp(pos, ios_base::beg);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetReadPos
//
//
//
//
//---------------------------------------------------------------------------

int CFileIO::GetReadPos()
{
	return mFs->tellg();
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetWritePos
//
//
//
//
//---------------------------------------------------------------------------

int CFileIO::GetWritePos()
{
	return mFs->tellp();
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		GuessFileFormat
//
// Return:
//
//  EFILETYPE_UNKNOWN,
// 	EFILETYPE_JPG,
// 	EFILETYPE_TIF,
// 	EFILETYPE_PNG,
// 	EFILETYPE_TGA,
// 	EFILETYPE_RAW
//
//---------------------------------------------------------------------------

EFileType CFileIO::GuessFileFormat(const char* Filename)
{
	const char* AppList[] =
	{
		".JPG",
		".TIF",
		".PNG",
		".TGA",
		".RAW",
		".GIF",
		NULL
	};

	int  FileType;
	char FilenameCopy[256];
	strcpy(FilenameCopy, Filename);

	if (strlen(FilenameCopy) > 4)
	{
		for (int i = 0; FilenameCopy[i] != 0; i++)
		{
			FilenameCopy[i] = toupper(FilenameCopy[i]);
		}

		for (int i = 0; AppList[i] != NULL; i++)
		{
			char* FilenameCopyPtr = FilenameCopy + strlen(FilenameCopy) - strlen(AppList[i]);
			if (strcmp(FilenameCopyPtr, AppList[i]) == 0)
			{
				EFileType ftab[] =
				{
					EFILETYPE_UNKNOWN,
					EFILETYPE_VOID,
					EFILETYPE_JPG,
					EFILETYPE_TIF,
					EFILETYPE_PNG,
					EFILETYPE_TGA,
					EFILETYPE_RAW,
					EFILETYPE_KMZ
				};


				return ftab[i + 2];
			}
		}
	}
	return EFILETYPE_UNKNOWN;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ReplaceApp
//
//---------------------------------------------------------------------------

char* CFileIO::ReplaceApp(const char* Filename, const char* neuApp)
{
  char  NameBuffer[512];
  char* NeuFilename;

  strcpy(NameBuffer, Filename);
  char* Punkt = strrchr(NameBuffer, '.');

  if (Punkt != NULL) *Punkt = 0;

  strcat(NameBuffer, neuApp);

  NeuFilename = new char[strlen(NameBuffer) + 1];

  strcpy(NeuFilename, NameBuffer);

  return NeuFilename;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ReplaceApp
//
//---------------------------------------------------------------------------

std::string CFileIO::ReplaceAppStr(const char* Filename, const char* neuApp)
{
  char  NameBuffer[512];
  std::string NeuFilename;

  strcpy(NameBuffer, Filename);
  char* Punkt = strrchr(NameBuffer, '.');

  if (Punkt != NULL) *Punkt = 0;

  strcat(NameBuffer, neuApp);

  NeuFilename = NameBuffer;

  return NeuFilename;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		Strcmpx
//
//---------------------------------------------------------------------------

bool CFileIO::Strcmpx(const char* s1, const char* s2)
{
	if ((s1 == NULL) || (s2 == NULL))
	{
		return false;
	}

	char* bstr1 = new char[strlen(s1) + 1];
	char* bstr2 = new char[strlen(s2) + 1];

	strcpy(bstr1, s1);
	strcpy(bstr2, s2);

	for (char* c1 = bstr1; *c1 != 0; c1++)
	{
		*c1 = toupper(*c1);
	}
	for (char* c1 = bstr2; *c1 != 0; c1++)
	{
		*c1 = toupper(*c1);
	}

	int s = strcmp(bstr1, bstr2);

	delete[] bstr2;
	delete[] bstr1;

	return s == 0;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ResolveWildCard
//
//---------------------------------------------------------------------------

bool CFileIO::ResolveWildCard(const char* Dir, const char* WildCard, stringvector* FileList)
{
	struct stat FileInfo;
  struct dirent *CurrentFile;
  DIR*   Directory;
  char** DirList = NULL;
  bool   r = false;

	Directory = opendir(Dir);

	//cout << "CFileIO::ResolveWildCard Dir=" << Dir << " Wildcard=" << WildCard << endl;

	if (WildCard[0] == '*')
	{
		WildCard++;
	}
	FileList->clear();

	if (Directory != NULL)
	{
		r = true;
		Directory = opendir(Dir);

		int i = 0;
		while ((CurrentFile = readdir(Directory)) != NULL)
		{
			char* EndString = CurrentFile->d_name + (strlen(CurrentFile->d_name) - strlen(WildCard));

			if (Strcmpx(EndString, WildCard))
			{
				string Filename = CurrentFile->d_name;
				FileList->push_back(Filename);
			}

		//cout << "File:" << CurrentFile->d_name << endl;
		}
		closedir(Directory);
	}
	return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ListDir
//
//---------------------------------------------------------------------------

bool CFileIO::ListDir(const char* Dir, stringvector* FileList)
{
	struct stat FileInfo;
  struct dirent *CurrentFile;
  DIR*   Directory;
  char** DirList = NULL;
  bool   r = false;

	Directory = opendir(Dir);



	FileList->clear();

	if (Directory != NULL)
	{
		r = true;
		Directory = opendir(Dir);

		int i = 0;
		while ((CurrentFile = readdir(Directory)) != NULL)
		{

			string Filename = CurrentFile->d_name;

			if ((Filename != ".") && (Filename != ".."))
			{

				FileList->push_back(Filename);
			}

		//cout << "File:" << CurrentFile->d_name << endl;
		}
		closedir(Directory);
	}
	return r;
}



//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ResolveWildCard
//
//---------------------------------------------------------------------------

char** CFileIO::ResolveWildCard(const char* Dir, const char* WildCard, int* DirCount)
{
	struct stat FileInfo;
  struct dirent *CurrentFile;
  DIR*   Directory;
  char** DirList = NULL;

  *DirCount = 0;
	Directory = opendir(Dir);

	cout << "CFileIO::ResolveWildCard Dir=" << Dir << " Wildcard=" << WildCard << endl;

	if (WildCard[0] == '*')
	{
		WildCard++;
	}

	if (Directory != NULL)
	{
		while ((CurrentFile = readdir(Directory)) != NULL)
		{
		  char* EndString = CurrentFile->d_name + (strlen(CurrentFile->d_name) - strlen(WildCard));

		  //cout << "Filename=" << CurrentFile->d_name <<  " EndString=" << EndString << endl;

		  if (Strcmpx(EndString, WildCard))
		  {
				(*DirCount)++;
			}
		}
		closedir(Directory);

		if (*DirCount > 0)
		{
			DirList = new char*[*DirCount + 1];

			Directory = opendir(Dir);

			int i = 0;
			while ((CurrentFile = readdir(Directory)) != NULL)
			{
			  char* EndString = CurrentFile->d_name + (strlen(CurrentFile->d_name) - strlen(WildCard));

			  if (Strcmpx(EndString, WildCard))
			  {
					DirList[i++] = NewString(CurrentFile->d_name);
				}

			//cout << "File:" << CurrentFile->d_name << endl;
			}
			closedir(Directory);
    	DirList[*DirCount] = NULL;
    }
	}
	return DirList;

}


#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   FileType (FileExists)
//
// Parameter: char* Foldername - Name des Ordners
//
// Return:    true  - Datei existiert
//            false - Datei existiert nicht
//
//---------------------------------------------------------------------------


int CFileIO::FileType(const char* Filename)
{
	struct stat attribut;
	int r;
	if (stat(Filename, &attribut))
	{
		if (attribut.st_mode & S_IFREG)
		{
			r = 1; // Regulaere Datei
		}
		else
		if (attribut.st_mode & S_IFDIR)
		{
			r = 2; // Ordner
		}
		else
		{
			r = -1; // unbekannt/nicht unterstuetzt
		}

	}
	else
	{
		r = 0;
	}
	return r;
}
#endif






//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   FileExists
//
// Parameter: char* Foldername - Name des Ordners
//
// Return:    true  - Datei existiert
//            false - Datei existiert nicht
//
//---------------------------------------------------------------------------

#if 1
bool CFileIO::FileExists(const char* Filename)
{


/*
#ifdef _WIN32

	OFSTRUCT ofstruct;

    ofstruct.
	// OpenFile liefert TRUE zurueck, wenn die Datei existiert
	// und OF_EXIST gegeben ist.

	bool r = OpenFile(Filename, &ofstruct, OF_EXIST);
	return r;

#else
*/
	struct stat attribut;
	return stat(Filename, &attribut) == 0;


}
#endif



#ifdef __WIN32

// ---------------------------------------------------------------------------
//
// METHODE       : ResolveWildCards
//
// KLASSE        : CParserExt
//
// RETURN        : char** - Zeiger auf Dateiliste (endet mit NULL)
//
// PARAMETER     : char* WildCardString - String mit Wildcards (*, ?)
//
// BESCHREIBUNG  : Erstellen einer Datei-Liste aus einem Wildcard-String
//
// HISTORY       :
//
// 28.11.2001 erstellt
//
// ---------------------------------------------------------------------------

char** CFileIO::ResolveWildCards(const char* WildCardString, int* nFiles)
{
	WIN32_FIND_DATA FindData;
	int AnzahlFiles = 0;

	HANDLE fhand = FindFirstFile(WildCardString, &FindData);

	if (fhand != INVALID_HANDLE_VALUE)
	{
		list<char*> FileList;

    do
    {
			char* Filename = new char[strlen(FindData.cFileName) + 1];
    	strcpy(Filename, FindData.cFileName);

    	FileList.push_back(Filename);

    	AnzahlFiles++;
		}
    while (FindNextFile(fhand, &FindData));

    char** FilePtrList = new char*[AnzahlFiles + 2];

		int i = 0;
		for (list<char*>::iterator fn  = FileList.begin();
		                           fn != FileList.end();
		                           fn++)
		{
			FilePtrList[i++] = *fn;
		}
		FilePtrList[AnzahlFiles] = NULL;

		if (nFiles != NULL)
		{
			*nFiles = AnzahlFiles;
		}
		FileList.clear();

		return FilePtrList;
	}

	char** FilePtrList = new char*[1];
	FilePtrList[0] = NULL;

	if (nFiles != NULL)
	{
		*nFiles = AnzahlFiles;
	}

	return FilePtrList;
}
#endif


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		StripExtension
//
// Liefert den Dateinamen ohne Datei-Anhang zurueck
//
//---------------------------------------------------------------------------

string CFileIO::StripExtension(const char* FullPathName)
{
	string OutString;
	char* TmpName = NewString(FullPathName);
  char* slash = strrchr(TmpName, '.');

  if (slash != NULL)
  {
		*slash = 0;
		OutString = TmpName;
	}
  else
  {
		OutString = FullPathName;
	}
	return OutString;
}

//---------------------------------------------------------------------------
//
//
// Klasse:  DeleteStringList
// Methode:
//
//
//---------------------------------------------------------------------------

void CFileIO::DeleteStringList(char** StringList)
{
	if (StringList != NULL)
	{
		for (int i = 0; StringList[i] != NULL; i++)
		{
			delete StringList[i];
		}
		delete StringList;
	}
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffuellen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

char* CFileIO::Fill0(int i, int n)
{
	static char strbuffer[32];
	return Fill0(strbuffer, i, n);
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffuellen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

string CFileIO::Fill0str(int i, int n)
{
	string fstr = Fill0(i, n);
	return fstr;
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffuellen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

char* CFileIO::Fill0(char* strbuffer, int i, int n)
{
	sprintf(strbuffer, "%d", i);

	char len = strlen(strbuffer);

	if (len < n)
	{
		char strbuffer2[16];

		strcpy(strbuffer2, strbuffer);
		strcpy(strbuffer + (n - len), strbuffer2);

		for (int j = 0; j < n - len; j++)
		{
			strbuffer[j] = '0';
		}
	}
	return strbuffer;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CreateDir
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CFileIO::CreateDir(const char* NewFolder)
{
	#if defined _WIN32

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	return CreateDirectory(NewFolder, &sa);

  #else
	mkdir(NewFolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  #endif
	return 1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CreateFullPath
//
// Parameter: PathName - Voller Pfadname (mit Dateinamen)
//            PathDiff - Pfadtrenner \ fuer Windows, / fuer Unix
//
// Return:
//
//  Es werden Verzeichnisse fuer alle Teile des Pfades erzeugt, ausser
//  fuer den ersten und den letzten Teil
//
//  Beispiel:
//
//   c:\daten\bilder\afpel.jpg
//
//  Erzeugt: "daten" und "bilder"
//
//---------------------------------------------------------------------------

void CFileIO::CreateFullPath(const string& PathName, char PathDiff)
{
	stringvector PathParts;
	NStringTool::Split(PathName, &PathParts, PathDiff);


	if (PathParts.size() > 1)
	{
		string PathName = PathParts[0];
		for (int i = 1; i < PathParts.size() - 1; i++)
		{
			PathName = PathName + PathDiff + PathParts[i];
			CreateDir(PathName.c_str());
		}
	}
}




//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CreateDir
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CFileIO::CreateDir(const string& NewFolder)
{
		return CreateDir(NewFolder.c_str());
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   NewString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

char* CFileIO::NewString(const char* OldStr)
{
  char* NewStr = new char[strlen(OldStr) + 1];
  strcpy(NewStr, OldStr);
  return NewStr;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ExtractFilename
//
//---------------------------------------------------------------------------

char* CFileIO::ExtractFilename(const char* FullPathName)
{
  const char* slash = strrchr(FullPathName, '/');

  if (slash == NULL)
  {
		slash = strrchr(FullPathName, '\\');

		if (slash == NULL)
		{
			return NewString(FullPathName);
		}
	}
  return NewString(slash + 1);
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ExtractPath
//
//---------------------------------------------------------------------------

const char* CFileIO::ExtractPath(const char* FullPathName)
{
	char* PathName = new char[strlen(FullPathName) + 1];

  strcpy(PathName, FullPathName);

  char* slash = strrchr(PathName, '\\');

 // cout << "Slash=" << slash << endl;

  if (slash != NULL)
  {
		slash[1] = 0;
	}
	else
	{
		slash = strrchr(PathName, '/');
		if (slash != NULL)
		{
			slash[1] = 0;
		}
	}
  return PathName;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   AppendData
//
// Daten werden an File1 angehaengt
//
//---------------------------------------------------------------------------

bool CFileIO::AppendData(const char* File1, const void* Data, int DataLen)
{
	bool Success = false;

	if (Data != NULL)
	{
		if (FileExists(File1))
		{
			OpenFileAppend(File1, ios::binary);
		}
		else
		{
			OpenFileWrite(File1, ios::binary);
		}
		if (mOpenWrite)
		{
			WriteBytes(Data, DataLen);
			CloseFile();
			Success = true;
		}
	}
	return Success;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadStringVector
//
//
//---------------------------------------------------------------------------

bool CFileIO::ReadStringVector(const std::string Filename, stringvector& StringVec)
{
	bool r = OpenFileRead(Filename, ios::binary);

	if (r)
	{
		std::string InputString;
		while (ReadLine(&InputString))
		{
			StringVec.push_back(InputString);
		}
		CloseFile();
	}
	return r;
}



