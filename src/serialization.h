#pragma once

#include <filesystem>
#include <fstream>

#include <pal/typedefs.h>

namespace idrs
{
    class Serialization
    {
    public:
        enum class IO
        {
            WriteBinary,
            ReadBinary,
            ReadText
        };

    public:
        Serialization(const std::filesystem::path &path, IO type);
        ~Serialization();

        void writeBinary(void *in, u32 len, u32 tsize);
        void readBinary(void *out, u32 len, u32 tsize);

        // void writeText(const std::string &key);

        bool readBool(const std::string &key);
        s64 readInt(const std::string &key);
        double readFlt(const std::string &key);
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