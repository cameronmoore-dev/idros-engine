#include "serialization.h"

#include <tomlc17/tomlc17.h>

namespace idrs
{
    static toml_result_t m_toml;

    Serialization::Serialization(const std::filesystem::path &path, IO type)
    {
        if (type == IO::ReadText)
        {
            m_toml = toml_parse_file_ex(path.string().c_str());
            if (!m_toml.ok)
            {
                printf("Toml failed to parse: %s\n", m_toml.errmsg);
                toml_free(m_toml);
            }
            return;
        }

        m_stream = (type == IO::WriteBinary) ? 
            std::fstream(path, std::ios::out | std::ios::binary) : 
            std::fstream(path, std::ios::in  | std::ios::binary);
    }

    Serialization::~Serialization()
    {
        m_stream.is_open() ? 
            m_stream.close() : 
            toml_free(m_toml);
    }

    void Serialization::writeBinary(void *in, u32 len, u32 tsize)
    {
        m_stream.write(reinterpret_cast<char*>(in), len * tsize);
    }

    void Serialization::readBinary(void *out, u32 len, u32 tsize)
    {
        m_stream.read(reinterpret_cast<char*>(out), len * tsize);
    }

    bool Serialization::readBool(const std::string &key)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        return data.u.boolean;
    }

    s64 Serialization::readInt(const std::string &key)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        return data.u.int64;
    }

    double Serialization::readFlt(const std::string &key)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        return data.u.fp64;
    }

    std::string Serialization::readString(const std::string &key)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        return std::string(data.u.s);
    }

    void Serialization::readArrayString(const std::string &key, void *ptr)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        std::string *buf = static_cast<std::string *>(ptr);

        for (u32 i = 0; i < data.u.arr.size; i++)
        {
            buf[i] = std::string(data.u.arr.elem[i].u.s);
        }
    }

    template <typename T>
    void Serialization::readArrayInt(const std::string &key, void *ptr)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        T *buf = static_cast<T*>(ptr);

        for (u32 i = 0; i < data.u.arr.size; i++)
        {
            buf[i] = (T)data.u.arr.elem[i].u.int64;
        }
    }

    template <typename T>
    void Serialization::readArrayFlt(const std::string &key, void *ptr)
    {
        toml_datum_t data = toml_seek(m_toml.toptab, key.c_str());
        T *buf = static_cast<T *>(ptr);

        for (u32 i = 0; i < data.u.arr.size; i++)
        {
            buf[i] = (T)data.u.arr.elem[i].u.fp64;
        }
    }

    template void Serialization::readArrayInt<u8>(const std::string&, void*);
    template void Serialization::readArrayInt<u16>(const std::string&, void*);
    template void Serialization::readArrayInt<u32>(const std::string&, void*);
    template void Serialization::readArrayInt<u64>(const std::string&, void*);
    template void Serialization::readArrayInt<s8>(const std::string&, void*);
    template void Serialization::readArrayInt<s16>(const std::string&, void*);
    template void Serialization::readArrayInt<s32>(const std::string&, void*);
    template void Serialization::readArrayInt<s64>(const std::string&, void*);

    template void Serialization::readArrayFlt<f32>(const std::string&, void*);
    template void Serialization::readArrayFlt<double>(const std::string&, void*);
}