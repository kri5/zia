#include "XMLNode.h"

Xml::Node::Node(xmlNode* node, xmlDocPtr doc)
{
	_node = node;
	_currentChildNode = NULL;
	_currentXmlChildNode = NULL;
	_doc = doc;
}

Xml::Node::~Node()
{
	while (!this->_toDelete.empty())
	{
		delete this->_toDelete.top();
		this->_toDelete.pop();
	}
}
#include <iostream>
std::string		Xml::Node::getValue() const
{
	assert(this->isElement());
	return (const char*)xmlNodeListGetString(this->_doc, this->_node->children, 0);
}

std::string		Xml::Node::getName() const
{
	return (const char*)this->_node->name;
}

std::string		Xml::Node::getAttr(const char* name) const
{
	xmlChar*	value = xmlGetProp(this->_node, (const xmlChar*)name);
	if (value == NULL)
		return ("");
	std::string retValue((char*)value);
	xmlFree(value);
	return retValue;
}

bool			Xml::Node::hasAttr(const char* name) const
{
	return (xmlHasProp(this->_node, (xmlChar*)name) != NULL);
}

int				Xml::Node::getType() const
{
	return this->_node->type;
}

bool			Xml::Node::isElement() const
{
	return (this->_node->type == XML_ELEMENT_NODE);
}

bool			Xml::Node::isChildNodeElement() const
{
	return (this->_currentXmlChildNode->type == XML_ELEMENT_NODE);
}

void			Xml::Node::initChildNodes()
{
	this->_currentXmlChildNode = this->_node->children;
	this->_currentChildNode = new Xml::Node(this->_currentXmlChildNode, this->_doc);
	this->_toDelete.push(this->_currentChildNode);
}

bool			Xml::Node::goToNextChildNode()
{
	if (this->_currentXmlChildNode == NULL || this->_currentChildNode == NULL)
	{
		this->initChildNodes();
		if (!this->isChildNodeElement())
			return (this->goToNextChildNode());
		return true;
	}
	if (this->_currentXmlChildNode->next)
	{
		this->_currentXmlChildNode = this->_currentXmlChildNode->next;
		if (!this->isChildNodeElement())
			return (this->goToNextChildNode());	
		this->_currentChildNode = new Xml::Node(this->_currentXmlChildNode, this->_doc);
		this->_toDelete.push(this->_currentChildNode);
		return true;
	}
	return false;
}

Xml::Node*		Xml::Node::getNextChildNode()
{
	if (this->goToNextChildNode())
	{
		return this->getCurrentChildNode();
	}
	return NULL;
}

Xml::Node*		Xml::Node::getCurrentChildNode()
{
	if (this->_currentXmlChildNode == NULL || this->_currentChildNode == NULL)
	{
		this->initChildNodes();
		return this->_currentChildNode;
	}
	return this->_currentChildNode;
}

