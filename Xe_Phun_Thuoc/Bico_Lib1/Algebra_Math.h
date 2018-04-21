#ifndef ALGEBRA_MATH_H
#define ALGEBRA_MATH_H

extern signed long get_abs(signed long num);
extern signed long orderNumber(signed long num, unsigned char order); //Ham luy thua, num có the am hoac duong, order khong duoc am
extern unsigned char getNumberOfDigit(signed long num); //Ham lay so chu so cua mot so, VD: getNumberOfDigit(1425) se tra ve ket qua la 4
extern unsigned char getDigitInNumber(signed long num, unsigned char stand); //stand - vi tri cua so can lay, lay mot chu so trong mot so, VD: getDigitInNumber(14578, 2) se tra ve ket qua la 5, num có the am hoac duong, stand khong duoc am
extern void seperateDigit(signed long num, unsigned char* storage, unsigned char storage_size); //tach cac chu so cua mot so va luu vao mang storage, VD: seperateDigit(452, a) thi cac phan tu cua a luc nay la a[0]=2, a[1]=5, a[2]=4

#endif
