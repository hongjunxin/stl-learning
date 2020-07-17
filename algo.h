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

#endif