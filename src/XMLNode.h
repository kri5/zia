#ifndef XMLNODE_H__
# define XMLNODE_H__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stack>
#include <string>
#include <assert.h>

namespace Xml
{
	class Document;

	class Node
	{
		public:
			Node(xmlNode* node, xmlDocPtr doc);
			~Node();
			/// Will return the node value. (IE for <loutre>test</loutre> this will return test)
			std::string			getValue() const;
			/// Will return the node name. (IE for <loutre>test</loutre>, this will return loutre)
			std::string			getName() const;
			/// Will return true if node has a text content. Otherwise, we should browse child nodes.
			bool				isElement() const;
			int					getType() const;
			bool				goToNextChildNode();
			/// Will move to the next child node and return it.
			Node*				getNextChildNode();
			Node*				getCurrentChildNode();
			std::string			getAttr(const char*) const;
			bool				hasAttr(const char*) const;
		private:
			void				initChildNodes();
			bool				isChildNodeElement() const;
			xmlNode*			_node;
			xmlNode*			_currentXmlChildNode;
			Node*				_currentChildNode;
			std::stack<Node*>	_toDelete;
			xmlDocPtr			_doc;
			//here should go the libxml pointer.
	};
}

#endif // XMLNODE_H__

