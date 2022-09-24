template<typename T>
T Socket::ReadType() requires ( std::is_fundamental_v<T> )
{
    T data;
    Read(&data, sizeof(T));
    return data;
}



template<typename T>
void Socket::ReadArray(T* data, size_t count) requires ( std::is_fundamental_v<T> )
{
    Read(reinterpret_cast<void*>(data), count * sizeof(T));
}



template<typename T>
std::vector<T> Socket::ReadVector(size_t count) requires ( std::is_fundamental_v<T> )
{
    std::vector<T> data;
    data.resize(count);
    Read(data.data(), count);
    return data;
}



template<typename T>
void Socket::WriteType(const T& data) requires ( std::is_fundamental_v<T> )
{
    Write(reinterpret_cast<void*>(&data), sizeof(T));
}



template<typename T>
void Socket::WriteArray(const T* data, size_t count) requires ( std::is_fundamental_v<T> )
{
    Write(reinterpret_cast<const void*>(data), count * sizeof(T));
}



template<typename T>
void Socket::WriteVector(const std::vector<T>& data) requires ( std::is_fundamental_v<T> )
{
    Write(reinterpret_cast<void*>(data), data.size() * sizeof(T));
}