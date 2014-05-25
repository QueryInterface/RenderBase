#include <xstddef>
#include <memory>

using namespace std;

// TEMPLATE FUNCTIONS make_shared and allocate_shared
#define _ALLOCATE_MAKE_SHARED_MOD( \
	TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, X1, X2, X3, X4) \
template<class _Ty COMMA LIST(_CLASS_TYPE)> inline \
	shared_ptr<_Ty> make_shared_mod(LIST(_TYPE_REFREF_ARG)) \
	{	/* make a shared_ptr */ \
	_Ref_count_obj<_Ty> *_Rx = \
		new _Ref_count_obj<_Ty>(LIST(_FORWARD_ARG)); \
	shared_ptr<_Ty> _Ret; \
	_Ret._Resetp0(_Rx->_Getptr(), _Rx); \
	return (_Ret); \
	} \
template<class _Ty, \
	class _Alloc COMMA LIST(_CLASS_TYPE)> inline \
	shared_ptr<_Ty> allocate_shared_mod( \
		const _Alloc& _Al_arg COMMA LIST(_TYPE_REFREF_ARG)) \
	{	/* make a shared_ptr */ \
	typedef _Ref_count_obj_alloc<_Ty, _Alloc> _Refoa; \
	typename _Alloc::template rebind<_Refoa>::other _Alref = _Al_arg; \
	_Refoa *_Rx = _Alref.allocate(1); \
	_TRY_BEGIN \
		::new (_Rx) _Refoa(_Al_arg COMMA LIST(_FORWARD_ARG)); \
	_CATCH_ALL \
		_Alref.deallocate(_Rx, 1); \
	_RERAISE; \
	_CATCH_END \
	shared_ptr<_Ty> _Ret; \
	_Ret._Resetp0(_Rx->_Getptr(), _Rx); \
	return (_Ret); \
	}

_VARIADIC_EXPAND_0X(_ALLOCATE_MAKE_SHARED_MOD, , , , )
#undef _ALLOCATE_MAKE_SHARED_MOD

class MyClass
{
    uint32_t fake;
};

int main()
{
    std::shared_ptr<MyClass> var = make_shared_mod<MyClass>();
    return 0;
}