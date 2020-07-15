#ifndef _HASHTABLE_IMPL_H_
#define _HASHTABLE_IMPL_H_

template <typename Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};

template <typename Value, typename Key, typename HashFcn,
		  typename ExtractKey, typename EqualKey, typename Alloc = alloc>
class hashtable;

template <typename Value, typename Key, typename HashFcn,
	      typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey,
			Alloc> const_iterator;
	typedef __hashtable_node<Value> node;
	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;
	
	node* cur;
	hashtable* ht;
	
	__hashtable_iterator(node* n, hashtable* tab) : cur(n), ht(tab) {}
	__hashtable_iterator() {}
	reference operator*() const { return cur->val; }
	pointer operator->() const { return &(operator*()); }
	
	iterator& operator++();
	iterator operator++(int);
	
	bool operator==(const iterator& it) const { return cur == it.cur; }
	bool operator!=(const iterator& it) const { return cur != it.cur; }
};		  

template <typename Value, typename Key, typename HashFcn,
	      typename ExtractKey, typename EqualKey, typename Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
{
	const node* old = cur;
	cur = cur->next;
	if (!cur) {
		size_type bucket = ht->bkt_num(old_val);
		while (!cur && ++bucket < ht->buckets.size())
			cur = ht->buckets[bucket];
	}
	return *this;
}

template <typename Value, typename Key, typename HashFcn,
	      typename ExtractKey, typename EqualKey, typename Alloc>
inline __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
{
	iterator tmp = *this;
	++*this;
	return tmp;
}

template <typename Value, typename Key, typename HashFcn,
		  typename ExtractKey, typename EqualKey, typename Alloc>
class hashtable {
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;
private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;
	
	typedef __hashtable_node<Value> node;
	typedef simple_alloc<node, Alloc> node_allocator;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			iterator;
	
	vector<node*, Alloc> buckets;
	size_type num_elements;
	
	void initialize_buckets(size_type n)
	{
		const size_type n_buckets = next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*) 0);
		num_elements = 0;
	}
	
	node* new_node(const value_type& obj)
	{
		node* n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY {
			construct(&n->val, obj);
			return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));=
	}
	
	void delete_node(node* n)
	{
		destroy(&n->val);
		node_allocator::deallocate(n);
	}	
	
	void resize(size_type num_elements_hint);
	pair<iterator, bool> insert_unique_noresize(const value_type& obj);
	
	size_type bkt_num(const value_type& obj, size_t n) const
	{ return bkt_num_key(get_key(obj), n); }
	
	size_type bkt_num(const value_type& obj) const 
	{ return bkt_num_key(get_key(obj)); }
	
	size_type bkt_num_key(const key_type& key) const
	{ return  bkt_num_key(key, buckets.size()); }
	
	size_type bkt_num_key(const key_type& key, size_t n) const 
	{ return hash(key) % n; }
	
public:
	size_type bucket_count() const { return buckets.size(); }
	
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
		: hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
	{ initialize_buckets(n); }
	
	pair<iterator, bool> insert_unique(const value_type& obj)
	{
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}
	
	iterator insert_equal(const value_type& obj)
	{
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}
	
	void clear();
	void copy_from(const hashtable& ht);
	
	iterator find(const key_type& key)
	{
		size_type n = bkt_num_key(key);
		node* first;
		
		for (first = buckets[n]; first && !equals(get_key(first->val), key);
			 first = first->next) {}
		return iterator(first, this);
	}
	
	size_type count(const key_type& key) const
	{
		const size_type n = bkt_num_key(key);
		size_type result = 0;
		
		for (const node* cur = buckets[n]; cur; cur = cur->next)
			if (equals(get_key(cur->val), key))
				++result;
		return result;
	}
};

template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
void hashtable<V, K, HF, Ex, Eq, A>::resize(size_type num_elements_hint)
{
	const size_type old_n = buckets.size();
	if (num_elements_hint > old_n) {
		const size_type n = next_size(num_elements_hint);
		if (n > old_n) {
			vector<node*, A> tmp(n, (node*) 0);
			__STL_TRY {
				for (size_type bucket = 0; bucket < old_n; ++bucket) {
					node* first = buckets[bucket];
					while (first) {
						size_type new_bucket = bkt_num(first->val, n);
						buckets[bucket] = first->next;
						first->next = tmp[new_bucket];
						tmp[new_bucket] = first;
						first = buckets[bucket];
					}
				}
				buckets.swap(tmp);
			}
		}
	}
}

template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
pair<typename hashtable<V, K, HF, Ex, Eq, A>::iterator, bool>
hashtable<V, K, HF, Ex, Eq, A>::insert_unique_noresize(const value_type& obj)
{
	const size_type n = bkt_num(obj);
	node* first = buckets[n];
	
	for (node* cur = first; cur; cur = cur->next)
		if (equals(get_key(cur->val), get_key(obj)))
			return pair<iterator, bool> (iterator(cur, this), false);
		
	node* tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	++num_elements;
	return pair<iterator, bool> (iterator(tmp, this), true);
}

template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
typename hashtable<V, K, HF, Ex, Eq, A>::iterator
hashtable<V, K, HF, Ex, Eq, A>::insert_equal_noresize(const value_type& obj)
{
	const size_type n = bkt_num(obj);
	node* first = buckets[n];
	
	for (node* cur = first; cur; cur = cur->next) {
		if (equals(get_key(cur->val), get_key(obj))) {
			node* tmp = new_node(obj);
			tmp->next = cur->next;
			cur->next = tmp;
			++num_elements;
			return iterator(tmp, this);
		}
	}
	
	node* tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	++num_elements;
	return iterator(tmp, this);
}

template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
void hashtable<V, K, HF, Ex, Eq, A>::clear()
{
	for (size_type i = 0; i < buckets.size(); ++i) {
		node* cur = buckets[i];
		while (cur != 0) {
			node* next = cur->next;
			delete_node(cur);
			cur = next;
		}
		buckets[i] = 0;
	}
	num_elements = 0;
}

template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
void hashtable<V, K, HF, Ex, Eq, A>::copy_from(const hashtable& ht)
{
	buckets.clear();
	buckets.reserve(ht.buckets.size());
	buckets.insert(buckets.end(), ht.buckets.size(), (node*) 0);
	__STL_TRY {
		for (size_type i = 0; i < ht.buckets.size(); ++i) {
			if (const node* cur = ht.buckets[i]) {
				node* copy = new_node(cur->val);
				buckets[i] = copy;
				
				for (node* next = cur->next; next; cur = next, next = cur->next) {
					copy->next = new_node(next->val);
					copy = copy->next;
				}
			}
		}
		num_elements = ht.num_elements;
	}
	__STL_UNWIND(clear());
}

#endif
