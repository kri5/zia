#include "Modules/DynLib.h"
#include <dlfcn.h>

DynLib::DynLib() : _err(NULL) {}

DynLib::~DynLib() {
  if (this->_err != NULL)
    delete this->_err;
  close();
}

bool DynLib::load(const std::string &filename) {
  if ((this->handle = dlopen(filename.c_str(), RTLD_LAZY)) == 0)
    return false;
  return true;
}

void *DynLib::sym(const std::string &symbol) {
  void *ret;
  char *err;

  dlerror();
  if (this->_err != NULL)
    delete this->_err;
  ret = dlsym(this->handle, symbol.c_str());
  err = dlerror();
  if (err != NULL)
    this->_err = new std::string(err);
  return ret;
}

void DynLib::close() {
  dlclose(this->handle);
}

const char *DynLib::lastError() {
  char *err;

  err = dlerror();
  if (err != NULL) {
    if (this->_err != NULL)
      delete this->_err;
    this->_err = new std::string(err);
  }
  if (this->_err != NULL)
    return this->_err->c_str();
  return NULL;
}
