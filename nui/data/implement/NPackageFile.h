#pragma once


class NPackageFile : protected nui::Base::Noncopyable
{
    typedef std::map<nui::Base::NString, nui::Base::NString> PathMap;
public:
    NPackageFile();
    ~NPackageFile();

public:
    void Init(LPCTSTR filePath, bool useRealFS);
    bool LoadConfig(nui::Base::NInstPtr<nui::Data::NBuffer> fileBuffer);
    nui::Base::NString GetRealPath(LPCTSTR xncPath);

private:
    nui::Base::NString GetXNCPrefix(const nui::Base::NString& path);
    bool LoadConfigForNode(nui::Base::NAutoPtr<nui::Data::NDataReader> reader, LPCTSTR xmlPath, LPCTSTR nodeName, LPCTSTR keyName, PathMap& pathMap);

protected:
    bool useRealFS_;

    PathMap mapPaths_;
    PathMap mapStrings_;
    nui::Base::NString rootPath_;
};
