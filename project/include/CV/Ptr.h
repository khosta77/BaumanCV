#ifndef CVRANGEFINDER_PTR_H
#define CVRANGEFINDER_PTR_H

#include <memory>
#include <type_traits>
#include "uchar_in_typedef.h"

template<typename T>
struct Ptr : public std::shared_ptr<T>
{
    inline Ptr()  : std::shared_ptr<T>() {}
    inline Ptr(nullptr_t)  : std::shared_ptr<T>(nullptr) {}
    template<typename Y, typename D> inline Ptr(Y* p, D d) : std::shared_ptr<T>(p, d) {}
    template<typename D> inline Ptr(nullptr_t, D d) : std::shared_ptr<T>(nullptr, d) {}

    template<typename Y> inline Ptr(const Ptr<Y>& r, T* ptr)  : std::shared_ptr<T>(r, ptr) {}

    inline Ptr(const Ptr<T>& o)  : std::shared_ptr<T>(o) {}
    inline Ptr(Ptr<T>&& o)  : std::shared_ptr<T>(std::move(o)) {}

    template<typename Y> inline Ptr(const Ptr<Y>& o)  : std::shared_ptr<T>(o) {}
    template<typename Y> inline Ptr(Ptr<Y>&& o)  : std::shared_ptr<T>(std::move(o)) {}

    inline Ptr(const std::shared_ptr<T>& o)  : std::shared_ptr<T>(o) {}
    inline Ptr(std::shared_ptr<T>&& o)  : std::shared_ptr<T>(std::move(o)) {}

    // Overload with custom DefaultDeleter: Ptr<IplImage>(...)
//    template<typename Y>
//    inline Ptr(const std::true_type&, Y* ptr) : std::shared_ptr<T>(ptr, DefaultDeleter<Y>()) {}

    // Overload without custom deleter: Ptr<std::string>(...);
    template<typename Y>
    inline Ptr(const std::false_type&, Y* ptr) : std::shared_ptr<T>(ptr) {}

//    template<typename Y = T>
//    inline Ptr(Y* ptr) : Ptr(has_custom_delete<Y>(), ptr) {}

    // Overload with custom DefaultDeleter: Ptr<IplImage>(...)
//    template<typename Y>
//    inline void reset(const std::true_type&, Y* ptr) { std::shared_ptr<T>::reset(ptr, DefaultDeleter<Y>()); }

    // Overload without custom deleter: Ptr<std::string>(...);
    template<typename Y>
    inline void reset(const std::false_type&, Y* ptr) { std::shared_ptr<T>::reset(ptr); }

//    template<typename Y>
//    inline void reset(Y* ptr) { Ptr<T>::reset(has_custom_delete<Y>(), ptr); }

    template<class Y, class Deleter>
    void reset(Y* ptr, Deleter d) { std::shared_ptr<T>::reset(ptr, d); }

    void reset()  { std::shared_ptr<T>::reset(); }

    Ptr& operator=(const Ptr& o) { std::shared_ptr<T>::operator =(o); return *this; }
    template<typename Y> inline Ptr& operator=(const Ptr<Y>& o) { std::shared_ptr<T>::operator =(o); return *this; }

    T* operator->() const  { return std::shared_ptr<T>::get();}
    typename std::add_lvalue_reference<T>::type operator*() const  { return *std::shared_ptr<T>::get(); }

    // OpenCV 3.x methods (not a part of standard C++ library)
    inline void release() { std::shared_ptr<T>::reset(); }
    inline operator T* () const { return std::shared_ptr<T>::get(); }
    inline bool empty() const { return std::shared_ptr<T>::get() == nullptr; }

    template<typename Y> inline
    Ptr<Y> staticCast() const  { return std::static_pointer_cast<Y>(*this); }

    template<typename Y> inline
    Ptr<Y> constCast() const  { return std::const_pointer_cast<Y>(*this); }

    template<typename Y> inline
    Ptr<Y> dynamicCast() const  { return std::dynamic_pointer_cast<Y>(*this); }
};

#endif //CVRANGEFINDER_PTR_H
