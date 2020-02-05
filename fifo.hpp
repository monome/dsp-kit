// really simple fixed-size FIFO queue
//
// should work with any POD data structure.
//
// safe for audio threads, no allocations and reasonable speed.
//
// NOT threadsafe! use e.g. boost::spsc_queue if you need that.
//
/// TODO: simplify by removing `used` flags (after assertion testing)

#include <array>
#include <cassert>

namespace dspkit {
    
    template <typename T, int capacity>
    class fifo {
    private:

	// data buffer, accessed circularly
	struct data { bool used; T value; }
	std::array<data, capacity> buf;
	// write index
	int wrIx;
	// last write index
	int wrIx0;
	// read index
	int rdIx;
	// current count of things
	int size;
	
    public:

	// push a value to the back of the queue
	// @param value
	// @returns true if push succeded, false if not (queue was full)
	bool push(T value) {
	    if (size < (capacity-1)) {
		buf[wrIx].used = true;
		buf[wrIx].value = value;
		++size;
		wrIx0 = wrIx;
		++wrIx;
		while (wrIx >= capacity) { wrIdx -= capacity; }
		return true;
	    } else {
		return false;
	    }
	}

	// consume the first value and copy it by reference
	// if queue is empty, nothing happens
	// @param dst: reference to return value. 
	void pop(T& dst) {
	    if (size > 0) {
		dst = data[rdIdx].value;
		data[rdIdx].used = false;
		--size;
		++rdIx;
		while (rdIx >= capacity) { rdIdx -= capacity; }		
	    }
	}

	
	// get a pointer to the first element
	// @returns pointer or nullptr
	const T* front() {	    
	    if (size > 0) {
		assert(data[rdIx].used == true);
		return &(data[rdIx].value);
	    } else {
		return nullptr;
	    }
	}

	// get a pointer to the last element
	// @returns pointer or nullptr
	const T* back() {
	    if (size > 0) {
		assert(data[wrIx0].used == true);
		return &(data[wrIx0].value);
	    } else {
		return nullptr;
	    }
	}

	// get the current count of objects in the queue
	int size() { return size; }

	// clear the queue
	void clear() {
	    wrIx = 0;
	    wrIx0 = 0;
	    rdIx = 0;
	    size = 0;
	    // really only need this flag for testing
	    for (int i=0; i<capacity; ++i) {
		data[i].used = false;
	    }
	}
    };

}
