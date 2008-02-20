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
		char				readChar(); //will juste return next char
		bool				readIdentifier(std::string& output);
		bool				readInteger(int& output);
		bool				readInteger(short& output);
		bool				readInteger(std::string& output);
		bool				readDecimal(double& output);
		bool				readDecimal(float& output);
        bool                readEmailAddress(std::string&);
		bool				isEOF() const;
		bool				isError() const;
		void				flush();
        void                saveContextPub();
        void                restoreContextPub();
        void                feed(const char* data)
        {
            this->_stream.write(data, strlen(data));
            this->extendBuffer();
        }
	private:
		bool											extendBuffer();
		void											saveContext();
		void											restoreContext();
		void											trim();
		void											ignore();

		//attributes
		std::stringstream                               _stream;
		std::vector<std::string>						_buffers;
		int												_i;
		int												_bufferId;
		int												_backI;
        int                                             _backIPub;
		int												_backBuffer;
        int                                             _backBufferPub;
		int												_nbRead;
};


#endif //PARSER_H__
