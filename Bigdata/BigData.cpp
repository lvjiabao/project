#include"BigData.h"


void FunTest()
{
	BigData f("-9223372036854775808");
	cout << f << endl;
	BigData f0("+9223372036854775807");
	cout << f0 << endl;
	BigData f1("1234567890");
	cout << f1 << endl;
	BigData f2("00001234567890");
	cout << f2 << endl;
	BigData f3("+1234567890");
	cout << f3 << endl;
	BigData f4("-1234567890");
	cout << f4 << endl;
	BigData f5("12345aaa67890");
	cout << f5 << endl;
	BigData f6("aaa1234567890");
	cout << f6 << endl;
	BigData f7("123456789123456789123456789123456789");
	cout << f7 << endl;
	BigData f8("-123456789123456789123456789123456789");
	cout << f8 << endl;
}

void FunTest1()
{
	//Í¬ºÅÕý
	cout << "123456789012345678901234567890\n";
	BigData f1("5555555555555555555555");
	BigData f2("5555555555555555555555");
	cout << f1 + f2<<endl;

	BigData f3("5555555");//
	BigData f4("5555555");
	cout << f3 + f4<<endl;

	BigData f5("5555555555555555555555");
	BigData f6("5555555");
	cout << f5 + f6<<endl;

	BigData f7("5555555");
	BigData f8("5555555555555555555555");
	cout << f7 + f8 << endl;

	BigData s1("-9223372036854775807");
	BigData s2("-1");
	cout << s1 + s2 << endl;

	BigData s3("9223372036854775807");
	BigData s4("1");
	cout << s3 + s4 << endl;

	BigData s5("9223372036854775807");
	BigData s6("-9223372036854775807");
	cout << s5 + s6 << endl;

}

void FunTest2()
{
	cout << "123456789012345678901234567890\n";
	BigData f1("-9223372036854775808");
	BigData f2("1");
	cout << f1 - f2 << endl;

	BigData f3("9223372036854775807");
	BigData f4("-1");
	cout << f3 - f4 << endl;

	BigData f5("-9223372036854775808");
	BigData f6("-1");
	cout << f5 - f6 << endl;

	BigData f7("9223372036854775807");
	BigData f8("1");
	cout << f7 - f8 << endl;

	BigData s1("-222222222222222222222222");
	BigData s2("-333333333333");
	cout << s1 - s2 << endl;
	cout << s2 - s1 << endl;

	BigData s3("222222222222222222222222");
	BigData s4("333333333333");
	cout << s3 - s4 << endl;
	cout << s4 - s3 << endl;
}

void FunTest3()
{
	cout << "123456789012345678901234567890\n";
	BigData f1("1234567890123456789");
	BigData f2("-1");
	cout << f1 * f2 << endl;

	BigData f3("-1234567890123456789");
	BigData f4("-2");
	cout << f3 * f4 << endl;

	BigData f5("-1111111111111111111");
	BigData f6("-11");
	cout << f5 * f6 << endl;
	cout << f6 * f5 << endl;

	BigData f7("2222222222222222222");
	cout << "2222222222222222222" << endl;
	BigData f8("5");
	cout << f7 * f8 << endl;

	BigData s1("2222222222222222222");
	BigData s2("-55");
	cout << s1 * s2 << endl;
	cout << s2 * s1 << endl;
}


void TestDiv()
{
	cout << "123456789012345678901234567890\n";
	cout << "24682468246824682468"<<endl;
	cout << "2\n";
	BigData f1("24682468246824682468");
	BigData f2("2");
	cout << f1 / f2 << endl<<endl;

	cout << "22222222222222222222" << endl;
	BigData f3("22222222222222222222");
	cout << "4\n";
	BigData f4("4");
	cout << f3 / f4 << endl<<endl;

	cout << "22222222222222222222" << endl;
	cout << "2\n";
	BigData f5("22222222222222222222");
	BigData f6("2");
	cout << f5 / f6 << endl << endl;

	cout << "33333333333333333333" << endl;
	cout << "33\n";
	BigData f7("33333333333333333333");
	BigData f8("33");
	cout << f7 / f8 << endl << endl;

	cout << "456153335481111111111113" << endl;
	cout << "-456153335481111111111114\n";
	BigData s1("456153335481111111111113");
	BigData s2("-456153335481111111111114");
	cout << s1 / s2 << endl << endl;

	cout << "33333333333333333333" << endl;
	cout << "333\n";
	BigData s3("33333333333333333333");
	BigData s4("333");
	cout << s3 / s4 << endl << endl;

	cout << "22222222222222222222" << endl;
	cout << "40\n";
	BigData s5("22222222222222222222");
	BigData s6("40");
	cout << s5 / s6 << endl << endl;

	cout << "22222222222222222222" << endl; 
	cout << "44\n";
	BigData s7("22222222222222222222");
	BigData s8("44");
	cout << s7 / s8 << endl << endl;

	cout << "22222222222222222222" << endl;
	cout << "44\n";
	BigData x1("22222222222222222222");
	BigData x2(-44);
	cout << x1 / x2 << endl << endl;

	

	
}

int main()
{
	//FunTest();
	//FunTest1();
	//FunTest2();
	//FunTest3();
	////TestDiv();
	////BigData a(-2222223456);
	////cout << a;
	BigData a("0");
	cout << a;
	return 0;
}