#ifndef PARSER_H__
# define PARSER_H__

#include <map>
#include <sstream>
#include <iostream>
#include "Utils/Buffer.h"
#include "MemoryManager.hpp"

class	Parser
{
	public:

		Parser(Buffer* = NULL);
		virtual				~Parser();
        virtual void        init();
		char				peekChar(); //will consume stream
        std::string         peekNChar(int);
		char				readChar();
        bool                readIfEqual(const std::string&);
        bool                peekIfEqual(char, std::string&);
        bool                peekIfEqual(char);
        bool                peekIfEqual(const std::string&, std::string&);
        bool                peekIfEqual(const std::string&);
        bool                peekIfNum(std::string&);

		bool				readAnythingBut(const std::string& forbiden, std::string& res);
		bool				readAnythingBut(const std::string& forbiden);

		void                readUpToIgnore();
		void				readUpToIgnore(std::string& output);

		void				readUntilEndOfLine();
		void				readUntilEndOfLine(std::string& output);

		bool				readIdentifier(std::string& output);
        bool                appendIdentifier(std::string&);

		bool				readInteger(int& output);
		bool				readInteger(short& output);
		bool				readInteger(std::string& output);
		bool				appendInteger(std::string& output);
		
		bool				readDecimal(double& output);
		bool				readDecimal(float& output);
		bool				readDecimal(std::string& output);
		bool				appendDecimal(std::string& output);

		//bool				isEOF() const;
		//bool				isError() const;
        bool                hasEOL();
        bool                isEOL();
        bool                isEnd() const;

        bool                isAlpha();
        bool                isAlpha(const char) const;
        bool                isNum();
        bool                isNum(const char) const;
        bool                isAlphaNum();
        bool                isAlphaNum(const char) const;
		void				flush();
        void                setIgnore(bool);
        bool                getIgnore() const;
        char                getLastReadChar() const;
        void                setComment(bool);
        void                setCommentList(const char* list);
        bool                getComment() const;
        void                saveContextPub();
        void                restoreContextPub();
		void				feed(const char*, size_t);
        bool                isFed() const;
        void                dump() const;

	protected:
		bool						isIgnore(char c) const;
        //FIXME: put me back in private after debug :)
		Buffer*                     _buffer;

	private:
		//bool						extendBuffer();
		void						__saveContext();
		void						__restoreContext();
		void						trim();
		void						ignore();
        void                        skipComment(char c);

		//attributes
		std::stringstream           _stream;
		unsigned int	    		_i;
		int							_backI;
        int                         _backIPub;
        bool                        _ignore;
        bool                        _comment;
        std::string                 _commentList;
        bool                        _end;
        char                        _lastReadChar;
        bool                        _isFed;
        bool                        _internalBuffer;
};


#endif //PARSER_H__
