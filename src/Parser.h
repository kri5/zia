#ifndef PARSER_H__
# define PARSER_H__

#include <map>
#include <sstream>
#include <vector>
#include <iostream>

class	Parser
{
	public:

		Parser();
		virtual				~Parser(){};
		char				peekChar(); //will consume stream
        std::string         peekNChar(int);
		char				readChar(); 
        bool                peekIfEqual(char, std::string&);
        bool                peekIfEqual(char);
        bool                peekIfEqual(const std::string&, std::string&);
        bool                peekIfEqual(const std::string&);

		bool				readIdentifier(std::string& output);
        bool                appendIdentifier(std::string&);
		bool				readInteger(int& output);
		bool				readInteger(short& output);
		bool				readInteger(std::string& output);
		bool				readDecimal(double& output);
		bool				readDecimal(float& output);
        bool                readEmailAddress(std::string&);
		//bool				isEOF() const;
		//bool				isError() const;

        bool                isAlpha();
        bool                isAlpha(const char) const;
        bool                isNum();
        bool                isNum(const char) const;
        bool                isAlphaNum();
        bool                isAlphaNum(const char) const;
		void				flush();
        void                setIgnore(bool);
        bool                getIgnore() const;
        void                setComment(bool);
        void                setCommentList(const char* list);
        bool                getComment() const;
        void                saveContextPub();
        void                restoreContextPub();
        void                feed(const char* data)
        {
            this->_stream.write(data, strlen(data));
            this->extendBuffer();
        }

        void                        printI() const;


	private:
		bool						extendBuffer();
		void						__saveContext();
		void						__restoreContext();
		void						trim();
		void						ignore();
        void                        readUpToIgnore();
        void                        skipComment(char c);

		//attributes
		std::stringstream           _stream;
		std::vector<std::string>	_buffers;
		int							_i;
		int							_bufferId;
		int							_backI;
        int                         _backIPub;
		int							_backBuffer;
        int                         _backBufferPub;
		int							_nbRead;
        bool                        _ignore;
        bool                        _comment;
        std::string                 _commentList;
};


#endif //PARSER_H__
