//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <random>
//using namespace std;
//
//const double prob_scopechange = 0.20;
//const double enter_bias = 0.54;
//const double prob_declare_vs_use = 0.20;
//const double prob_undeclared = 0.015;
//const double prob_dupdeclare = 0.015;
//const double prob_default_id_len = 0.90;
//const int default_id_len = 4;
//const int max_id_len = 25;
//
//random_device arandomdevice;
//mt19937 generator(arandomdevice());
//
//bool truewithprob(double p)
//{
//	uniform_real_distribution<> distro(0, 1);
//	return distro(generator) < p;
//}
//
//int randint(int n)
//{
//	uniform_int_distribution<> distro(0, n - 1);
//	return distro(generator);
//}
//
//int main()
//{
//	cout << "enter output file name: ";
//	string filename;
//	getline(cin, filename);
//	cout << "about how many test file lines should i generate? ";
//	int nlines;
//	cin >> nlines;
//	ofstream outf(filename.c_str());
//	if (!outf)
//	{
//		cout << "cannot create " << filename << endl;
//		return 1;
//	}
//
//	int nestinglevel = 0;
//	vector<string> ids;
//	for (int n = 1; n <= nlines; n++)
//	{
//		if (truewithprob(prob_scopechange))
//		{
//			if (nestinglevel > 0 &&
//				truewithprob(n < nlines / 2 ? (1 - enter_bias) : enter_bias))
//			{
//				outf << "}\n";
//				nestinglevel--;
//			}
//			else
//			{
//				outf << "{\n";
//				nestinglevel++;
//			}
//		}
//		else if (truewithprob(prob_declare_vs_use) || ids.empty())
//		{
//			static char name[max_id_len + 1];
//			int len = default_id_len;
//			if (!truewithprob(prob_default_id_len))
//				len = 1 + randint(max_id_len);
//			for (int k = 0; k < len; k++)
//			{
//				int r = randint(2 * 26);
//				name[k] = (r < 26 ? 'a' + r : 'a' + r - 26);
//			}
//			name[len] = '\0';
//			ids.push_back(name);
//			outf << name;
//			if (!truewithprob(prob_undeclared))
//				outf << ' ' << n;
//			outf << endl;
//		}
//		else if (truewithprob(prob_dupdeclare))
//			outf << ids[randint(ids.size())] << ' ' << n << endl;
//		else
//			outf << ids[randint(ids.size())] << endl;
//	}
//	for (; nestinglevel > 0; nestinglevel--)
//		outf << "}\n";
//}
//
