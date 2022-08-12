#include <iostream>
#include "darray.hpp"

using namespace std;

template <typename T>
void print( darray<T> &a ) {
	for ( int i = 0; i < a.size(); ++i ) {
		cout << a[ i ] << " ";
	}
	cout << endl;
}

int main() {
	darray<int> a( 6, 2 );
	darray<string> b( 2, "hello" );
	a.push_back( 10 );
	a.push_back( 29 );

	print( a );
	print( b );
	a.erase( 7 );
	print( a );
	a.erase( 4, 5 );
	print( a );
	a.resize( 4 );
	print( a );
	return 0;
}
