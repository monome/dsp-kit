namespace dspkit { 
    // mega simple lookup table class
    template<typename T>
    class Lut {
    public:
	// look up a value from a table with normalized position
	// warning: no bounds checking on position!
	// must be in [0, 1] or you get garbage.
	static T lookupLinear(float x, const T* tab, unsigned int size) {
	    const unsigned int size_1 = size-1;
	    const float fidx = x * size_1;
	    const unsigned int idx = static_cast<unsigned int>(fidx);
	    if (idx >= size_1) {
		return tab[size_1];
	    }
	    const float a = tab[idx];
	    const float b = tab[idx+1];
	    const float f = fidx - static_cast<float>(idx);
	    return a + (b-a)*f;
	}
    };

}
