#ifndef	_TEXT_LOADER
#define	_TEXT_LOADER

//*****************************************************************************
//
//		�e�L�X�g�t�@�C������N���X
//
//		8/7	:�@assets����̓ǂݍ��݊���
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
		void	LoadString( char* str );	//	������擾
		void	LoadName( char* str );		//	"���O"������擾
		float	LoadFloat( void );			//	�����_��
		int		LoadInt( void );			//	����

		void	LoadStringTest( char* str );//	������擾�i�|�C���^�ړ��Ȃ��j

		void	SetIndex( int index );
		void	Skip( void );	//	{}�X�L�b�v
		void	In( void );		//	{�N��
		void	Out( void );	//	}�O��

		void	newLine();		//	���̍s��

		unsigned long	GetPointer(){ return ReadIndex; }
		void	SetPointer( unsigned long p ){ ReadIndex = p; }

		void	push();		//	���݈ʒu�ۑ�
		void	pop();		//	�ʒu����

		//	����
		bool	Search( const char* str );
		bool	Search2( const char* str );
		bool	SearchIndent( const char* str );
		bool	SearchLine( const char* str );

		//	�������E���
		textLoader( const char* filename, READ_LOCATION readLocation=LOCATION_ASSETS );
		textLoader(){}

		~textLoader()
		{
			if( FileBuf ) delete[] FileBuf;		
		}

private:
		bool	bInit;

		char	*FileBuf;		//	�t�@�C���o�b�t�@
		unsigned long	TextSize;		//	�t�@�C���T�C�Y
		unsigned long	ReadIndex;		//	���݈ʒu�|�C���^
		
		unsigned long	PushIndex;
		int		SearchTop();	//	���o��

protected:
		bool	Open( const char* filename, READ_LOCATION readLocation );

};

#endif

