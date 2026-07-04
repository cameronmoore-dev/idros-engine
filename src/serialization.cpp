#include "serialization.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/pointer.h>

namespace idrs
{
    static rapidjson::Document dread;
    static rapidjson::Document dwrite;

    Serialization::Serialization(const std::filesystem::path &path, IO type)
    {
        if (type == IO::ReadText)
        {
            FILE *fp;
            fopen_s(&fp, path.string().c_str(), "rb");
            char contents[path.string().size()];
            rapidjson::FileReadStream frs(fp, contents, sizeof(contents));

            dread.ParseStream(frs);
            fclose(fp);
            return;
        }
        else if (type == IO::WriteText)
        {
            dwrite.SetObject();
            return;
        }

        m_stream = (type == IO::WriteBinary) ? 
            std::fstream(path, std::ios::out | std::ios::binary) : 
            std::fstream(path, std::ios::in  | std::ios::binary);
    }

    Serialization::~Serialization()
    {
        if (m_stream.is_open())
        {
            m_stream.close();
        }

        else if (dread.IsNull())
        {
            /* Push write document contents into a file */
            FILE *fp;
            fopen_s(&fp, "_prefab.json", "w");
            char buf[UINT16_MAX];

            rapidjson::FileWriteStream os(fp, buf, sizeof(buf));
            rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
            writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
            dwrite.Accept(writer);

            fclose(fp);
            dwrite.SetNull();
            return;
        }

        dread.SetNull();
    }

    void Serialization::writeBinary(void *in, u32 len, u32 tsize)
    {
        m_stream.write(reinterpret_cast<char*>(in), len * tsize);
    }

    void Serialization::readBinary(void *out, u32 len, u32 tsize)
    {
        m_stream.read(reinterpret_cast<char*>(out), len * tsize);
    }

    template<typename T>
    void Serialization::writeSingle(const std::string &key, T value)
    {
        dwrite.AddMember(rapidjson::GenericStringRef(key.c_str()), rapidjson::Value(value), dwrite.GetAllocator());
    }

    void Serialization::writeString(const std::string &key, const std::string &value)
    {
        dwrite.AddMember(rapidjson::GenericStringRef(key.c_str()), rapidjson::GenericStringRef(value.c_str()), dwrite.GetAllocator());
    }

    template<typename T>
    void Serialization::writeArray(const std::string &key, T *ptr, u32 len)
    {
        rapidjson::Value arr(rapidjson::kArrayType);
        arr.Reserve(len, dwrite.GetAllocator());

        for (u32 i = 0; i < len; i++)
        {
            arr.PushBack(ptr[i], dwrite.GetAllocator());
        }

        dwrite.AddMember(rapidjson::GenericStringRef(key.c_str()), arr, dwrite.GetAllocator());
    }

    void Serialization::writeStringArray(const std::string &key, std::string *ptr, u32 len)
    {
        rapidjson::Value arr(rapidjson::kArrayType);
        arr.Reserve(len, dwrite.GetAllocator());

        for (u32 i = 0; i < len; i++)
        {
            arr.PushBack(rapidjson::GenericStringRef(ptr[i].c_str()), dwrite.GetAllocator());
        }

        dwrite.AddMember(rapidjson::GenericStringRef(key.c_str()), arr, dwrite.GetAllocator());
    }

    bool Serialization::readBool(const std::string &key)
    {
        rapidjson::Pointer p(key.c_str());
        rapidjson::Value *v = rapidjson::GetValueByPointer(dread, p);
        return v->GetBool();
    }

    s64 Serialization::readInt(const std::string &key)
    {
        rapidjson::Pointer ptr(key.c_str());
        rapidjson::Value *value = rapidjson::GetValueByPointer(dread, ptr);
        return value->GetInt64();
    }

    f32 Serialization::readFlt(const std::string &key)
    {
        rapidjson::Pointer p(key.c_str());
        rapidjson::Value *v = rapidjson::GetValueByPointer(dread, p);
        return v->GetFloat();
    }

    std::string Serialization::readString(const std::string &key)
    {
        rapidjson::Pointer p(key.c_str());
        rapidjson::Value *v = rapidjson::GetValueByPointer(dread, p);
        return v->GetString();
    }

    void Serialization::readArrayString(const std::string &key, void *ptr)
    {
        rapidjson::Pointer p(key.c_str());
        rapidjson::Value *v = rapidjson::GetValueByPointer(dread, p);

        std::string *buf = static_cast<std::string*>(ptr);
        for (u32 i = 0; i < v->Size(); i++)
        {
            buf[i] = std::string(v->GetArray()[i].GetString());
        }
    }

    template <typename T>
    void Serialization::readArrayInt(const std::string &key, void *ptr)
    {
        rapidjson::Pointer p(key.c_str());
        rapidjson::Value *v = rapidjson::GetValueByPointer(dread, p);

        T *buf = static_cast<T*>(ptr);
        for (u32 i = 0; i < v->Size(); i++)
        {
            buf[i] = (T)v->GetArray()[i].GetInt();
        }
    }

    template <typename T>
    void Serialization::readArrayFlt(const std::string &key, void *ptr)
    {
        rapidjson::Pointer p(key.c_str());
        rapidjson::Value *v = rapidjson::GetValueByPointer(dread, p);

        T *buf = static_cast<T*>(ptr);
        for (u32 i = 0; i < v->Size(); i++)
        {
            buf[i] = (T)v->GetArray()[i].GetFloat();
        }
    }

    template void Serialization::writeSingle<bool>(const std::string&, bool);
    template void Serialization::writeSingle<u8>(const std::string&, u8);
    template void Serialization::writeSingle<u16>(const std::string&, u16);
    template void Serialization::writeSingle<u32>(const std::string&, u32);
    template void Serialization::writeSingle<u64>(const std::string&, u64);
    template void Serialization::writeSingle<s8>(const std::string&, s8);
    template void Serialization::writeSingle<s16>(const std::string&, s16);
    template void Serialization::writeSingle<s32>(const std::string&, s32);
    template void Serialization::writeSingle<s64>(const std::string&, s64);
    template void Serialization::writeSingle<f32>(const std::string&, f32);
    template void Serialization::writeSingle<double>(const std::string&, double);

    template void Serialization::writeArray<bool>(const std::string&, bool*, u32);
    template void Serialization::writeArray<u8>(const std::string&, u8*, u32);
    template void Serialization::writeArray<u16>(const std::string&, u16*, u32);
    template void Serialization::writeArray<u32>(const std::string&, u32*, u32);
    template void Serialization::writeArray<u64>(const std::string&, u64*, u32);
    template void Serialization::writeArray<s8>(const std::string&, s8*, u32);
    template void Serialization::writeArray<s16>(const std::string&, s16*, u32);
    template void Serialization::writeArray<s32>(const std::string&, s32*, u32);
    template void Serialization::writeArray<s64>(const std::string &, s64*, u32);
    template void Serialization::writeArray<f32>(const std::string &, f32*, u32);
    template void Serialization::writeArray<double>(const std::string &, double*, u32);

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
