#pragma once

#include <filesystem>
#include <fstream>

#include <idcore/typedefs.h>

namespace idrs
{
    class Serialization
    {
    public:
        enum class IO
        {
            WriteBinary,
            ReadBinary,
            WriteText,
            ReadText
        };

    public:
        Serialization(const std::filesystem::path &path, IO type);
        ~Serialization();

        void writeBinary(void *in, u32 len, u32 tsize);
        void readBinary(void *out, u32 len, u32 tsize);

        template<typename T>
        void writeSingle(const std::string &key, T value);
        void writeString(const std::string &key, const std::string &value);

        template<typename T>
        void writeArray(const std::string &key, T* ptr, u32 len);
        void writeStringArray(const std::string &key, std::string *ptr, u32 len);

        bool readBool(const std::string &key);
        s64 readInt(const std::string &key);
        f32 readFlt(const std::string &key);
        std::string readString(const std::string &key);

        void readArrayString(const std::string &key, void *ptr);
        template<typename T>
        void readArrayInt(const std::string &key, void* ptr);
        template <typename T>
        void readArrayFlt(const std::string &key, void *ptr);

    private:
        std::fstream m_stream;
    };
}
