#include <stdio.h>
#include <stdlib.h>

#include "time.h"

#define TIME_TO_LONG(t) ((t).time << 16 | (t).parts)

void printHeader(const char * name) {
	printf("===========================================================\n");
	printf("== %s\n", name);
	printf("===========================================================\n");
}

void testAdd() {

	struct Time a, b, c, d;
	long la, lb, lc, ld;

	printHeader("Add test");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;

	/**
	 * TEST 1
	 */
	a.time = 345;
	a.parts = 60000;

	b.time = 234;
	b.parts = 60000;

	timeAdd(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la + lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}
	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;

	/**
	 * TEST 2
	 */
	a.time = 0;
	a.parts = 60000;

	b.time = 0;
	b.parts = 60000;

	timeAdd(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la + lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}
	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;
	/**
	 * TEST 3
	 */
	a.time = 1;
	a.parts = 100;

	b.time = 1;
	b.parts = 100;

	timeAdd(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la + lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}

	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;
	/**
	 * TEST 4
	 */
	a.time = 0;
	a.parts = 100;

	b.time = 0;
	b.parts = 100;

	timeAdd(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la + lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}

	printf("\n");
}

void testSub() {

	struct Time a, b, c, d;
	long la, lb, lc, ld;

	printHeader("Sub test");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;

	/**
	 * TEST 1
	 */
	a.time = 345;
	a.parts = 60000;

	b.time = 234;
	b.parts = 60000;

	timeSub(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la - lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}
	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;

	/**
	 * TEST 2
	 */
	a.time = 0;
	a.parts = 60000;

	b.time = 0;
	b.parts = 60000;

	timeSub(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la - lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}
	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;
	/**
	 * TEST 3
	 */
	a.time = 1;
	a.parts = 100;

	b.time = 1;
	b.parts = 100;

	timeSub(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la - lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}

	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;
	/**
	 * TEST 4
	 */
	a.time = 100;
	a.parts = 0;

	b.time = 2;
	b.parts = 13;

	timeSub(&a, &b, &c);

	la = TIME_TO_LONG(a);
	lb = TIME_TO_LONG(b);

	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la - lb == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}

	printf("\n");
}

void testDiff() {

	struct Time a, b, c, d;
	long la, lb, lc, ld;

	printHeader("Diff test");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;

	/**
	 * TEST 1
	 */
	a.time = 345;
	a.parts = 60000;

	timeDiff(&a, 2, &c);

	la = TIME_TO_LONG(a);
	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la / 2 == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}
	printf("\n");

	/**
	 * CLEAR DATA
	 */
	a.time = 0;
	a.parts = 0;
	b.time = 0;
	b.parts = 0;
	c.time = 0;
	c.parts = 0;
	d.time = 0;
	d.parts = 0;

	la = 0;
	lb = 0;
	lc = 0;
	ld = 0;

	/**
	 * TEST 2
	 */
	a.time = 30;
	a.parts = 60;

	timeDiff(&a, 2, &c);

	la = TIME_TO_LONG(a);
	lc = TIME_TO_LONG(c);

	printf("a=%li, b=%li, c=%li, d=%li\n", la, lb, lc, ld);

	if (la / 2 == lc) {
		printf("Test Line %i OK\n", __LINE__);
	} else {
		printf("Test Line %i failed\n", __LINE__);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	testAdd();
	testSub();
	testDiff();

	return 0;
}
