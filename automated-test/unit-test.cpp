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
		assertInt("t2 isClear", t2.isClear(), 0);
		assertInt("t2 mcc", t2.getMcc(), 310);
		assertInt("t2 mnc", t2.getMnc(), 410);
		assertStr("t2", t2.toString(), "310410");

		PLMN_RK::MccMnc t2b = t2;
		assertInt("t2b mcc", t2b.getMcc(), 310);
		assertInt("t2b mnc", t2b.getMnc(), 410);
		assertStr("t2b", t2b.toString(), "310410");

		PLMN_RK::MccMnc t2c(t2);
		assertInt("t2c mcc", t2c.getMcc(), 310);
		assertInt("t2c mnc", t2c.getMnc(), 410);

		PLMN_RK::MccMnc t2d("310410");
		assertInt("t2d mcc", t2d.getMcc(), 310);
		assertInt("t2d mnc", t2d.getMnc(), 410);
		assertInt("t2d", t2d == t2, 1);
	
	}
	// These examples are from https://forum.sierrawireless.com/t/creating-a-forbidden-list-of-networks-in-sim/6116
	{
		PLMN_RK::MccMnc t3;
		t3.withMcc(404).withMnc(44).withTwoDigitMnc();
		assertInt("t3 mcc", t3.getMcc(), 404);
		assertInt("t3 mnc", t3.getMnc(), 44);
		assertStr("t3", t3.toString(), "40444");

		PLMN_RK::MccMnc t3b("40444");
		assertInt("t3b mcc", t3b.getMcc(), 404);
		assertInt("t3b mnc", t3b.getMnc(), 44);
		assertInt("t3b", t3b == t3, 1);

		assertStr("t3", t3.toPLMN(), "04F444");


		// 04F444 -> 40444 -> Spice telecomm
	}
	{
		PLMN_RK::MccMnc t4(405, 803);
		assertInt("t4 mcc", t4.getMcc(), 405);
		assertInt("t4 mnc", t4.getMnc(), 803);
		assertStr("t4", t4.toString(), "405803");


		PLMN_RK::MccMnc t4b("405803");
		assertInt("t4b mcc", t4b.getMcc(), 405);
		assertInt("t4b mnc", t4b.getMnc(), 803);
		assertInt("t4b", t4b == t4, 1);

		assertStr("t4", t4.toPLMN(), "043508");

		// 043508 -> 405803 -> Airtel
	}
	{
		PLMN_RK::MccMnc t5(405, 820);
		assertInt("t5 mcc", t5.getMcc(), 405);
		assertInt("t5 mnc", t5.getMnc(), 820);
		assertStr("t5", t5.toString(), "405820");

		PLMN_RK::MccMnc t5b("405820");
		assertInt("t5b mcc", t5b.getMcc(), 405);
		assertInt("t5b mnc", t5b.getMnc(), 820);
		assertInt("t5b", t5b == t5, 1);

		assertStr("t5", t5.toPLMN(), "040528");
		// 040528 - > 405820 -> uninor
	}
	{
		PLMN_RK::MccMnc t6(405, 34);
		assertInt("t6 mcc", t6.getMcc(), 405);
		assertInt("t6 mnc", t6.getMnc(), 34);
		assertStr("t6", t6.toString(), "405034");

		PLMN_RK::MccMnc t6b("405034");
		assertInt("t6b mcc", t6b.getMcc(), 405);
		assertInt("t6b mnc", t6b.getMnc(), 34);
		assertInt("t6b", t6b == t6, 1);

		// 044530 -> 405034 -> unknown
		assertStr("t6", t6.toPLMN(), "044530");
	}
}


int main(int argc, char *argv[]) {
	testMccMnc();

	return 0;
}
