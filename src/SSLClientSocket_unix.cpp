#include "SSLClientSocket_unix.h"
#include <openssl/ssl.h>

//
// Object construction:
// 3 steps.
//
// 1_ Memory allocation
// 2_ Init ??? -> Default constructor
// 3_ Constructor code execution
//
// Merci jack :)

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
        unsigned long e;
        static char *buf;

        e = ERR_get_error();
        buf = ERR_error_string(e, NULL);
        // printf("SSL_accept error: %s", buf);
        this->close(true);
        throw 0;
    }
}

int     SSLClientSocket::send(char *buf, int length) const
{
    /*
     * WARNING
     * When an SSL_read() operation has to be repeated because of
     * SSL_ERROR_WANT_READ or SSL_ERROR_WANT_WRITE, it must be repeated with
     * the same arguments.
     */
    int iResult = SSL_write(ssl, buf, length);
    if (iResult <= 0)
    {
        // call SSL_get_error()
        throw 0;
    }
    return iResult;
}

int     SSLClientSocket::recv(char *buf, int length) const
{
    /*
     * WARNING
     * When an SSL_write() operation has to be repeated because of
     * SSL_ERROR_WANT_READ or SSL_ERROR_WANT_WRITE, it must be repeated with
     * the same arguments.
     */
    int iResult = SSL_read(ssl, buf, length);
    if (iResult <= 0)
    {
        // call SSL_get_error()
        throw 0;
    }
    return iResult;
}
 
void    SSLClientSocket::close(bool shutdown) const
{
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ::close(listenSocket);
}
