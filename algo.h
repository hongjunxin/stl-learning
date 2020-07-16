#ifndef _ALGO_H_
#define _ALGO_H_

template <typename Iterator, typename T>
Iterator find(Iterator begin, Iterator end, const T& value)
{
    while (begin != end && *begin != value)
        ++begin;
    return begin;
}

template <typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
	for (; first != last; ++first)
		init = init + *first;
	return init;
}

template <typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init,
		     BinaryOperation binary_op)
{
	for (; first != last; ++first)
		init = binary_op(init, *first);
	return init;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
							       OutputIterator result)
{
	if (first == last) return result;
	*result = *first;
	return __adjacent_difference(first, last, result, value_type(first));
}								   

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last,
                                     OutputIterator result, T*)
{
	T value = *first;
	while (++first != last) {
		T tmp = *first;
		*++result = tmp - value;
		value = tmp;
	}
	return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
							       OutputIterator result, BinaryOperation binary_op)
{
	if (first == last) return result;
	*result = *first;
	return __adjacent_difference(first, last, result, value_type(first), binary_op);
}	
							   
template <typename InputIterator, typename OutputIterator, typename T,
		  typename BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last,
                                     OutputIterator result, T*,
									 BinaryOperation binary_op)
{
	T value = *first;
	while (++first != last) {
		T tmp = *first;
		*++result = binary_op(tmp, value);
		value = tmp;
	}
	return ++result;
}									 

template <typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init)
{
	for (; first1 != last1; ++first1, ++first2)
		init = init + (*first1 * *first2);
	return init;
}				

template <typename InputIterator1, typename InputIterator2, typename T,
          typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init, BinaryOperation1 binary_op1,
				BinaryOperation2 binary_op2)
{
	for (; first1 != last1; ++first1, ++first2)
		init = binary_op1(init, binary_op2(*first1, *first2));
	return init;
}				

template <typename InputIterator, typename OutputIterator>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
                             OutputIterator result, T*)
{
	T value = *first;
	while (++first != last) {
		value = value + *first;
		*++result = value;
	}
	return ++result;
}							 

template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result)
{
	if (first == last) return result;
	*result = *first;
	return __partial_sum(first, last, result, value_type(first));
}						   

template <typename InputIterator, typename OutputIterator, 
          typename T, typename BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
                             OutputIterator result, T*, 
							 BinaryOperation binary_op)
{
	T value = *first;
	while (++first != last) {
		value = binary_op(value, *first);
		*++result = value;
	}
	return ++result;
}							 

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op)
{
	if (first == last) return result;
	*result = *first;
	return __partial_sum(first, last, result, value_type(first), binary_op);
}						   

template <typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
                             OutputIterator result, input_iterator_tag)
{
	for (; first != last; ++result, ++first)
		*result = *first;
	return result;
}

template <typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator
__copy(RandomAccessIterator first, RandomAccessIterator last,
       OutputIterator result, random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}	   

template <typename RandomAccessIterator, typename OutputIterator,
          typename Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomAccessIterator last,
         OutputIterator result, Distance*)
{
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}		 

template <typename InputIterator, typename OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator() (InputIterator first, InputIterator last,
	                           OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};

template <typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

template <typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
	return __copy_d(first, last, result, (ptrdiff_t*) 0);
}

template <typename T>
struct __copy_dispatch<T*, T*>
{
	T* operator() (T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template <typename T>
struct __copy_dispatch<const T*, T*>
{
	T* operator() (const T* first, const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	return __copy_dispatch<InputIterator, OutputIterator>() 
			(first, last, result);
}

inline char* copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}


#endif