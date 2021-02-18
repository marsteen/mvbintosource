#ifndef CFILEIO_HPP
#define CFILEIO_HPP

#include <CFileIO.h>

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


#endif