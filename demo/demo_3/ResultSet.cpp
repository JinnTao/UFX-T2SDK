#include <algorithm>
#include <sstream>
#include "t2sdk_interface.h"
#include "ResultSet.h"


using namespace std;

CResultSet::CResultSet ()
:_currentRow(_fieldNameAndValue.end())
,_handerID(0)
{
}


CResultSet::~CResultSet ()
{
}

bool CResultSet::addField(const std::string & fieldName)
{
    if(_fieldNameAndValue.empty())//first
        _fieldNameAndValue.push_back(TRowType());
    _fieldNameAndValue.front().push_back(fieldName);
    return true;
}

bool CResultSet::addValue(const std::string & value)
{

    if(getRowCount() == 1){//first add value, only has header
        _fieldNameAndValue.push_back(TRowType());
    }
    int colCount = getColCount();
    TRowType* curRow = &_fieldNameAndValue.back();
    if(curRow->size() == colCount){//new row
        _fieldNameAndValue.push_back(TRowType());
        curRow = &_fieldNameAndValue.back();
    }
    curRow->push_back(value);
    return true;
}

bool CResultSet::getValueByName(const std::string fieldName, std::string& value) const
{
    const TRowType& nameVec = _fieldNameAndValue.front();
    TRowType::const_iterator iter = find(nameVec.begin(), nameVec.end(), fieldName);
    if(iter == nameVec.end())
        return false;
    int index = distance(nameVec.begin(), iter);
    if(eof()) return false;
    value = (*_currentRow)[index];
    return true;
}

bool CResultSet::getValue(std::string& value) const
{
    if(_currentColOfData == _currentRow->end())
        return false;
    value = *_currentColOfData;
    ++_currentColOfData;
    return true;
}

bool CResultSet::unpack(IF2UnPacker *unpacker)
{
    //先取第二个结果集，如果失败则取第一个结果集
    if(unpacker->SetCurrentDatasetByIndex(1) == 0){
        unpacker->SetCurrentDatasetByIndex(0);
    }

    unsigned int rowCount = 0;
    int colCount = 0;
    for(colCount = 0; colCount < unpacker->GetColCount(); colCount++){
        string fieldName = unpacker->GetColName(colCount);
        addField(fieldName);
    }

    for(rowCount = 0; rowCount < unpacker->GetRowCount(); rowCount++){
        for(colCount = 0; colCount < unpacker->GetColCount(); colCount++){
            int type = unpacker->GetColType(colCount);
            stringstream value;
            switch(type){
                case 'I':
                    value << unpacker->GetIntByIndex(colCount);
                    addValue(value.str());
                    break;
                case 'C':
                    value << unpacker->GetCharByIndex(colCount);
                    addValue(value.str());
                    break;
                case 'S':
                    value << unpacker->GetStrByIndex(colCount);
                    addValue(value.str());
                    break;
                case 'F':
                    value << unpacker->GetDoubleByIndex(colCount);
                    addValue(value.str());
                    break;
                case 'R':
                    addValue("RawData");
                    break;
            }
        }
        unpacker->Next();
    }
    first(); //set up iterator
    return true;
}

bool CResultSet::getField(std::string& value) const
{
    CResultSet::TNameValueType::const_iterator iter = _fieldNameAndValue.begin();
    if(_currentColOfName == iter->end())
        return false;
    value = *_currentColOfName;
    ++_currentColOfName;
    return true;
}

bool CResultSet::hasField(std::string value) const
{
    if(_fieldNameAndValue.empty())
        return false;
    const TRowType& row = _fieldNameAndValue.front();
    return find(row.begin(), row.end(), value) != row.end();
}
