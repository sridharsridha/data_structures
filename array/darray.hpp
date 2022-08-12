#ifndef __DARRAY_HPP
#define __DARRAY_HPP

#include <new>
#include <initializer_list>
#include <algorithm>

template < class T >
class darray {
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type* pointer;
		typedef value_type* const const_pointer;

	private:
		size_type _sz;  
		size_type _cap;   
		pointer   _start;
		
		void _grow();
		void _alloc( size_type n );

	public:
		// Access methods.
		size_type size() const { return _sz; }
		size_type capacity() const { return _cap; }
		bool empty() const { return _sz == 0; }
		void reserve( size_type n ) { _alloc( n ); };
		void shrink_to_fit();

		reference at( size_type n ) { return *( _start + n ); }
		const_reference at( size_type n ) const { return *( _start + n ); }
		reference operator[]( size_type n ) { return at( n ); }
		const_reference operator[]( size_type n ) const { return at( n ); }

		pointer data() { return _start; }
		const_pointer data() const { return _start; }

		reference front() { return *_start; };
		const_reference front() const { return *_start; };
		reference back() { return *( _start + _sz - 1 ); };
		const_reference back() const { return *( _start + _sz - 1 ); };

		// Constructors.
		darray();
		darray( size_type n );
		darray( size_type n, const_reference v );
		darray( const value_type & da );
		darray( value_type && da );
		darray( const std::initializer_list< value_type > &l );
		~darray();

		// Modification methods.
		template <typename... Args>
		reference emplace_back( Args&&... args );

		reference push_back( const_reference v ) {
			return emplace_back( v );
		}
		reference push_back( value_type&& v ) {
			return emplace_back( std::move( v ) );
		}
		void pop_back() { --_sz; }
		void insert( size_t pos, const_reference v );
		void insert( size_t pos, value_type&& v );
	  void erase( size_t pos );
		void erase( size_t start, size_t end );

		void clear();
		void resize( size_type sz );
    void resize( size_type sz, const_reference c );
		void swap( darray& );

};

// Constructors.
template < class T >
darray< T >::darray() {
	_cap = _sz = 0;
	_start = nullptr;
}

template < class T >
darray< T >::darray( size_type n ) {
	_alloc( n );
}

template < class T >
darray< T >::darray( size_type n, const_reference v ) {
	_alloc( n );
	for( int i = 0; i < n; ++i ) {
		emplace_back( v ); 
	}
}

template < class T >
darray< T >::darray( const value_type& da ) {
	_alloc( da._cap );
	std::move( da._start, da._start + da._sz, _start );
	_sz = da._sz;
}

template < class T >
darray< T >::darray( value_type&& da ) {
	_cap = std::move( da._cap );
	_sz = std::move( da._sz );
	_start = std::move( da._start );
}

template < class T >
darray< T >::darray( const std::initializer_list< value_type > & ll ) {
	for( auto v : ll ) {
		push_back( v );	
	}
}

template < class T >
darray< T >::~darray() {
	for ( int i = 0; i < _sz; ++i ) {
		_start[ i ].~value_type();
	}
	_sz = 0;
	_cap = 0;
	delete[] _start;
}

template < class T >
void darray< T >::shrink_to_fit() {
	pointer tmp = _start;
	_alloc( _sz );
	std::move( tmp, tmp + _sz, _start );
	delete [] tmp;
}

// MODIFICATION METHODS.
//

template < class T >
template < typename... Args >
typename darray< T >::reference
darray< T >::emplace_back( Args&&... args ) {
	if ( _sz == _cap ) _grow();
	return *new ( _start + _sz++ )
		value_type( std::forward<Args>( args )... ); 
}

template < class T >
void darray< T >::insert( size_t pos, const_reference v ) {
	if ( _sz == _cap ) _grow();
	for ( int i = _sz; i > pos; --i ) {
		_start[ i ] = std::move( _start[ i - 1 ] );
	}
	_start[ pos ] = v;
	_sz++;
}

template < class T >
void darray< T >::erase( size_t pos ) {
	assert( !empty() );
	_start[ pos ].~value_type();  
	std::move( _start + pos + 1, _start + pos + _sz, _start + pos );
	--_sz;
}

template < class T >
void darray< T >::erase( size_t start, size_t end ) {
	assert( !empty() && start >= 0 && end <= _sz );
	for ( int i = start; i < end; ++i ) {
		_start[ i ].~value_type();  
	}
	std::move( _start + end, _start + end + ( _sz - end ), _start + start );
	_sz -= ( end - start );
}

template < class T >
void darray< T >::clear() {
	for ( int i = 0; i < _sz; ++i ) {
		_start[ i ].~value_type();
	}
	_sz = 0;
}

template < class T >
void darray< T >::resize( size_type sz ) {
	if ( _sz < sz ) {
		for ( int i = 0; i < sz - _sz; ++i ) {
			push_back( value_type() );
		}
		_sz = sz;
	} else if ( _sz > sz ) {
		erase( sz, _sz );
	}
}


template < class T >
void darray< T >::swap( darray& da ) {
	std::swap( _cap, da._cap );
	std::swap( _sz, da._sz );
	std::swap( _start, da._start );
}


template < class T >
void darray< T >::_grow() {
	if ( !_start ) {
		_alloc( 2 );
	} else {
		pointer old_start = _start;
		size_type old_sz = _sz;
		_alloc( _cap * 2 );
		std::move( old_start, old_start + old_sz, _start );
		_sz = old_sz;
		delete [] old_start;
	}
}

template < class T >
void darray< T >::_alloc( size_type n ) {
	_start = new value_type[ n ];
	_cap = n;
	_sz = 0;
}

#endif




