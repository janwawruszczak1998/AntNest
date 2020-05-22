#ifndef NC2DARRAY_INCLUDE
#define NC2DARRAY_INCLUDE

#include <vector>
#include <cstdlib>

// Basically a 2D array of chars made with std::vector.
class NC2DArray 
	{
	public:
		// Creates 2D array with `width` and `height`.
		NC2DArray(int width, int height);
		~NC2DArray();

		// Returns element at `x` `y`.
		char at(int x, int y) const;

		// Sets `value` of element at `x` `y`.
		void set (int x, int y, char value);


		// Width size of the array.
		int width () const;


		// Height size of the array.
		int height () const;


	private:
		// Internal vector storage of chars.
		std::vector < std::vector < char > > array;
	};
	
#endif 