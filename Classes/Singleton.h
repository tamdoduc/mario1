#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
    static T* getInstance()
    {
        if (!_instance)
        {
            _instance = new T();
        }
        return _instance;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T* _instance;
};

template <typename T>
T* Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
