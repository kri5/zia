#include "SSLClientSocket.h"

/// Initializing SSL socket and doing handshake with the client.
SSLClientSocket::SSLClientSocket(int acceptedSocket)
{
  int iResult;
  bool no_error = true;
  listenSocket = acceptedSocket;

  // Initializing the SSL Method
  ctx = SSL_CTX_new(SSLv23_server_method());
  if (!ctx)
  {
    this->logError();
    return;
  }

  //FIXME we must get the certificates path from the configuration!
  // Load the certificate and the private key
  if (SSL_CTX_use_certificate_file(ctx, "ssl/server.crt", SSL_FILETYPE_PEM) < 1)
  {
    this->logError();
    return;
  }
  if (SSL_CTX_use_PrivateKey_file(ctx, "ssl/server.key.unsecure", SSL_FILETYPE_PEM) < 1)
  {
    this->logError();
    return;
  }

  // Creating SSL Structure
  ssl = SSL_new(ctx);
  if (!ssl)
  {
    this->logError();
    return;
  }


  if (SSL_set_fd(ssl, acceptedSocket) < 1)
  {
    this->logError();
    return;
  }

  // Handshake (is blocking if underlying socket is blocking!!)
  do
  {
    iResult = SSL_accept(ssl);
    switch(SSL_get_error(ssl, iResult))
    {
      case SSL_ERROR_NONE:
      case SSL_ERROR_ZERO_RETURN:
      case SSL_ERROR_WANT_READ:
	break;
      case SSL_ERROR_WANT_WRITE:
	break;
      case SSL_ERROR_WANT_CONNECT:
      case SSL_ERROR_WANT_ACCEPT:
      case SSL_ERROR_WANT_X509_LOOKUP:
      case SSL_ERROR_SYSCALL:
      case SSL_ERROR_SSL:
      default:
	no_error = false;
	this->logError();
	break;
    }
  }
  while (iResult < 0 && no_error);
}

SSLClientSocket::~SSLClientSocket()
{
  this->close(true);
  SSL_free(ssl);
  SSL_CTX_free(ctx);
}

int     SSLClientSocket::send(const std::string& buf, int length) const
{
    send(buf.c_str(), buf.size());
}

int     SSLClientSocket::send(const char *buf, int length) const
{
  bool no_error = true;
  int iResult;

  do
  {
    iResult = SSL_write(ssl, buf, length);
    switch(SSL_get_error(ssl, iResult))
    {
      case SSL_ERROR_NONE:
      case SSL_ERROR_ZERO_RETURN:
      case SSL_ERROR_WANT_READ:
      case SSL_ERROR_WANT_WRITE:
	//Logger::getInstance() << Logger::Debug << "SSL_write: renegociating session." << Logger::Flush;
	break;
      case SSL_ERROR_WANT_CONNECT:
      case SSL_ERROR_WANT_ACCEPT:
      case SSL_ERROR_WANT_X509_LOOKUP:
      case SSL_ERROR_SYSCALL:
      case SSL_ERROR_SSL:
      default:
	no_error = false;
	this->logError();
	break;
    }
  }
  while (iResult < 0 && no_error);

  return iResult;
}

int     SSLClientSocket::recv(char *buf, int length) const
{
  bool no_error = true;
  int iResult;

  do
  {
    iResult = SSL_read(ssl, buf, length);
    switch(SSL_get_error(ssl, iResult))
    {
      case SSL_ERROR_NONE:
      case SSL_ERROR_ZERO_RETURN:
      case SSL_ERROR_WANT_READ:
	//Logger::getInstance() << Logger::Debug << "SSL_read: renegociating session." << Logger::Flush;
	break;
      case SSL_ERROR_WANT_WRITE:
      case SSL_ERROR_WANT_CONNECT:
      case SSL_ERROR_WANT_ACCEPT:
      case SSL_ERROR_WANT_X509_LOOKUP:
      case SSL_ERROR_SYSCALL:
      case SSL_ERROR_SSL:
      default:
	no_error = false;
	this->logError();
	break;
    }
  }
  while (iResult < 0 && no_error);

  return iResult;
}

//FIXME This method doesn't exist anymore in the IClientSocket interface!
void    SSLClientSocket::close(bool shutdown)
{
    if (this->_closed == false)
    {
        SSL_shutdown(ssl);
        this->_closed = true;
        //IClientSocket::close(shutdown);
    }
}

bool    SSLClientSocket::isClosed() const
{
    return this->_closed;
}

int     SSLClientSocket::getNativeSocket() const
{
    return listenSocket;
}

/// Get and send errors (as strings) to the logger.
void	SSLClientSocket::logError() const
{
  /*unsigned long e;
  static char *buf;

  e = ERR_get_error();
  buf = ERR_error_string(e, NULL);
  Logger::getInstance() << Logger::Error << "SSL " << std::string(buf) << Logger::Flush;*/
}

