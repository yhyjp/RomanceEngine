#ifndef __RM_MEMORY_SHARED_PTR__9EC5959313EE4F099B8F57FA2B941611__
#define __RM_MEMORY_SHARED_PTR__9EC5959313EE4F099B8F57FA2B941611__

#include <boost/shared_ptr.hpp>

namespace RomanceEngine {
namespace Memory {
  template<class T>
  class SharedPtr : public boost::shared_ptr<T>
  {
  public:
    SharedPtr() : boost::shared_ptr<T>() {}
    SharedPtr(SharedPtr const & r) : boost::shared_ptr<T>(r) {}
    template<class Y> explicit SharedPtr(Y * p) : boost::shared_ptr<T>(p) {}
    template<class Y, class D> SharedPtr(Y * p, D d) : boost::shared_ptr<T>(p, d) {}
    template<class Y, class D, class A> SharedPtr( Y * p, D d, A a ) : boost::shared_ptr<T>(p, d, a) {}
  };
  
} // namespace Math
} // namespace RomanceEngine


#endif //__RM_MEMORY_SHARED_PTR__9EC5959313EE4F099B8F57FA2B941611__