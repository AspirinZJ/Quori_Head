#include "mTot.h"

float mTot(float m, float d, float rm, float R, float del)
{
    complex double m2 = cpow(m, 2), m3 = cpow(m, 3), m5 = cpow(m, 5);
    complex double d2 = cpow(d, 2), d3 = cpow(d, 3);
    complex double rm2 = cpow(rm, 2), rm4 = cpow(rm, 4);
    complex double R2 = cpow(R, 2), R3 = cpow(R, 3), R4 = cpow(R, 4);
    complex double del2 = cpow(del, 2), del3 = cpow(del, 3), del4 = cpow(del, 4);
    complex double m2p1 = m2 + 1, m2p12 = cpow(m2p1, 2), m2p13 = cpow(m2p1, 3);
    complex double Rddel = R + d - del, Rddel2 = cpow((Rddel), 2);
    complex double Rddelrm = Rddel + rm;
    complex double para1 = R * rm - 2 * R * del - 2 * d * del + d * rm - 2 * del * rm + 2 * del2 + 4 * R2 * m2 + 2 * del2 * m2 + d * m2 * rm - 2 * del * m2 * rm + 4 * R * d * m2 - 6 * R * del * m2 + 3 * R * m2 * rm - 2 * d * del * m2;
    complex double para2 = (8 * R * del3 + 8 * d * del3 - 4 * del4 - 4 * R2 * del2 - 16 * R4 * m2 + R2 * rm2 - 4 * d2 * del2 - 4 * del4 * m2 + d2 * rm2 + 4 * del2 * rm2 - 32 * R3 * d * m2 + 24 * R * del3 * m2 + 48 * R3 * del * m2 + 8 * d * del3 * m2 - 16 * R2 * d2 * m2 - 52 * R2 * del2 * m2 + 9 * R2 * m2 * rm2 - 4 * d2 * del2 * m2 + d2 * m2 * rm2 + 4 * del2 * m2 * rm2 - 8 * R * d * del2 + 2 * R * d * rm2 - 4 * R * del * rm2 - 4 * d * del * rm2 - 40 * R * d * del2 * m2 + 16 * R * d2 * del * m2 + 64 * R2 * d * del * m2 + 6 * R * d * m2 * rm2 - 12 * R * del * m2 * rm2 - 4 * d * del * m2 * rm2);
    complex double para3 = 2 * R * d - 2 * R * del - 2 * d * del + R2 + d2 - 3 * R2 * m2 + d2 * m2 + 2 * R * d * m2 + 2 * R * del * m2 - 2 * d * del * m2;
    complex double para4 = 18 * R4 * m * rm2 - 36 * R4 * m3 * rm2 - 54 * R4 * m5 * rm2 + 54 * R2 * d2 * m * rm2 + 36 * R * d3 * m3 * rm2 + 36 * R3 * d * m3 * rm2 + 18 * R * d3 * m5 * rm2 - 18 * R3 * d * m5 * rm2 + 36 * R2 * del2 * m * rm2 + 36 * R3 * del * m3 * rm2 + 90 * R3 * del * m5 * rm2 + 108 * R2 * d2 * m3 * rm2 + 54 * R2 * d2 * m5 * rm2 - 36 * R2 * del2 * m5 * rm2 + 18 * R * d3 * m * rm2 + 54 * R3 * d * m * rm2 - 54 * R3 * del * m * rm2 + 36 * R * d * del2 * m * rm2 - 54 * R * d2 * del * m * rm2 - 108 * R2 * d * del * m * rm2 + 72 * R * d * del2 * m3 * rm2 - 108 * R * d2 * del * m3 * rm2 - 144 * R2 * d * del * m3 * rm2 + 36 * R * d * del2 * m5 * rm2 - 54 * R * d2 * del * m5 * rm2 - 36 * R2 * d * del * m5 * rm2;
    complex double para10 = (m2p13 * cpow(para2, 3) + 108 * R2 * m2 * rm4 * m2p12 * Rddel2 * cpow(para3, 2));
    complex double para11 = csqrt(para10);
    complex double para5 = cpow((1.7320508075688 * para11 + para4), (1.0 / 3.0));
    complex double para6 = 4330209751607469.0 * para5;
    complex double para7 = 2.49809726143892e16 * m2p1 * para2;
    complex double para8 = 9007199254740992 * R * m * Rddelrm;
    complex double para9 = 36028797018963968 * R * m * Rddelrm * para5;
    complex double resultComplex = cpow(((6 * Rddel) / Rddelrm - (2 * Rddel) / Rddelrm + cpow(para1, 2) / (4 * R2 * m2 * cpow(Rddelrm, 2)) + para6 / para8 - para7 / para9), 0.5) * 0.5 - cpow(((2 * Rddel) / Rddelrm - ((2 * (2 * R * del + R * rm + 2 * d * del + d * rm - 2 * del * rm - 2 * del2 - 4 * R2 * m2 - 2 * del2 * m2 + d * m2 * rm - 2 * del * m2 * rm - 4 * R * d * m2 + 6 * R * del * m2 + 3 * R * m2 * rm + 2 * d * del * m2)) / (R * m * Rddelrm) + cpow(para1, 3) / (4 * R3 * m3 * cpow(Rddelrm, 3)) + (6 * (Rddel)*para1) / (R * m * cpow(Rddelrm, 2))) / cpow(((6 * Rddel) / Rddelrm - (2 * Rddel) / Rddelrm + cpow(para1, 2) / (4 * R2 * m2 * cpow(Rddelrm, 2)) + para6 / para8 - para7 / para9), 0.5) + (6 * Rddel) / Rddelrm + cpow(para1, 2) / (2 * R2 * m2 * cpow(Rddelrm, 2)) - para6 / para8 + para7 / para9), 0.5) * 0.5 - para1 / (4 * R * m * Rddelrm);
    float resultReal = creal(resultComplex);
    return resultReal;
}