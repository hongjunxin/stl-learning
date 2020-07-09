#ifndef _HEAP_IMPL_H_
#define _HEAP_IMPL_H_

template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	/* make sure new element already in the end */
	__push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <typename RandomAccessIterator, typename Distance, typename T>
inline void __push_heap_aux(RandomAccessIterator first, 
							RandomAccessIterator last, Distance*, T*)
{
	__push_heap(first, Distance((last - first) - 1), Distance(0), 
				T(*(last - 1)));
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
				 Distance topIndex, T value)
{
	Distance parent = (holeIndex - 1) / 2;
	/* max-heap */
	while (holeIndex > topIndex && *(first + parent) < value) {
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}

template <typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__pop_heap_aux(first, last, value_type(first));
}

template <typename RandomAccessIterator, typename T>
inline void __pop_heap_aux(RandomAccessIterator first,
						   RandomAccessIterator last, T*)
{
	__pop_heap(first, last-1, last-1, T(*(last-1)), distance_type(first));
}

template <typename RandomAccessIterator, typename Distance, typename T>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
					   RandomAccessIterator result, T value, Distance*)
{
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
				   Distance len, T value)
{
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2; // right child
	while (secondChild < len) {
		if (*(first + secondChild) < *(first + (secondChild - 1)))
			secondChild--;
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * secondChild + 2;
	}
	if (secondChild == len) {
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	__push_heap(first, holeIndex, topIndex, value);
}

template <typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	while (last - first > 1)
		pop_heap(first, last--);
}

#endif