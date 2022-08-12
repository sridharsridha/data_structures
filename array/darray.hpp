#ifndef __DARRAY_HPP
#define __DARRAY_HPP

#include <new>
#include <initializer_list>
#include <vector>

template <class T>
class darray {
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef T* const const_pointer;

		// Access methods.
		//
		size_type size() const { return _size; }
		size_type capacity() const { return _cap; }
		size_type empty() const { return _size != 0; }
		const_pointer c_ptr() const { return _buf; }

		// Constructors.
		darray();
		darray( size_type n );
		darray( size_type n, const_reference v );
		darray( const_reference da );
		darray( value_type && da );
		darray( cosnt std::initializer_list<T> & ll );

		darray( size_t capacity, const_reference v) {
			_cap = capacity;
			_alloc();
			for( int i = 0; i < capacity; ++i ) {

			}

		}
		// Copy constructor.
		darray( const darray &da ) {
			_cap = da.capacity();
			_size = da.size();
			_buf = new T[ _cap ];
			memcpy( _buf, da.c_ptr(), sizeof( T ) * da.size() );
		}
		// Move constructor.
		darray( darray &&da ) {
			_cap = da._cap;
			_size = da._size;
			_buf = da._buf;
			da._cap = 2;
			da._size = 0;
			da._buf = nullptr;
		}
		// Initialization list.
		darray( std::initializer_list<T> ll ) {
		}
		~darray() { delete[] _buf; }

		// MODIFICATION METHODS.
		reference push_back( const_reference val ) {
			return emplace_back(val);
		}
		reference push_back( value_type&& val ) {
			return emplace_back( std::move( val ) );
		}
		template <typename... Args>
		reference emplace_back( Args&&... args ) {
			if ( _size == _cap ) _grow();
			return *new ( _buf + _size++ ) T( std::forward<Args>( args )... ); 
		}
		void pop_back() {
		}
		void insert( size_t pos, T val ) {
		}
		void erase( size_t pos ) {
		}
		void erase( size_t start, size_t end ) {
		}

	private:
		size_type _size   = 0;
		size_type _cap    = 2;
		pointer   _start  = nullptr;
		
		// Helpers
		void _grow( );
		void _alloc( size_type n );
};

darray::darray() {
	this->_cap = 0;
	this->_size = 0;
	this->_start = nullptr;
}
darray::darray( size_type n );
darray::darray( size_type n, const_reference v );
darray::darray( const_reference da );
darray::darray( value_type && da );
darray::darray( cosnt std::initializer_list<T> & ll );


void darray::_grow() {
	_cap *= 2;
	pointer tmp = _buf;
	std::memcpy( tmp, _buf, sizeof( T ) * _size );
	delete[] tmp;
}

void darray::_alloc( size_type n ) {
	this->_start = new value_type[ n ];
	this->_cap = n;
	this->_size = 0;
}

#endif




