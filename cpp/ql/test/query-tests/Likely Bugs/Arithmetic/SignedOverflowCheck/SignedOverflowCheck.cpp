// Signed-comparison tests

/* 1. Signed-signed comparison.  The semantics are undefined. */
bool cannotHoldAnother8(int n1) {
    // clang 8.0.0 -O2: deleted (silently)
    // gcc 9.2 -O2: deleted (silently)
    // msvc 19.22 /O2: not deleted
    return n1 + 8 < n1; // BAD
}

/* 2. Signed comparison with a narrower unsigned type.  The narrower
      type gets promoted to the (signed) larger type, and so the
      semantics are undefined. */
bool cannotHoldAnotherUShort(int n1, unsigned short delta) {
    // clang 8.0.0 -O2: deleted (silently)
    // gcc 9.2 -O2: deleted (silently)
    // msvc 19.22 /O2: not deleted
    return n1 + delta < n1; // BAD
}

/* 3. Signed comparison with a non-narrower unsigned type.  The
      signed type gets promoted to (a possibly wider) unsigned type,
      and the resulting comparison is unsigned. */
bool cannotHoldAnotherUInt(int n1, unsigned int delta) {
    // clang 8.0.0 -O2: not deleted
    // gcc 9.2 -O2: not deleted
    // msvc 19.22 /O2: not deleted
    return n1 + delta < n1; // GOOD
}

bool shortShort1(unsigned short n1, unsigned short delta) {
    // clang 8.0.0 -O2: deleted
    // gcc 9.2 -O2: deleted
    // msvc 19.22 /O2: not deleted
	return n1 + delta < n1; // BAD
}

bool shortShort2(unsigned short n1, unsigned short delta) {
    // clang 8.0.0 -O2: not deleted
    // gcc 9.2 -O2: not deleted
    // msvc 19.22 /O2: not deleted
	return (unsigned short)(n1 + delta) < n1; // GOOD
}

/* Distinguish `varname` from `ptr->varname` and `obj.varname` */
struct N {
	unsigned short n1;
} n, *np;

bool shortStruct1(unsigned short n1, unsigned short delta) {
	return np->n1 + delta < n1; // GOOD
}

bool shortStruct1a(unsigned short n1, unsigned short delta) {
	return n1 + delta < n.n1; // GOOD
}

bool shortStruct2(unsigned short n1, unsigned short delta) {
	return (unsigned short)(n1 + delta) < n.n1; // GOOD
}

struct se {
        short xPos;
        short yPos;
        short xSize;
        short ySize;
};

extern se *getSo(void);

bool func1(se *so) {
	se *o = getSo();
	if (so->xPos + so->xSize < o->xPos // GOOD
	    || so->xPos > o->xPos + o->xSize) { // GOOD
    	// clang 8.0.0 -O2: not deleted
    	// gcc 9.2 -O2: not deleted
    	// msvc 19.22 /O2: not deleted
		return false;
	}
	return true;
}
