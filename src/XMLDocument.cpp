#include "XMLDocument.h"

#include "MemoryManager.hpp"

Xml::Document::Document(const char* filename)
{
	_doc = xmlReadFile(filename, NULL, XML_PARSE_RECOVER );
	if (_doc == NULL)
		throw 0;
	_rootNode = NULL;
}

Xml::Node*	Xml::Document::getRootNode()
{
	if (!this->_rootNode)
	{
		this->_rootNode = new Xml::Node(xmlDocGetRootElement(this->_doc), this->_doc);
	}
	return this->_rootNode;
}

Xml::Document::~Document()
{
	xmlFreeDoc(_doc);
}

xmlDocPtr		Xml::Document::getDocumentPtr() const
{
	return this->_doc;
}
