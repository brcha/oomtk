#ifndef SUPCPP_CPPABI_HPP
#define SUPCPP_CPPABI_HPP

/*! \addtogroup supcpp supc++ */
/*@{*/

/*! Beginning of the .ctors section */
extern void (*__ctors_start__)();
/*! End of the .ctors section */
extern void (*__ctors_end__)();
/*! Typedef the dso_handle */
typedef void *dso_handle;
/*! This dso_handle */
extern dso_handle __dso_handle;

#include <cstddef>
#include <typeinfo>

void *operator new (size_t size) throw();
void *operator new[] (size_t size) throw();
void *operator new (size_t size, void* memory) throw();
void *operator new[] (size_t size, void* memory) throw();
void operator delete (void * p);
void operator delete[] (void * p);
void operator delete (void *p, void *q);
void operator delete[] (void *p, void *q);

namespace __cxxabiv1
{
	extern "C"
	{
		/*! Support function for pure virtual functions */
		void __cxa_pure_virtual();
		/*! Support function for global/static object construction */
		void __cxa_initialize();
		/*!Register a function to be called at destruction
		 *\param[in] f pointer to the funtion
		 *\param[in] p additional parameter
		 *\param[in] d the dso handle
		 *\return 0, if successfull */
		int __cxa_atexit(void (*f)(void *), void *p, dso_handle d);
		/*! Support function for global/static object destruction
		 *\param[in] d the dso handle */
		void __cxa_finalize(dso_handle d);
	};
	/*! Saves information about objects that must be destroyed at exit */
	struct finalizeObject
	{
		/*! Function to call */
		void (*function)(void *);
		/*! Parameter */
		void *param;
		/*! Dynamic shared object */
		void *dso;
	};

	class __fundamental_type_info : public std::type_info
	{
		protected:
			/*! The copy-constructor */
			__fundamental_type_info(const __fundamental_type_info &rhs);
			/*! The = operator */
			__fundamental_type_info &operator = (const __fundamental_type_info &rhs);
	};

	class __array_type_info : public std::type_info
	{
		protected:
			/*! The copy-constructor */
			__array_type_info(const __array_type_info &rhs);
			/*! The = operator */
			__array_type_info &operator = (const __array_type_info &rhs);
	};

	class __function_type_info : public std::type_info
	{
		protected:
			/*! The copy-constructor */
			__function_type_info(const __function_type_info &rhs);
			/*! The = operator */
			__function_type_info &operator = (const __function_type_info &rhs);
	};

	class __enum_type_info : public std::type_info
	{
		protected:
			/*! The copy-constructor */
			__enum_type_info(const __enum_type_info &rhs);
			/*! The = operator */
			__enum_type_info &operator = (const __enum_type_info &rhs);
	};

	class __class_type_info : public std::type_info
	{
		protected:
			/*! The copy-constructor */
			__class_type_info(const __class_type_info &rhs);
			/*! The = operator */
			__class_type_info &operator = (const __class_type_info &rhs);
	};

	class __si_class_type_info : public __class_type_info
	{
		public:
			const __class_type_info *__base_type;
		protected:
			/*! The copy-constructor */
			__si_class_type_info(const __si_class_type_info &rhs);
			/*! The = operator */
			__si_class_type_info &operator = (const __si_class_type_info &rhs);
	};

	struct __base_class_type_info
	{
		public:
			const __class_type_info *__base_type;
			long __offset_flags;

			enum __offset_flags_masks
			{
				__virtual_mask	= 0x1,
				__public_mask	= 0x2,
				__offset_shift	= 8
			};
	};

	class __vmi_class_type_info : public __class_type_info
	{
		public:
			unsigned int __flags;
			unsigned int __base_count;
			__base_class_type_info __base_info[1];

			enum __flags_masks
			{
				__non_diamond_repeat_mask	= 0x1,
				__diamond_shaped_mask		= 0x2
			};
		protected:
			/*! The copy-constructor */
			__vmi_class_type_info(const __vmi_class_type_info &rhs);
			/*! The = operator */
			__vmi_class_type_info &operator = (const __vmi_class_type_info &rhs);
	};

	class __pbase_type_info : public std::type_info
	{
		public:
			unsigned int __flags;
			const std::type_info *__pointee;

			enum __masks
			{
				__const_mask = 0x1,
				__volatile_mask = 0x2,
				__restrict_mask = 0x4,
				__incomplete_mask = 0x8,
				__incomplete_class_mask = 0x10
			};
	};

	class __pointer_type_info : public __pbase_type_info
	{
		protected:
			/*! The copy-constructor */
			__pointer_type_info(const __pointer_type_info &rhs);
			/*! The = operator */
			__pointer_type_info &operator = (const __pointer_type_info &rhs);
	};
};

/*! The dynamic_cast operator */
extern "C" void* __dynamic_cast (	const void *sub,
									const __cxxabiv1::__class_type_info *src,
									const __cxxabiv1::__class_type_info *dst,
									std::ptrdiff_t src2dst_offset);

/*@}*/

#endif
