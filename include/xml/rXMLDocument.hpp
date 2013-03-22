#ifndef R_XMLDOCUMENT_HPP
#define R_XMLDOCUMENT_HPP

#include <fstream>

#include "rTypes.hpp"
#include "xml/rXMLElement.hpp"

#include "xml/rXMLWriter.hpp"
#include "xml/rXMLReader.hpp"
#include "xml/rXMLDocumentLoader.hpp"

class rXMLDocument{
public:
	rXMLDocument();
	~rXMLDocument();
	
	void Clear();
	rXMLElement* GetRoot() const;
	rXMLElement* CreateRoot(const rString& name);
	
	int WriteToFile(const rString& path) const;
	int WriteToStream(std::ostream& stream) const;
	
	rXMLReaderError LoadFromFile(const rString& path);
	rXMLReaderError LoadFromStream(std::istream& stream);
	
private:
	
	void WriteXMLElement(const rXMLElement* element, rXMLWriter& writer) const;
	
private:
	
	rXMLElement* m_root;
};

#endif
