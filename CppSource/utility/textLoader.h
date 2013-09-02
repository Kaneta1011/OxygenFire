#ifndef	_TEXT_LOADER
#define	_TEXT_LOADER

//*****************************************************************************
//
//		テキストファイル操作クラス
//
//		8/7	:　assetsからの読み込み完了
//*****************************************************************************
class	textLoader {
public:
	enum READ_LOCATION
	{
		LOCATION_ASSETS,
		LOCATION_LOCAL,
		LOCATION_SD_CARD,
	};

public:
		bool	EndCheck();
		void	LoadString( char* str );	//	文字列取得
		void	LoadName( char* str );		//	"名前"文字列取得
		float	LoadFloat( void );			//	小数点数
		int		LoadInt( void );			//	整数

		void	LoadStringTest( char* str );//	文字列取得（ポインタ移動なし）

		void	SetIndex( int index );
		void	Skip( void );	//	{}スキップ
		void	In( void );		//	{侵入
		void	Out( void );	//	}外へ

		void	newLine();		//	次の行へ

		unsigned long	GetPointer(){ return ReadIndex; }
		void	SetPointer( unsigned long p ){ ReadIndex = p; }

		void	push();		//	現在位置保存
		void	pop();		//	位置復元

		//	検索
		bool	Search( const char* str );
		bool	Search2( const char* str );
		bool	SearchIndent( const char* str );
		bool	SearchLine( const char* str );

		//	初期化・解放
		textLoader( const char* filename, READ_LOCATION readLocation=LOCATION_ASSETS );
		textLoader(){}

		~textLoader()
		{
			if( FileBuf ) delete[] FileBuf;		
		}

private:
		bool	bInit;

		char	*FileBuf;		//	ファイルバッファ
		unsigned long	TextSize;		//	ファイルサイズ
		unsigned long	ReadIndex;		//	現在位置ポインタ
		
		unsigned long	PushIndex;
		int		SearchTop();	//	頭出し

protected:
		bool	Open( const char* filename, READ_LOCATION readLocation );

};

#endif

