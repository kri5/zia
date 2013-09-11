#include <cstring>

#include "Modules/BlackList.h"
#include "File/IFile.h"
#include "File/File.h"

BlackList::BlackList(const std::string &path) { this->_path = path; }

BlackList::BlackList(std::string &&path) { this->_path = path; }

bool BlackList::isBlackListed(const std::string &filename) const {
  return (this->_blackList.find(filename) != this->_blackList.end());
}

void BlackList::blackList(const std::string &filename) {
  IFile *file = new File(filename, this->_path);
  this->_blackList[filename] =
      BlackList::Item(file->getModifDate()->getTimestamp(), file->getSize());
  delete file;
}

bool BlackList::hasChanged(const std::string &filename) const {
  std::map<std::string, BlackList::Item>::const_iterator it =
      this->_blackList.find(filename);
  if (it == this->_blackList.end())
    return false;
  IFile *file = new File(filename, this->_path);
  bool res = (it->second.modifTime != file->getModifDate()->getTimestamp() ||
              it->second.size != file->getSize());
  delete file;
  return res;
}

