#ifndef _PRIORITY_QUEUE_IMPL_H_
#define _PRIORITY_QUEUE_IMPL_H_

template <typename T, typename Sequence = vector<T>,
	      typename Compare = less<typename Sequence::value_type>>
class priority_queue {
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;
	Compare comp;
public:
	priority_queue() : c() {}
	explicit priority_queue(const Compare& x) : c(), comp(x) {}
	
	template <typename InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& x)
		: c(first, last), comp(x)
	{ make_heap(c.begin(), c.end(), comp); }
	
	template <typename InputIterator>
	priority_queue(InputIterator first, InputIterator last) : c(first, last)
	{ make_heap(c.begin(), c.end(), comp); }
	
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	const_reference top() const { return c.front(); }
	
	void push(const value_type& x)
	{
		__STL_TRY {
			c.push_back(x);
			push_heap(c.begin(), c.end(), comp);
		}
		__STL_UNWIND(c.clear());
	}
	
	void pop()
	{
		__STL_TRY {
			pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
		__STL_UNWIND(c.clear());
	}
};

#endif