#ifndef _TEXT_WRITER_
#define _TEXT_WRITER_

#include <fstream>

namespace rlib{
	class textWriter
	{
	public:
		textWriter();
		~textWriter();

		bool open(const char* filePath);
		void close();

		void nestIn();
		void nestOut();
		void nestClear();

		textWriter& br();
		textWriter& writeNest();

		textWriter& write(char c);
		textWriter& write(const char* data);
		textWriter& write(int value);
		textWriter& write(float value);
		textWriter& write(double value);

	public:
		bool good();
		bool bad();

	private:

	private:
		char mNestChar;//�l�X�g�Ɏg������
		int mDepth; //�l�X�g�̐[��

		std::ofstream mStream;
	};

}

#endif