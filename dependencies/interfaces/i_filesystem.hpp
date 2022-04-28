#pragma once

class i_filesystem
{
private:
    template < typename T >
    static T get_virtual(void* pClass, int nIndex)
    {
        return reinterpret_cast<T>((*(int**)pClass)[nIndex]);
    }
public:
    int read(void* output, int size, void* file)
    {
        typedef int(__thiscall* t_read)(void*, void*, int, void*);
        return get_virtual< t_read >(this, 0)(this, output, size, file);
    }

    void* open(const char* file_name, const char* options, const char* path_id)
    {
        typedef void* (__thiscall* t_open)(void*, const char*, const char*, const char*);
        return get_virtual< t_open >(this, 2)(this, file_name, options, path_id);
    }

    void close(void* file)
    {
        typedef void(__thiscall* t_close)(void*, void*);
        return get_virtual< t_close >(this, 3)(this, file);
    }

    unsigned int size(void* file)
    {
        typedef unsigned int(__thiscall* t_size)(void*, void*);
        return get_virtual< t_size >(this, 7)(this, file);
    }
};