#pragma once

//Stop std::vector from checking to much
//#define _ITERATOR_DEBUG_LEVEL 0
#include <vector>
using std::vector;

#include <string>
using std::string;
using std::to_string;
#include<cassert>
#include<iostream>
using std::cout;
#include <algorithm>

template<class T>
struct vectorHack ;


template<class T>
class Dalloc {
    std::allocator<T> stdalloc;
    static vectorHack<T*> allocatedList;
public:
    static const size_t allocFile = sizeof(size_t);
    static const size_t allocLine = sizeof(size_t) + 1 * sizeof(string);
    static const size_t deallocFile = sizeof(size_t) + 2 * sizeof(string);
    static const size_t deallocLine = sizeof(size_t) + 3 * sizeof(string);
    static const size_t offsetPrel = sizeof(size_t) + 4 * sizeof(string);
    static const size_t filler = (offsetPrel + sizeof(T) - 1) / sizeof(T);
    static const size_t offset = filler * sizeof(T);

public:
    T* allocate(size_t n, const char* file, size_t line) {
        if (!n)
            return nullptr;
        char* memPtr = (char*)stdalloc.allocate(filler + n);
        allocatedList.push_back((T*)memPtr);
        new(memPtr) size_t(n);
        new(memPtr + allocFile) string(file);
        new(memPtr + allocLine) string(std::to_string(line));
        new(memPtr + deallocFile) string("XXX");
        new(memPtr + deallocLine) string("XXX");
        memset(memPtr + offset, 1, n*sizeof(T)); // To be able to revognize uninitiated memory!
        return (T*)(memPtr + offset);
    }
    void deallocate(T* ptr, size_t n, const char* file, size_t line) {
        if (!ptr)
            return; //deallocating a nullptr is a null operation
        auto findPtr = std::find(allocatedList.begin(), allocatedList.end(), ptr - filler);
        if (findPtr == allocatedList.end()) {
            assert("" == std::string("Trying to delete a non-allocated item!") + MsgAt(file, line) + '\n');
            return;
        }
        char* memPtr = (char*)*findPtr;
        size_t savedN = *(size_t*)memPtr;
        auto alFile = *reinterpret_cast<std::string*>(memPtr + allocFile);
        auto alLine = *reinterpret_cast<std::string*>(memPtr + allocLine);
        if (savedN != n) {
            auto msg = string("deleting wrong number of objects: \n") +
                "allocated " + to_string(savedN) + "deallocating " + to_string(n);
            msg += string("\ntrying to delete ") + MsgAt(file, line);
            msg += string("\nwas allocated " + MsgAt(alFile, alLine) + '\n');
            cout << msg;
            __debugbreak();
            return;
        }
        auto deFile = *reinterpret_cast<std::string*>(memPtr + deallocFile);
        auto deLine = *reinterpret_cast<std::string*>(memPtr + deallocLine);
        if (deFile != string("XXX")) {
            auto msg = string("deleting twice ") + MsgAt(file, line);
            msg += string("\nwas already deleted ") + MsgAt(deFile, deLine);
            msg += string("\nwas allocated " + MsgAt(alFile, alLine) + '\n');
            cout << msg;
            __debugbreak();
            return;
        }
        *reinterpret_cast<std::string*>(memPtr + deallocFile) = file;
        *reinterpret_cast<std::string*>(memPtr + deallocLine) = std::to_string(line);

    }
    string MsgAt(const char* file, size_t line) {
        return string(" in ") + file + string(" at ") + std::to_string(line);
    }
    string MsgAt(string file, string line) {
        return std::string(" in ") + file + " at " + line;
    }
    string MakeString(char* ptr) {
        return *reinterpret_cast<std::string*>(ptr + deallocFile);
    }
};

template<class T>
struct vectorHack : vector<T> {
    using vector<T>::vector;
    ~vectorHack() {
        this->erase(std::remove_if(this->begin(), this->end(),
            [](auto ptr) {return *reinterpret_cast<std::string*>((char*)ptr + Dalloc<T>::deallocFile) != string("XXX"); }), this->end());
        if (!this->empty()) {
            auto msg = string("Memory leaks, allocated at ") +
                "\nlength, line, file\n";
            int count = 0;
            for (auto ptr : *this) {
                char* memPtr = (char*)ptr;
                size_t savedN = *(size_t*)memPtr;
                auto alFile = *reinterpret_cast<std::string*>(memPtr + Dalloc<T>::allocFile);
                auto alLine = *reinterpret_cast<std::string*>(memPtr + Dalloc<T>::allocLine);
                msg += to_string(savedN) + "  " + alLine + "  " + alFile + "  " + to_string(++count) + '\n';
                cout << msg ;
                __debugbreak(); //Avsiktligt break för att kunna kolla minnesläckor!
            }
            this->clear();
        }
    }
};
template<class T>
vectorHack<T*> Dalloc<T>::allocatedList;

#undef allocate
#define allocate(x) allocate(x, __FILE__, __LINE__)

#undef deallocate
#define deallocate(x, y) deallocate(x, y, __FILE__, __LINE__)
