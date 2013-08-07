#include	"TextLoader.h"

#include "utility.h"

#include <stdlib.h>
#include <ctype.h>
#include "assetsLoader.h"

//文字判別関数

inline int isalnum(int c)
{
	return (c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_U|_L|_N)));
}

inline bool IsDBCSLeadByte(char c)
{
	return c > 0x7f;
}

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

bool	textLoader::Open( const char* filename )
{
	FileBuf = NULL;
	int size;
	AssetsLoader::load(&FileBuf, &size, filename);
	TextSize = (unsigned long)size;
	ReadIndex = 0;

	return true;
}

bool	textLoader::EndCheck()
{
	if( ReadIndex < TextSize ) return true;
	return false;
}

void	textLoader::SetIndex( int index )
{
	ReadIndex = index;
}
//*****************************************************************************************************************************
//
//*****************************************************************************************************************************

textLoader::textLoader( const char* filename )
{
	bInit = Open(filename);
}

//--------------------------------------------------------------------------------------
//	頭出し
//--------------------------------------------------------------------------------------
int	textLoader::SearchTop( void )
{
	for(;;){
		if( ReadIndex >= TextSize ) return -1;

		if( isalnum(FileBuf[ReadIndex]) ) break;
		if( IsDBCSLeadByte(FileBuf[ReadIndex]) ) break;

		if( FileBuf[ReadIndex] == '{' ) break;
		if( FileBuf[ReadIndex] == '}' ) break;
		if( FileBuf[ReadIndex] == '"' ) return 1;
		if( FileBuf[ReadIndex] == '.' ) break;
		if( FileBuf[ReadIndex] == '-' ) break;
		if( FileBuf[ReadIndex] == '=' ) break;
		ReadIndex ++;
		if( ReadIndex >= TextSize ) return -1;
	}
	return	0;
}

//
void	textLoader::LoadString( char* str )
{
	int		len = 0;
	
	int mode = SearchTop();
	if( ReadIndex >= TextSize ) return;
	//	名前読み込みモード
	if( mode == 1 )
	{
		str[len] = '"';
		len   ++;
		ReadIndex ++;
		for(;;){
			if( IsDBCSLeadByte(FileBuf[ReadIndex]) ){ 
				str[len] = FileBuf[ReadIndex];
				len   ++;
				ReadIndex ++;
			} else {
				if( FileBuf[ReadIndex] == '"' ) break;
			}
			if( ReadIndex >= TextSize ) break;

			str[len] = FileBuf[ReadIndex];
			len   ++;
			ReadIndex ++;
		}
		str[len] = '"';
		len   ++;
		str[len] = '\0';
		ReadIndex ++;
		return;
	}

	for(;;){
		if( ReadIndex>0 && !IsDBCSLeadByte(FileBuf[ReadIndex-1]) ){ 
			if( FileBuf[ReadIndex] == ';' ) break;
			if( FileBuf[ReadIndex] == ')' ) break;
			if( FileBuf[ReadIndex] == '(' ) break;
			if( FileBuf[ReadIndex] == ',' ) break;
			if( FileBuf[ReadIndex] == ' ' ) break;
			if( FileBuf[ReadIndex] == '\t' ) break;
			if( FileBuf[ReadIndex] == 0x0d ) break;
			if( FileBuf[ReadIndex] == 0x0a ) break;
			if( ReadIndex >= TextSize ) break;
		}
		str[len] = FileBuf[ReadIndex];
		len   ++;
		ReadIndex ++;
		str[len+1] = 0;
	}
	str[len] = '\0';
	ReadIndex ++;
}

void	textLoader::LoadName( char* str )
{
	int		len = 0;
	bool	bDQMode = false;

	SearchTop();
	if( ReadIndex >= TextSize ) return;

	if( FileBuf[ReadIndex] == '"' )
	{
		ReadIndex++;
		bDQMode = true;
	}

	for(;;){
		if( ReadIndex>0 && !IsDBCSLeadByte(FileBuf[ReadIndex-1]) ){ 
			if( FileBuf[ReadIndex] == '"' ){ ReadIndex++; break; }
			if( FileBuf[ReadIndex] == '(' ) break;
			if( FileBuf[ReadIndex] == ',' ) break;
			if( !bDQMode && FileBuf[ReadIndex] == ' ' ) break;
			if( FileBuf[ReadIndex] == '\t' ) break;
			if( FileBuf[ReadIndex] == 0x0d ) break;
			if( FileBuf[ReadIndex] == 0x0a ) break;
		}
		str[len] = FileBuf[ReadIndex];
		len   ++;
		ReadIndex ++;
	}
	str[len] = '\0';
	ReadIndex ++;
}

//
void	textLoader::LoadStringTest( char* str )
{
	int		len = 0;

	SearchTop();
	if( ReadIndex >= TextSize ) return;

	unsigned long	WorkIndex = ReadIndex;

	for(;;){
		if( WorkIndex>0 && !IsDBCSLeadByte(FileBuf[WorkIndex-1]) ){ 
			if( FileBuf[WorkIndex] == ')' ) break;
			if( FileBuf[WorkIndex] == '(' ) break;
			if( FileBuf[WorkIndex] == ',' ) break;
			if( FileBuf[WorkIndex] == ' ' ) break;
			if( FileBuf[WorkIndex] == '\t' ) break;
			if( FileBuf[WorkIndex] == 0x0d ) break;
			if( FileBuf[WorkIndex] == 0x0a ) break;
		}
		str[len] = FileBuf[WorkIndex];
		len   ++;
		WorkIndex ++;
	}
	str[len] = '\0';
}

//
float	textLoader::LoadFloat( void )
{
	float	ret;
	char	temp[20];
	LoadString(temp);

	ret = (float)atof(temp);
	return ret;
}

int		textLoader::LoadInt( void )
{
	int		ret;
	char	temp[20];
	LoadString(temp);

	ret = atoi(temp);
	return ret;
}

//		{}読み飛ばし！！
void	textLoader::Skip( void )
{
	int		work = 0;

	if( FileBuf[ReadIndex] != '{' ) work = 1;

	for(;;){
		if( FileBuf[ReadIndex] == '{' ) work ++;
		if( FileBuf[ReadIndex] == '}' ){
			work--;
			if( work <= 0 ) break;
		}
		ReadIndex++;
	}
	ReadIndex++;
}

//		{に入る
void	textLoader::In( void )
{
	for(;;){
		if( ReadIndex >= TextSize ) return;

		if( FileBuf[ReadIndex] == '{' ) break;
		ReadIndex++;
	}
	ReadIndex++;
}

//		{から出る
void	textLoader::Out( void )
{
	int		count = 0;
	for(;;){
		if( ReadIndex >= TextSize ) return;

		if( FileBuf[ReadIndex] == '{' ) count++;
		if( FileBuf[ReadIndex] == '}' ){
			count--;
			if( count < 0 ) break;
		}
		ReadIndex++;
	}
	ReadIndex++;
}



//------------------------------------------------------
//	検索
//------------------------------------------------------
bool	textLoader::Search( const char* str )
{
	char	temp[128];
	while(EndCheck()){
		LoadString(temp);
		if( strcmp( temp, str ) == 0 ) return true;
	}
	return false;
}

//------------------------------------------------------
//	行内検索
//------------------------------------------------------
bool	textLoader::SearchLine( const char* str )
{
	char	temp[128];
	unsigned long	oldIndex = ReadIndex;
	
	//	行頭設定
	for(;;)
	{
		ReadIndex --;
		if( FileBuf[ReadIndex] == '\n' ) break;
		if( FileBuf[ReadIndex] == '\r' ) break;
	}
	ReadIndex ++;
	//	行内検索
	for(;;)
	{
		if( FileBuf[ReadIndex] == '\n' ) break;
		if( FileBuf[ReadIndex] == '\r' ) break;
		if( !isalnum(FileBuf[ReadIndex]) )
		{
			ReadIndex ++;
			continue;
		}
		LoadString(temp);
		if( strcmp( temp, str ) == 0 ) return true;
	}
	ReadIndex = oldIndex;
	return false;
}

//------------------------------------------------------
//	階層内検索
//------------------------------------------------------
bool	textLoader::Search2( const char* str )
{
	char	temp[128];
	int		work = 0;

	if( FileBuf[ReadIndex] != '{' ) work = 1;
	while(EndCheck()){
		LoadString(temp);
		if( strcmp( temp, str ) == 0 ) return true;

		if( strcmp( temp, "{" ) == 0 ){
			work++;
		}
		if( strcmp( temp, "}" ) == 0 ){
			work--;
			if( work <= 0 ) return false;
		}
	}
	return false;
}

//------------------------------------------------------
//	同一インデント内検索
//------------------------------------------------------
bool	textLoader::SearchIndent( const char* str )
{
	char	temp[128];
	int		work = 0;

	unsigned long	oldIndex = ReadIndex;

	for(;;)
	{
		LoadString(temp);
		if( ReadIndex >= TextSize )
		{
			ReadIndex = oldIndex;
			return false;
		}

		if( strcmp( temp, str ) == 0 ) return true;

		if( strcmp( temp, "{" ) == 0 ){
			Skip();
			continue;
		}
		if( strcmp( temp, "}" ) == 0 ){
			work--;
			if( work <= 0 )
			{
				ReadIndex = oldIndex;
				return false;
			}
		}
	}
	ReadIndex = oldIndex;
	return false;
}


void	textLoader::push()
{
	PushIndex = ReadIndex;
}

void	textLoader::pop()
{
	ReadIndex = PushIndex;
}

void	textLoader::newLine()
{
	for(;;){
		if( ReadIndex >= TextSize ) return;
		if( FileBuf[ReadIndex] == '\n' ) break;
		if( FileBuf[ReadIndex] == '\r' ) break;
		ReadIndex++;
	}
}

//*****************************************************************************************************************************
//
//*****************************************************************************************************************************

