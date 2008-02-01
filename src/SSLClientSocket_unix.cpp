#include "SSLClientSocket_unix.h"
#include <openssl/ssl.h>

SSLClientSocket::SSLClientSocket(int acceptedSocket) : ClientSocket(acceptedSocket)
{
  int SSL_RESULT;

  /* This must maybe be defined near the main ? */
  SSL_library_init(); // load encryption & hash algorithms for SSL
  SSL_load_error_strings(); // load the error strings for good error reporting

  // Initialising the SSL Method
  ctx = SSL_CTX_new(SSLv23_server_method());

  // Load the certificate and the private key
  SSL_CTX_use_certificate_file(ctx, "../ssl/public.crt", SSL_FILETYPE_PEM);
  SSL_CTX_use_PrivateKey_file(ctx, "../ssl/private.key", SSL_FILETYPE_PEM);

  // Creating SSL Structure
  ssl = SSL_new(ctx);
  SSL_set_fd(ssl, acceptedSocket);

  // Handshake (is blocking!!)
  SSL_RESULT = SSL_accept(ssl);
  if (SSL_RESULT <= 0)
  {
    this->logError();
    this->close(true);
    throw 0;
  }
}

SSLClientSocket::~SSLClientSocket()
{
  this->close(true);
  SSL_free(ssl);
  SSL_CTX_free(ctx);
}

int     SSLClientSocket::send(char *buf, int length) const
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
	Logger::getInstance()->Log(Logger::DEBUG, "SSL_write: renegociating session.");
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
	Logger::getInstance()->Log(Logger::DEBUG, "SSL_read: renegociating session.");
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

void    SSLClientSocket::close(bool shutdown) const
{
  SSL_shutdown(ssl);
  ClientSocket::close(shutdown);
}

void	SSLClientSocket::logError() const
{
  unsigned long e;
  static char *buf;

  e = ERR_get_error();
  buf = ERR_error_string(e, NULL);
  Logger::getInstance()->log(Logger::ERROR, "SSL error: " + buf);
}


