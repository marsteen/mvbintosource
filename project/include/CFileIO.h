
#ifndef CFILEIO_H
#define CFILEIO_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <EFileType.h>
#include <NStringTool.h>



#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__DESKTOP__) || defined(__IOS__)
#define OPENMODE std::_Ios_Openmode
#define SEEKDIR  std::_Ios_Seekdir
#else
#define OPENMODE std::ios_base::openmode
#define SEEKDIR  std::ios_base::seekdir
#endif	

enum EFileAttribut
{
	EFA_NOT_FOUND,
	EFA_FILE,
	EFA_FOLDER
};

class CFileIO
{
	public:

		CFileIO()
		{
			mOpenRead  = false;
			mOpenWrite = false;
			mFs = NULL;
		}

		~CFileIO()
		{
			delete mFs;
		}

		static bool   WriteFile(const char* Filename, const void* Buffer, int nBytes);
		static void*  ReadFile(const char* Filename, int* nBytes);
		static void*  ReadFile(const std::string Filename, int* nBytes);
		static int  GetFileSize(const char* Filename);
		static EFileType GuessFileFormat(const char* Filename);
		static char*  ReplaceApp(const char* Filename, const char* neuApp);
		static std::string  ReplaceAppStr(const char* Filename, const char* neuApp);
		static bool   CreateDir(const char* NewFolder);
		static bool   CreateDir(const std::string& NewFolder);
		static char*  NewString(const char* OldStr);
		static void   CreateFullPath(const std::string& PathName, char PathDiff);
		static bool   ListDir(const char* Dir, stringvector* FileList);



		bool   OpenFileRead(const char* Filename, OPENMODE type);
		bool   OpenFileRead(const std::string Filename, OPENMODE OpenMode);
		bool   OpenFileWrite(const char*Filename, OPENMODE type);
		bool   OpenFileWrite(const std::string Filename, OPENMODE type);
		bool   OpenFileAppend(const char* Filename, OPENMODE type);
		bool   OpenFileChange(const char* Filename, int Position);
		bool 	 ReadBytes(void* buffer, int nBytes);

		void*	 ReadBytes(int nBytes);
		bool   WriteBytes(const void* buffer, int nBytes);
		bool   WriteString(const char* str);

		char*  ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count);
		int 	 GetChar();
		bool   IsEof();
		void   Clear();
		void   SkipBytes(int nBytes);
		bool   ReadLine(std::vector<char>* Line);
		bool   ReadLine(std::string* Line);
		bool   ReadSplitLine(stringvector* SplitVector, char SplitChar);
		bool   AppendData(const char* File1, const void* Data, int DataLen);
		void   ReadAllLines(std::vector<std::string>& LineVec, char CommentSign);
		int    WriteFromFile(const char* Filename);
		bool   ReadStringVector(const std::string Filename, stringvector& StringVec);


		std::string StripExtension(const char* FullPathName);


		char** ResolveWildCards(const char* WildCardString, int* nFiles);
		char** ResolveWildCard(const char* Dir, const char* WildCard, int* DirCount);
		static bool   ResolveWildCard(const char* Dir, const char* WildCard, std::vector<std::string>* FileList);

		void   DeleteStringList(char** StrList);
		static char*  Fill0(int i, int n);
		static char*  Fill0(char* strbuffer, int i, int n);
    static std::string Fill0str(int i, int n);

    static char*  ExtractFilename(const char* FullPathName);
		static const char*  ExtractPath(const char* FullPathName);
		std::string  ExtractPath(const std::string& InputString, const char PathDif);
		static bool   Strcmpx(const char* s1, const char* s2);
		int    GetInt(const char* str);

		static int  FileType(const char* Filename);
		static bool FileExists(const char* Filename);
    static bool FileCopy(const std::string FileSrc, const std::string FileDst);


		bool Good()
		{
			return (int) mFs->good();
		}


		void  CloseFile();

		bool 	SetReadPos(int pos);
		bool 	SetReadPos2(int pos, SEEKDIR where);
		int  	GetReadPos();

		int  	GetWritePos();
		void  SetWritePos(unsigned int pos);
		void 	SetWritePosEnd();


		//std::streampos GetFileSize(const char* Filename, bool* FileExists=NULL);


		template <typename Tobj>
		bool ReadObject(Tobj* obj);

		template <typename Tobj>
		bool WriteObject(const Tobj* obj);

	protected:

		std::fstream* mFs;
		bool          mOpenWrite;
		bool          mOpenRead;
};

#include <CFileIO.hpp>

#endif
