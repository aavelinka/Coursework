#include "TextFile.h"
#include "FileException.h"

template <typename T>
void TextFile<T>::clearFile() const
{
    ofstream file(filename, ios::out | ios::trunc);
    if (!file.is_open())
    {
        throw FileException(60, "Не удалось очистить файл: " + filename);
    }
    file.close();
}

template <typename T>
void TextFile<T>::openFile(fstream& file, ios_base::openmode mode) const
{
    file.open(filename, mode);
    if (!file.is_open())
    {
        // Создаем файл если он не существует
        ofstream createFile(filename);
        if (!createFile.is_open())
        {
            throw FileException(61, "Не удалось создать файл: " + filename);
        }
        createFile.close();
        
        // Пытаемся открыть снова
        file.open(filename, mode);
        if (!file.is_open())
        {
            throw FileException(62, "Не удалось открыть файл после создания: " + filename);
        }
    }
}

template <typename T>
void TextFile<T>::saveRecord(const T& object)
{
    fstream file;
    openFile(file, ios::out | ios::app);

    if (!file.is_open()) {
        throw FileException(100, "Failed to open file for writing");
    }
    
    file << object;
    
    file.close();
}

template <typename T>
T* TextFile<T>::readRecord()
{
    fstream file;
    openFile(file, ios::in);
    
    T* object = new T(); 
    
    if (file >> *object)
    {
        file.close();
        return object;
    }
    
    delete object;
    file.close();
    return nullptr;
}

template <typename T>
vector<T*> TextFile<T>::readAllRecords()
{
    vector<T*> records;
    fstream file;
    openFile(file, ios::in);

    while (file.peek() != EOF)
    {
        T* object = new T();
        if (file >> *object)
        {
            records.push_back(object);
        }
        else
        {
            delete object;
            break;
        }
    }
    
    file.close();
    return records;
}

template <typename T>
bool TextFile<T>::hasRecords() const 
{
    ifstream file(filename);
    return file.good() && file.peek() != EOF;
}

template <typename T>
TextFile<T>& TextFile<T>::operator=(const TextFile<T>& other)
{
    if (this != &other) 
    {
        this->filename = other.filename;
    }
    return *this;
}