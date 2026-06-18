#include "editorUtils.hpp"

#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#endif

// this is some of the most ridiculous looking code in the entire code base, I don't think I like WinAPI
std::string saveFileDialog() {
#ifdef _WIN32
  OPENFILENAMEA ofn;
  char szFile[260] = {0};

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = "json";

  if (GetSaveFileNameA(&ofn)) {
    return std::string(szFile);
  }
#endif
  return "";
}

std::string openFileDialog() {
  #ifdef _WIN32
    OPENFILENAMEA ofn;
    char szFile[260] = {0};
        
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        
    if (GetOpenFileNameA(&ofn) == TRUE) {
      return std::string(szFile);
    }
  #endif
    return "";
}

bool endsWith(const std::string& s, const std::string& suffix) {
  return s.size() >= suffix.size() &&
         s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}
