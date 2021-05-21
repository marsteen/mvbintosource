//------------------------------------------------------------------------------
//
// PROJECT : Bin2Source
// FILE    : main.cpp
// VERSION : 1.0
// AUTOR   : Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <CFileIO.h>

using namespace std;

enum
{
	DTYPE_BYTE,
	DTYPE_UBYTE,
	DTYPE_INT,
	DTYPE_UINT,
	DTYPE_SHORT,
	DTYPE_USHORT,
	DTYPE_LONG,
	DTYPE_ULONG
};

const char* dataTypeString[]
{
	"int8_t",
	"uint8_t",
	"int32_t",
	"uint32_t",
	"int16_t",
	"uint16_t",
	"int64_t",
	"uint64_t"
};


static char* MakeHexByte(unsigned char n)
{
	static char  hbuf[3];
	const char* htab = "0123456789ABCDEF";

	hbuf[0] = htab[n >> 4];
	hbuf[1] = htab[n & 0xF];
	hbuf[2] = 0;

	return hbuf;
}


std::string MakeHexBytes(const unsigned char* bytes, int n)
{
	std::string HexString;
	for (int i = 0; i < n; i++)
	{
	  HexString += MakeHexByte(bytes[n - i - 1]);
	}
	return HexString;
	
}


std::string filternameString(const char* str)
{
    std::string outstr;
	for (int i = 0; str[i] != 0; i++)
	{
		char c = str[i];
		if (
		    ((c >= 'a') && (c <= 'z')) ||
  		    ((c >= 'A') && (c <= 'Z')) ||  		    
  		    ((c >= '0') && (c <= '9'))
           )
		{
			outstr += c;
		}
		else
		{
			outstr += '_';
		}
	}
	return outstr;
}



template <typename T>
bool WriteTextFile(const char* InputFile, bool WriteHex, int dataType)
{
	CFileIO fio;
	int     FileSize;
	bool    r = false;
    const int lineLimit = 32 / sizeof(T);

	const T* FileData = (T*) fio.ReadFile(InputFile, &FileSize);
	if (FileData != NULL)
	{
		int x = 0;
		
		stringvector sp;
		std::string fname;
		NStringTool::Split(InputFile, &sp, '/');
		if (sp.size() > 1)
		{
		   fname = sp[sp.size() - 1];
		}
		else
		{
			fname = InputFile;
		}
				
		std::string FldName = std::string("array_") + filternameString(fio.ReplaceApp(fname.c_str(), ""));
		char* SrcFilename = fio.ReplaceApp(InputFile, ".cpp");
		
		ofstream fout(SrcFilename);

		if (fout.good())
		{
			r = true;
			
			fout << "#define SIZE_" << FldName << " " << FileSize << '\n';
			fout << dataTypeString[dataType] << " " << FldName << "[] = \n";
			fout << "{\n";
			
			int DataSize = FileSize / sizeof(T);

			for (int i = 0; i < DataSize; i++)
			{
				if (x == 0)
				{
					fout << "  ";
				}
				
				//fout << FileData[i];

				//fout << "0x" << MakeHexByte(FileData[i]);
				
				if (WriteHex)
				{
					fout.setf(std::ios::hex, std::ios::basefield);
					fout << "0x" << FileData[i];
					
					//fout << "0x" << MakeHexBytes((unsigned char*) (FileData + i), sizeof(T));
				}
				else
				{
					fout << FileData[i];
				}

				if (i != FileSize-1)
				{
					fout << ',';
				}

				if (x++ == lineLimit)
				{
					fout << '\n';
					x = 0;
				}
			}
			fout << "\n};\n";
			fout.close();
		}

		delete[] SrcFilename;
		fio.CloseFile();
	}	
	return r;
}




int main(int argc, char* argv[])
{


	if (argc < 2)
	{
		cout << "usage bin2source <file> [options]" << endl;
		cout << "version 1.2" << endl;
		cout << "options:" << endl;
		cout << " -dec:    decimal output" << endl;
		cout << " -byte:   8 bit int output" << endl;
		cout << " -ubyte:  8 bit unsigned int output (default)" << endl;		
		cout << " -short:  16 bit int output" << endl;
		cout << " -ushort: 16 bit unsigned int output" << endl;		
		cout << " -int:    32 bit int output" << endl;
		cout << " -uint:   32 bit unsigned int output" << endl;
		cout << " -long:   64 bit int output" << endl;
		cout << " -ulong:  64 bit unsigned int output" << endl;
		
	}
	else
	{
		bool hexwrite = true;
		int  datatype = DTYPE_UBYTE;
        int filenameIndex = 1;
		
		for (int i = 1; i < argc; i++)
		{
			std::string option = argv[i];
						
			if (option == "-dec")
			{
				hexwrite = false;
			}
			else
			if (option == "-int")
			{
				datatype = DTYPE_INT;
			}
			else
			if (option == "-uint")
			{
				datatype = DTYPE_UINT;
			}
			else
			if (option == "-short")
			{
				datatype = DTYPE_SHORT;
			}
			else
			if (option == "-ushort")
			{
				datatype = DTYPE_USHORT;
			}
			else
			if (option == "-long")
			{
				datatype = DTYPE_LONG;
			}
			else
			if (option == "-ulong")
			{
				datatype = DTYPE_ULONG;
			}
			else
            {
              filenameIndex = i;  
            }
		}
		
		switch (datatype)
		{	
			case DTYPE_UBYTE:
				
				WriteTextFile<uint8_t>(argv[filenameIndex], hexwrite, datatype);
				break;
				
			case DTYPE_BYTE:
				
				WriteTextFile<int8_t>(argv[filenameIndex], hexwrite, datatype);
				break;

			case DTYPE_SHORT:
				
				WriteTextFile<int16_t>(argv[filenameIndex], hexwrite, datatype);
				break;
				
			case DTYPE_USHORT:
				
				WriteTextFile<uint16_t>(argv[filenameIndex], hexwrite, datatype);
				break;

			case DTYPE_INT:
				
				WriteTextFile<int32_t>(argv[filenameIndex], hexwrite, datatype);
				break;

			case DTYPE_UINT:
				
				WriteTextFile<uint32_t>(argv[filenameIndex], hexwrite, datatype);
				break;
				
			case DTYPE_LONG:
				
				WriteTextFile<int64_t>(argv[filenameIndex], hexwrite, datatype);
				break;
				
			case DTYPE_ULONG:
				
				WriteTextFile<uint64_t>(argv[filenameIndex], hexwrite, datatype);
				break;
				
		}
	}
	return 0;
}
