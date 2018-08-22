#if !defined ( __ResultSet_h )
#define __ResultSet_h 1


#include <string>
#include <vector>


struct IF2UnPacker;

//
// class CResultSet
//
class CResultSet
{
public :
    CResultSet();
    virtual ~CResultSet();

    typedef std::vector<std::string> TRowType;
    typedef std::vector<TRowType> TNameValueType;
    typedef TNameValueType::const_iterator const_iterator;
    typedef TNameValueType::iterator iterator;
public:
    bool addField(const std::string & fieldName);

    bool addValue(const std::string & value);

    bool getValueByName(const std::string fieldName, std::string& value) const;
    bool getValue(std::string& value) const;

    bool getField(std::string& value) const;

    bool hasField(std::string value) const;

    //结果集的行数
    //包括列名
    int  getRowCount() const;

    //结果集的列数
    int  getColCount() const;

    bool eof() const;

    iterator last();
    iterator first();
    iterator next();
    iterator prev();

    bool unpack(IF2UnPacker *unpacker);

    int getHandlerID();
    void setHandlerID(int id);

private:
    CResultSet(const CResultSet&);
    CResultSet& operator=(const CResultSet&);

private:

    int    _handerID;

    TNameValueType                      _fieldNameAndValue;
    mutable TRowType::iterator          _currentColOfData;
    mutable TRowType::iterator          _currentColOfName;
    mutable TNameValueType::iterator    _currentRow;
};

inline int CResultSet::getRowCount() const
{
    return _fieldNameAndValue.size();
}

inline int CResultSet::getColCount() const
{
    if(_fieldNameAndValue.empty())
        return 0;
    return _fieldNameAndValue.front().size();
}

inline bool CResultSet::eof() const
{
    return _currentRow == _fieldNameAndValue.end();
}

inline CResultSet::TNameValueType::iterator CResultSet::last()
{
    _currentRow = _fieldNameAndValue.end();
    CResultSet::TNameValueType::iterator iter = _fieldNameAndValue.begin();
    if(iter != _fieldNameAndValue.end())
        _currentColOfName = iter->begin();
    return _currentRow;
}

inline CResultSet::TNameValueType::iterator CResultSet::first()
{
    _currentRow = _fieldNameAndValue.begin();
    if(!eof())
        _currentColOfName = _currentRow->begin();
    next();//skip first line, name
    if(!eof())
        _currentColOfData = _currentRow->begin();
    return _currentRow;
}

inline CResultSet::TNameValueType::iterator CResultSet::next()
{
    if(!eof())
        ++_currentRow;
    if(!eof())
        _currentColOfData = _currentRow->begin();
    return _currentRow;
}

inline CResultSet::TNameValueType::iterator CResultSet::prev()
{
    if(_currentRow != _fieldNameAndValue.begin())
        --_currentRow;
    if(!eof())
        _currentColOfData = _currentRow->begin();
    return _currentRow;
}

inline int CResultSet::getHandlerID()
{
    return _handerID;
}

inline void CResultSet::setHandlerID(int id)
{
    _handerID = id;
}


#endif        // __ResultSet_h
