#include "Util/Net/HTTP/HTTPHeader.hpp"



#include <cassert>



void HTTPHeader::Add(const HTTPHeader::Key& key, const HTTPHeader::Value& value)
{
    if (mEntries.contains(key))
    {
        mEntries.at(key).push_back(value);
    }
    else
    {
        mEntries.insert({key, {value}});
    }
}



void HTTPHeader::Set(const HTTPHeader::Key& key, const HTTPHeader::Value& value)
{
    mEntries.insert_or_assign(key, std::vector<Value>{value});
}



HTTPHeader::Value HTTPHeader::Get(const HTTPHeader::Key& key) const
{
    assert(mEntries.contains(key));
    return mEntries.at(key)[0];
}



std::vector<HTTPHeader::Value> HTTPHeader::GetAll(const HTTPHeader::Key& key) const
{
    assert(mEntries.contains(key));
    return mEntries.at(key);
}



bool HTTPHeader::Contains(const HTTPHeader::Key& key) const
{
    return mEntries.contains(key);
}
