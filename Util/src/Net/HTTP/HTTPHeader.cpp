#include "Util/Net/HTTP/HTTPHeader.hpp"


#include "Util/Assert.hpp"



#include "Util/Utilities.hpp"



void HTTPHeader::Add(const HTTPHeader::Key& key, const HTTPHeader::Value& value)
{
    auto lc = ToLowercase(key);
    if (mEntries.contains(lc))
    {
        mEntries.at(lc).push_back(value);
    }
    else
    {
        mEntries.insert({lc, {value}});
    }
}



void HTTPHeader::Set(const HTTPHeader::Key& key, const HTTPHeader::Value& value)
{
    auto lc = ToLowercase(key);
    mEntries.insert_or_assign(lc, std::vector<Value>{value});
}



HTTPHeader::Value HTTPHeader::Get(const HTTPHeader::Key& key) const
{
    auto lc = ToLowercase(key);
    Assert(mEntries.contains(lc));
    return mEntries.at(lc)[0];
}



std::vector<HTTPHeader::Value> HTTPHeader::GetAll(const HTTPHeader::Key& key) const
{
    auto lc = ToLowercase(key);
    Assert(mEntries.contains(lc));
    return mEntries.at(lc);
}



bool HTTPHeader::Contains(const HTTPHeader::Key& key) const
{
    auto lc = ToLowercase(key);
    return mEntries.contains(lc);
}
