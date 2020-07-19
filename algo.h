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

/* sorted range as a precondition */
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
						 OutputIterator result)
{
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		} else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		} else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}						 

/* sorted range as a precondition */
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
						        OutputIterator result)
{
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}						 

/* sorted range as a precondition */
/* eg: S1(1,2,3,4), S2(1,3,5), set_difference(S1, S2) -> R(2, 4) 
 * R = S1 - S2, means elements in S1 but not't in S2.
 */
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
						      OutputIterator result)
{
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		} else if (*first2 < *first1) {
			++first2;
		} else {
			++first1;
			++first2;
		}
	}
	return copy(first1, last1, result);
}	

/* sorted range as a precondition */
/* Result = (S1 - S2) U (S2 - S1) */
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
						      OutputIterator result)
{
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		} else if(*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		} else {
			++first1;
			++first2;
		}
	}
	return copy(first2, last2, copy(first1, last1, result));
}
				
template <typename ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
{
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;
		first = next;
	}
	return last;
}

template <typename ForwardIterator, typename BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
                              BinaryPredicate binary_pred)
{
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (binary_pred(*first, *next)) return first;
		first = next;
	}
	return last;
}

template <typename InputIterator, typename T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}

template <typename InputIterator, typename Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (pred(*first))
			++n;
	return n;
}

template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T& value)
{
	while (first != last && *first != value) ++first;
	return first;
}

template <typename InputIterator, typename Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
{
	while (first != last && !pred(*first)) ++first;
	return first;
}

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
							forward_iterator_tag, forward_iterator_tag)
{
	if (first2 == last2)
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {
			ForwardIterator1 new_result = search(first1, last1, first2, last2);
			if (new_result == last1)
				return result;
			else {
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}							

template <typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator1 
__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
           BidirectionalIterator2 first2, BidirectionalIterator2 last2,
		   bidirectional_iterator_tag, bidirectional_iterator_tag)
{
	typedef reverse_iterator<BidirectionalIterator1> reviter1;
	typedef reverse_iterator<BidirectionalIterator2> reviter2;
	
	reviter1 rlast1(first1);
	reviter2 rlast2(first2);
	
	reviter1 rresult = search(reviter1(last1), rlast1,
	                          reviter2(last2), rlast2);
	
	if (rresult == rlast1)
		return last1;
	else {
		BidirectionalIterator1 result = rresult.base();
		advance(result, -distance(first2, last2));
		return result;
	}
}

template <typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1,
         ForwardIterator2 first2, ForwardIterator2 last2)
{
	typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
	typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
	
	return __find_end(first1, last1, first2, last2, category1(), category2());
}		 

template <typename InputIterator, typename ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2)
{
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (*first1 == *iter)
				return first1;
	return last1;
}							

template <typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
							BinaryPredicate comp)
{
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (comp(*first1, *iter))
				return first1;
	return last1;
}	

template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f)
{
	for (; first != last; ++first)
		f(*first);
	return f;
}

template <typename ForwardIterator, typename Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen)
{
	for (; first != last; ++first)
		*first = gen();
}

template <typename OutputIterator, typename Size, typename Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen)
{
	for (; n > 0; --n, ++first)
		*first = gen();
	return first;
}

/* sorted range as a precondition */
template <typename InputIterator1, typename InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			return false;
		else if (*first1 < *first2)
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}			  

/* sorted range as a precondition */
template <typename InputIterator1, typename InputIterator2, typename Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2, Compare comp)
{
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2, *first1))
			return false;
		else if (comp(*first1, *first2))
			++first1;
		else 
			++first1, ++first2;
	}
	return first2 == last2;
}			  

template <typename ForwardIterator>
ForwardIterator
max_element(ForwardIterator first, ForwardIterator last)
{
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result < *first) result = first;
	return result;
}

template <typename ForwardIterator, typename Compare>
ForwardIterator
max_element(ForwardIterator first, ForwardIterator last, Compare comp)
{
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*result, *first)) result = first;
	return result;
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

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
					 OutputIterator result)
{
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		} else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator,
          typename Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
					 OutputIterator result, Compare comp)
{
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2, *first1)) {
			*result = *first2;
			++first2;
		} else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

template <typename ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
{
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*first < *result) result = first;
	return result;
}

template <typename ForwardIterator, typename Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator lastm
                            Compare comp)
{
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*first, *result)) result = first;
	return result;
}

template <typename BidirectionalIterator, typename Predicate>
BidirectionalIterator
partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred)
{
	while (true) {
		while (true)
			if (first == last)
				return first;
			else if (pred(*first))
				++first;
			else 
				break;
		--last;
		while (true)
			if (first == last)
				return first;
			else if (!pred(*last))
				--last;
			else
				break;
		iter_swap(first, last);
		++first;
	}
}

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                           OutputIterator result, const T& value)
{
	for (; first != last; ++first)
		if (*first != value) {
			*result = *first;
			++result;
		}
	return result;
}

template <typename ForwardIterator, typename T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T& value)
{
	first = find(first, last, value);
	ForwardIterator next = first;
	
	return first == last ? first : remove_copy(++next, last, first, value);
}

template <typename InputIterator, typename OutputIterator, typename Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred)
{
	for (; first != last; ++first)
		if (!pred(*first)) {
			*result = *first;
			++result;
		}
	return result;
}							  

template <typename ForwardIterator, typename Predicate>
ForwardIterator
remove_if(ForwardIterator first, ForwardIterator last, Predicate pred)
{
	first = find_if(first, last, pred);
	ForwardIterator next = first;
	
	return first == last ? first : remove_copy_if(++next, last, first, pred);
}

template <typename ForwardIterator, typename T>
void replace(ForwardIterator first, ForwardIterator last,
             const T& old_value, const T& new_value)
{
	for (; first != last; ++first)
		if (*first == old_value) *first = new_value;
}

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T& old_value,
							const T& new_value)
{
	for (; first != last; ++first, ++result)
		*result = *first == old_value ? new_value : *first;
	return result;
}

template <typename ForwardIterator, typename Predicate, typename T>
void replace_if(ForwardIterator first, ForwardIterator last,
                Predicate pred, const T& new_value)
{
	for (; first != last; ++first)
		if (pred(*first)) *first = new_value;
}

template <typename Iterator, typename OutputIterator, typename Predicate, typename T>
OutputIterator replace_copy_if(Iterator first, Iterator last,
                               OutputIterator result, Predicate pred,
							   const T& new_value)
{
	for (; first != last; ++first, ++result)
		*result = pred(*first) ? new_value : *first;
	return result;
}

template <typename BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last,
               bidirectional_iterator_tag)
{
	while (true)
		if (first == last || first == --last)
			return;
		else
			iter_swap(first++, last);
}

template <typename RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag)
{
	/* only RandomAccessIterator has < operation */
	while (first < last) iter_swap(first++, --last);
}

template <typename BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last)
{
	__reverse(first, last, iterator_category(first));
}

template <typename BidirectionalIterator, typename OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
                            OutputIterator result)
{
	while (first != last) {
		--last;
		*result = *last;
		++result;
	}
	return result;
}

template <typename ForwardIterator, typename Distance>
void __rotate(ForwardIterator first, ForwardIterator middle,
               ForwardIterator last, Distance*, forward_iterator_tag)
{
	for (ForwardIterator i = middle; ;) {
		iter_swap(first, i);
		++first;
		++i;
		if (first == middle) {
			if (i == last) return;
			middle = i;
		} else if (i == last) {
			i = middle;
		}
	}
}

template <typename BidirectionalIterator, typename Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
              BidirectionalIterator last, Distance*,
			  bidirectional_iterator_tag)
{
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
}

template <typename EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
{
	while (n != 0) {
		EuclideanRingElement t = m % n;
		m = n;
		n = t;
	}
	return m;
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomAccessIterator initial, Distance shift, T*)
{
	T value = *initial;
	RandomAccessIterator ptr1 = initial;
	RandomAccessIterator ptr2 = ptr1 + shift;
	
	while (ptr2 != initial) {
		*ptr1 = *ptr2;
		ptr1 = ptr2;
		if (last - ptr2 > shift)
			ptr2 += shift;
		else
			ptr2 = first + (shift - (last - ptr2));
	}
	
	*ptr1 = value;
}					

template <typename RandomAccessIterator, typename Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
              RandomAccessIterator last, Distance*, random_access_iterator_tag)
{
	Distance n = __gcd(last - first, middle - first);
	while (n--)
		__rotate_cycle(first, last, first + n, middle - first,
					   value_type(first));
}

template <typename ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle,
                   ForwardIterator last)
{
	if (first == middle || middle == last) return;
	__rotate(first, middle, last, distance_type(first),
			 iterator_category(first));
}

template <typename ForwardIterator, typename OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, OutputIterator result)
{
	return copy(first, middle, copy(middle, last, result));
}

template <typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator search(ForwardIterator1 first1, ForwardIterator1 last1,
                              ForwardIterator2 first2, ForwardIterator2 last2)
{
	return __search(first1, last1, first2, last2, distance_type(first1),
	                distance_type(first2));
}

template <typename ForwardIterator1, typename ForwardIterator2,
          typename Distance1, typename Distance2>
ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator1 first2, ForwardIterator2 last2,
						  Distance1*, Distance2*)
{
	Distance1 d1 = 0;
	Distance2 d2 = 0;
	distance(first1, last1, d1);
	distance(first2, last2, d2);
	
	if (d1 < d2) return last1;
	
	ForwardIterator1 current1 = first1;
	ForwardIterator2 current2 = first2;
	
	while (current2 != last2) {
		if (*current1 == *current2) {
			++current1;
			++current2;
		} else {
			if (d1 == d2)
				return last1;
			else {
				current1 = ++first1;
				current2 = first2;
				--d1;
			}
		}
	}
	return first1;
}

template <typename ForwardIterator, typename Integer, typename T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value)
{
	if (count <= 0)
		return first;
	else {
		first = find(first, last, value);
		while (first != last) {
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while (i != last && n != 0 && *i == value) {
				++i;
				--n;
			}
			if (n == 0)
				return first;
			else
				first = find(i, last, value);
		}
		return last;
	}
}

template <typename ForwardIterator, typename Integer, typename T,
          typename BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value,
						 BinaryPredicate binary_pred)
{
	if (count <= 0)
		return first;
	else {
		while (first != last) {
			if (binary_pred(*first, value)) break;
			++first;
		}
		while (first != last) {
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while (i != last && n != 0 && binary_pred(*i, value)) {
				++i;
				--n;
			}
			if (n == 0)
				return first;
			else {
				while (i != last) {
					if (binary_pred(*i, value)) break;
					++i;
				}
				first = i;
			}
		}
		return last;
	}
}

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2)
{
	for (; first1 != last1; ++first1, ++first2)
		iter_swap(first1, first2);
	return first2;
}

template <typename InputIterator, typename OutputIterator, typename UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
                         OutputIterator result, UnaryOperation op)
{
	for (; first != last; ++first, ++result)
		*result = op(*first);
	return result;
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator,
          typename BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, OutputIterator result,
						 BinaryOperation binary_op)
{
	for (; first1 != last1; ++first1, ++first2, ++result)
		*result = binary_op(*first1, *first2);
	return result;
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
                              ForwardIterator result, forward_iterator_tag)
{
	*result = *first;
	while (++first != last)
		if (*result != *first) *++result = *first;
	return ++result;
}

/* output iterator is write-only, so we can't do *result */
template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, T*)
{
	T value = *first;
	*result = value;
	while (++first != last)
		if (value != *first) {
			value = *first;
			*++result = value;
		}
	return ++result;
}

template <typename InputIterator, typename OutputIterator>
inline OutputIterator __unique_copy(InputIterator first, InputIterator last,
                                    OutputIterator result, output_iterator_tag)
{
	return __unique_copy(first, last, result, value_type(first));
}

template <typename InputIterator, typename OutputIterator>
inline OutputIterator
unique_copy(InputIterator first, InputIterator last, OutputIterator result)
{
	if (first == last) return result;
	return __unique_copy(first, last, result, iterator_category(result));
}

/* sorted range as a precondition */
template <typename ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last)
{
	first = adjacent_find(first, last);
	return unique_copy(first, last, first);
}

template <typename RandomAccessIterator, typename T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last, T*)
{
	make_heap(first, middle);
	for (RandomAccessIterator i = middle; i < last; ++i)
		if (*i < *first)
			__pop_heap(first, middle, i, T(*i), distance_type(first));
	sort_heap(first, middle);
}

template <typename RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                         RandomAccessIterator last)
{
	__partial_sort(first, middle, last, value_type(first));
}

template <typename RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		__linear_insert(first, i, value_type(first));
}

template <typename RandomAccessIterator, typename T>
inline void __linear_insert(RandomAccessIterator first,
                            RandomAccessIterator last, T*)
{
	T value = *last;
	if (value < *first) {
		copy_backward(first, last, last + 1);
		*first = value;
	}
	else {
		__unguarded_linear_insert(last, value);
	}
}

void __unguarded_linear_insert(RandomAccessIterator last, T value)
{
	RandomAccessIterator next = last;
	--next;
	while (value < *next) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}

template <typename T>
inline const T& __median(const T& a, const T& b, const T& c)
{
	if (a < b)
		if (b < c)
			return b;
		else if (a < c)
			return c;
		else
			return a;
	else if (a < c)
		return a;
	else if (b < c)
		return c;
	else 
		return b;
}


#endif