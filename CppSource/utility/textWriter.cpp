#include "textWriter.h"

using namespace rlib;

textWriter::textWriter():
mNestChar(' '),
mDepth(0)
{
}

textWriter::~textWriter()
{
}

void textWriter::close()
{
	if( good() ){
		this->mStream.close();
	}
	nestClear();
}

bool textWriter::open(const char* filePath)
{
	close();
	this->mStream.open(filePath);
	return good();
}

textWriter& textWriter::write(char c)
{
	this->mStream.put(c);
	return *this;
}

textWriter& textWriter::write(const char* data)
{
	size_t size = strlen(data);
	this->mStream.write(data, size);
	return *this;
}

textWriter& textWriter::write(int value)
{
	this->mStream << value;
	return *this;
}

textWriter& textWriter::write(float value)
{
	this->mStream << value;
	return *this;
}

textWriter& textWriter::write(double value)
{
	this->mStream << value;
	return *this;
}

void textWriter::nestIn()
{
	this->mDepth ++;
}

void textWriter::nestOut()
{
	this->mDepth --;
	if( this->mDepth < 0 ) this->mDepth = 0;
}

void textWriter::nestClear()
{
	this->mDepth = 0;
}

textWriter& textWriter::br()
{
	this->mStream << std::endl;
	writeNest();
	return *this;
}

bool textWriter::good()
{
	return this->mStream.good();
}

bool textWriter::bad()
{
	return this->mStream.bad();
}

textWriter& textWriter::writeNest()
{
	for( int i=0; i<this->mDepth; i++ )
	{
		this->mStream.put( this->mNestChar );
	}
	return *this;
}
