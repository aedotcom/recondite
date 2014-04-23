#include "stream/rOFileStream.hpp"

rOFileStream::rOFileStream(){
}

rOFileStream::rOFileStream(const rString& path){
	Open(path);
}

void rOFileStream::Open(const rString& path){
	m_file.open(path.c_str(), std::ios::binary);
}

void rOFileStream::Close(){
	m_file.close();
}

rOStream& rOFileStream::Write(const char* buffer, size_t size){
	m_file.write(buffer, size);
	return *this;
}

void rOFileStream::Seek(size_t pos){
	m_file.seekp(pos);
}

size_t rOFileStream::Pos(){
	return m_file.tellp();
}

bool rOFileStream::IsOk() const{
	return !m_file.fail();
}

rOStream& rOFileStream::operator << (char c){
	m_file << c;
	return *this;
}

rOStream& rOFileStream::operator << (const char* str){
	m_file << str;
	return *this;
}

rOStream& rOFileStream::operator << (unsigned char c){
	m_file << c;
	return *this;
}

rOStream& rOFileStream::operator << (short s){
	m_file << s;
	return *this;
}

rOStream& rOFileStream::operator << (unsigned short s){
	m_file << s;
	return *this;
}

rOStream& rOFileStream::operator << (int i){
	m_file << i;
	return *this;
}

rOStream& rOFileStream::operator << (unsigned int i){
	m_file << i;
	return *this;
}

rOStream& rOFileStream::operator << (float f){
	m_file << f;
	return *this;
}

rOFileStream::operator bool() const{
	return IsOk();
}