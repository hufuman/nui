#include "StdAfx.h"
#include "Employees.h"


Employees::Employees(void)
{
}

Employees::~Employees(void)
{
}

Employees& Employees::Get()
{
    static Employees instance;
    return instance;
}

void Employees::Load()
{
    loading_ = true;
    NInstPtr<NFileSystem> fs(MemToolParam);
    NInstPtr<NBuffer> buffer(MemToolParam);
    fs->LoadFile(_T("@data:employees.txt"), buffer);
    NString result = utf82t(static_cast<LPCSTR>(buffer->GetBuffer())).Replace(_T("\r"), _T(""));

    NAssertTempDisable();

    int lineTokenPos = 0;
    NString line;
    NString seq, name;
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();

    // first line is column
    result.Tokenize(lineTokenPos, _T("\n"), false, line);

    while(result.Tokenize(lineTokenPos, _T("\n"), false, line))
    {
        // seq name
        int pos = 0;
        if(!line.Tokenize(pos, _T("\t"), false, seq)
            || !line.Tokenize(pos, _T("\t"), false, name))
        {
            break;
        }
        EmployeeInfo info;
        NString imagePath(_T("@images:employees\\"));
        imagePath += name;
        info.draw = loader->LoadImage(imagePath + _T(".png"));
        if(info.draw == NULL)
            info.draw = loader->LoadImage(imagePath + _T(".jpg"));
        if(info.draw == NULL)
            info.draw = loader->LoadImage(imagePath + _T(".jpeg"));
        if(info.draw == NULL)
        {
            NString msg(_T("Failed to load image for "));
            msg += name;
            MessageBox(NULL, msg, _T("Error"), MB_OK | MB_ICONERROR);
        }
        else
        {
            info.draw->AddRef();
        }
        if(name.GetLength() == 2)
        {
            info.name = name[0];
            info.name += _T("  ");
            info.name += name[1];
        }
        else
        {
            info.name = name;
        }
        info.seq = seq;
        info.bonusIndex = -1;
        employees_.AddItem(info);
    }

    LoadBonus();
    loading_ = false;
}

void Employees::LoadBonus()
{
    loading_ = true;
    bonusResultMap_.clear();

    NInstPtr<NBuffer> buffer(MemToolParam);
    // load file
    NString filePath = File::CombinePath(NModule::GetInst().GetAppPath(), _T("bonus_result.txt"));
    HANDLE hFile = ::CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
    ::SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    DWORD dwRead;
    ::ReadFile(hFile, buffer->GetBuffer(dwFileSize), dwFileSize, &dwRead, NULL);
    ::CloseHandle(hFile);

    // 
    int lineTokenPos = 0;
    NString line;
    NString bonusIndexes;
    bonusIndexes.Assign((LPCTSTR)buffer->GetBuffer(), dwFileSize / sizeof(TCHAR));
    bonusIndexes.Replace(_T("\r"), _T(""));
    NString employeeIndex, bonusIndex;
    lineTokenPos = 0;
    int employeeCount = employees_.Count();
    while(bonusIndexes.Tokenize(lineTokenPos, _T("\n"), false, line))
    {
        int pos = 0;
        if(!line.Tokenize(pos, _T("\t"), false, employeeIndex)
            || !line.Tokenize(pos, _T("\t"), false, bonusIndex))
        {
            break;
        }
        int eIndex = _ttoi(employeeIndex);
        int bIndex = _ttoi(bonusIndex);
        MarkEmployeeBonus(eIndex, bIndex);
    }
    loading_ = false;
}

void Employees::Destroy()
{
    int count = employees_.Count();
    for(int i=0; i<count; ++ i)
    {
        const EmployeeInfo& info = employees_[i];
        if(info.draw != NULL)
            info.draw->Release();
    }
}

void Employees::Reset()
{
    int count = employees_.Count();
    for(int i=0; i<count; ++ i)
    {
        employees_[i].bonusIndex = -1;
    }
    bonusResultMap_.clear();
    NString path;
    NString filePath = File::CombinePath(NModule::GetInst().GetAppPath(), _T("bonus_result.txt"));
    for(int i=0; i<100; ++ i)
    {
        path = filePath;
        path.Replace(_T(".txt"), _T("_%1.txt"));
        path = path.arg(i);
        if(File::IsFileExists(path))
            continue;
        ::MoveFile(filePath, path);
    }
    LoadBonus();
}

int Employees::Count() const
{
    return employees_.Count();
}

const EmployeeInfo& Employees::GetEmployeeInfo(int index) const
{
    return employees_[index];
}

int Employees::GetRandomNextIndex() const
{
    while(true)
    {
        int index = rand() % employees_.Count();
        const EmployeeInfo& result = employees_[index];
        if(result.bonusIndex < 0)
            return index;
    }
}

const std::list<int>* Employees::GetBonusEmployees(int bonusIndex) const
{
    BonusResultMap::const_iterator ite = bonusResultMap_.find(bonusIndex);
    if(ite == bonusResultMap_.end())
        return NULL;
    const std::list<int>& list = ite->second;
    return &list;
}

void Employees::MarkEmployeeBonus(int employeeIndex, int bonusIndex)
{
    employees_[employeeIndex].bonusIndex = bonusIndex;
    BonusResultMap::iterator ite = bonusResultMap_.find(bonusIndex);
    if(ite == bonusResultMap_.end())
    {
        std::list<int> list;
        ite = bonusResultMap_.insert(std::make_pair(bonusIndex, list)).first;
    }
    std::list<int>& list = ite->second;

    if(std::find(list.begin(), list.end(), employeeIndex) == list.end())
        list.push_back(employeeIndex);

    if(loading_)
        return;

    NString line(_T("%1\t%2\r\n"));
    line = line.arg(employeeIndex).arg(bonusIndex);

    // save into file
    NString filePath = File::CombinePath(NModule::GetInst().GetAppPath(), _T("bonus_result.txt"));
    HANDLE hFile = ::CreateFile(filePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
    ::SetFilePointer(hFile, 0, 0, FILE_END);
    DWORD dwWritten = 0;
    ::WriteFile(hFile, (LPCVOID)(line.GetData()), line.GetLength() * sizeof(TCHAR), &dwWritten, 0);

    ::FlushFileBuffers(hFile);
    ::CloseHandle(hFile);
}

