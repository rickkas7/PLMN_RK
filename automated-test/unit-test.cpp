#include "Particle.h"
#include "PLMN_RK.h"


#define assertInt(msg, got, expected) _assertInt(msg, got, expected, __LINE__)
void _assertInt(const char *msg, int got, int expected, int line) {
	if (expected != got) {
		printf("assertion failed %s line %d\n", msg, line);
		printf("expected: %d\n", expected);
		printf("     got: %d\n", got);
		assert(false);
	}
}

#define assertStr(msg, got, expected) _assertStr(msg, got, expected, __LINE__)
void _assertStr(const char *msg, const char *got, const char *expected, int line) {
	if (strcmp(expected, got) != 0) {
		printf("assertion failed %s line %d\n", msg, line);
		printf("expected: %s\n", expected);
		printf("     got: %s\n", got);
		assert(false);
	}
}

void testMccMnc() {
	{
		PLMN_RK::MccMnc t1;
		assertInt("t1 isClear", t1.isClear(), 1);
	}
	{
		PLMN_RK::MccMnc t2(310, 410);
		assertInt("t2 mcc", t2.getMcc(), 310);
		assertInt("t2 mnc", t2.getMnc(),410);

		PLMN_RK::MccMnc t2b = t2;
		assertInt("t2b mcc", t2b.getMcc(), 310);
		assertInt("t2b mnc", t2b.getMnc(),410);

		PLMN_RK::MccMnc t2c(t2);
		assertInt("t2c mcc", t2c.getMcc(), 310);
		assertInt("t2c mnc", t2c.getMnc(),410);

	}
}


int main(int argc, char *argv[]) {
	testMccMnc();

	return 0;
}
