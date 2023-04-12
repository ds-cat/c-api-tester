#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <chrono>
#include <thread>
#include <random>


using namespace std;
using namespace std::chrono;


int sample_size=5;
int run_size=1000;

vector<int> cluster_sizes;
vector<int> cluster_temp;
vector<int>  cluster_pro;
vector<vector<string>> content;
int n = 0;
int max_cluster = 0;
int temp;
int templist;
vector<int> ids;

string k = "24";
string inputfilename = "clustered_sentances-" + k + ".csv";
//string inputfilename = "clustered_faces" + k + ".csv";
void Cluster_Prop_Assign(string filename)
{
	string line;
	string word;
	vector<string>row;
	vector<vector<string>>content;
	//read in csv
	fstream file(filename, ios::in);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();

			stringstream str(line);

			while (getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else
		cout << "Could not open the file\n";
	for (int i = 0; i < content.size(); i++)
	{
		for (int j = 0; j < content[i].size()-1; j++)
		{
			if(i%2 == 1)
				if (stoi(content[i][j + 1]) > max_cluster)
				{
					max_cluster = stoi(content[i][j + 1]);
				}
				cluster_temp.push_back(stoi(content[i][j+1]));
		}
		
	}
	for (int i = 0; i <= max_cluster; ++i)
	{
		cluster_sizes.push_back(count(cluster_temp.begin(), cluster_temp.end(), i));
	}

}

void Cluster_Prop_Assign_Face(vector<vector<string>> content)
{
	for (int i = 0; i < content.size(); i++)
	{
		for (int j = 0; j < content[i].size() - 1; j++)
		{
			if (i % 2 == 1)
				if (stoi(content[i][j + 1]) > max_cluster)
				{
					max_cluster = stoi(content[i][j + 1]);
				}
			cluster_temp.push_back(stoi(content[i][j + 1]));
		}

	}
	for (int i = 0; i <= max_cluster; ++i)
	{
		cluster_sizes.push_back(count(cluster_temp.begin(), cluster_temp.end(), i));
	}
}
uint64_t timeSinceEpochMillisec() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}



vector<int>  Cluster_Samples_ID(vector<int>sizes, vector<int>temp, float sample)
{
	vector<int>id;
	int randhold;
	float prop = sample / 2000;
	transform(sizes.begin(), sizes.end(), sizes.begin(), [prop](int& c) { return c * prop; });
	while (accumulate(sizes.begin(), sizes.end(), 0)< sample)
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib(0, max_cluster);
		//cout << distrib(gen) << endl;
		++sizes.at(distrib(gen));
	}
	int zed = 0;
	//cout << accumulate(sizes.begin(), sizes.end(), 0) << endl;
	while (accumulate(sizes.begin(), sizes.end(),0) >0)
	{
		
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib2000(0, 2000);
		randhold = distrib2000(gen);
		//cout << randhold << endl;
		if (count(id.begin(), id.end(), randhold) == 0)
		{
			if (randhold <= 1999 && randhold >=0)
			{
				zed = sizes.at(temp.at(randhold));
				if (zed > 0)
				{

					id.push_back(randhold);
					//cout << sizes.at(temp.at(randhold)) << endl;
					sizes.at(temp.at(randhold)) = sizes.at(temp.at(randhold)) - 1;
					//cout << sizes.at(temp.at(randhold)) << endl;
					//cout << accumulate(sizes.begin(), sizes.end(), 0) << endl;
				}
				else
				{
					//cout << "ding " << endl;
				}
			}
		}
		//cout << zed << endl;
		
		//cout << accumulate(sizes.begin(), sizes.end(), 0) <<endl;
	}
	//out << accumulate(sizes.begin(), sizes.end(), 0);
	return id;
}

void getcontent(vector<vector<string>>& content)
{
	vector<string> stringtemp;
	string line;
	string word;
	vector<string>row;
	fstream file("dataset_results.csv", ios::in);
	if (file.is_open())
	{
		int x = 0;
		int fixer = 0;
		while (getline(file, line))
		{

			row.clear();
			//cout << line << endl;
			stringstream str(line);

			while (getline(str, word, ','))
			{
				++x;
				//cout << word << endl;
				word.erase(remove(word.begin(), word.end(), '"'), word.end());
				word.erase(remove(word.begin(), word.end(), '\n'), word.cend());
				if (word != "")
				{
					//cout << word << endl;
					if (fixer < 5)
					{

						stringtemp.push_back(word);
						++fixer;
					}
					else
					{
						content.push_back(stringtemp);
						stringtemp.clear();
						stringtemp.push_back(word);
						fixer = 1;
					}
				}
				if (content.size() == 1999)
				{
					if (stringtemp.size() == 5)
					{
						content.push_back(stringtemp);
					}
				}

			}



		}
		//cout << content.at(1999).at(0) << endl;
	}
	else
		cout << "Could not open dataset results\n";

}

void API_acc(vector<int> id, float &one, float &two, float &three, vector<vector<string>> content)
{
	//text,dataset,fyhao api,twinword api,symanto api
	vector<string> stringtemp;
	string line;
	string word;
	vector<string>row;
	//read in csv
	/*
	fstream file("dataset_results.csv", ios::in);
	if (file.is_open())
	{
		int x = 0;
		int fixer = 0;
		while (getline(file, line))
		{
			
			row.clear();
			//cout << line << endl;
			stringstream str(line);
			
			while (getline(str, word, ','))
			{
				++x;
				//cout << word << endl;
				word.erase(remove(word.begin(), word.end(), '"'), word.end());
				word.erase(remove(word.begin(), word.end(), '\n'), word.cend());
				if (word != "")
				{
					//cout << word << endl;
					if (fixer < 5)
					{
						
						stringtemp.push_back(word);
						++fixer;
					}
					else
					{
						content.push_back(stringtemp);
						stringtemp.clear();
						stringtemp.push_back(word);
						fixer = 1;
					}
				}
				if (content.size() == 1999)
				{
					if (stringtemp.size() == 5)
					{
						content.push_back(stringtemp);
					}
				}
				
			}
			
			
				
		}
		//cout << content.at(1999).at(0) << endl;
	}
	else
		cout << "Could not open dataset results\n";
		*/
	for (int i = 0; i < id.size(); ++i)
	{

		string GT;
		string check;
		
		
		GT = content.at(id.at(i)).at(1);
		check = content.at(id.at(i)).at(2);
		if (GT == check)
			++one;
		check = content.at(id.at(i)).at(3);
		if (GT == check)
			++two;
		check = content.at(id.at(i)).at(4);
		if (GT == check)
			++three;
	}
	int trueidsize = id.size();

	one = one / trueidsize;

	two = two / trueidsize;

	three = three / trueidsize;
}
void API_acc_face(vector<int> id, vector<vector<string>> content, float& one, float& two, float& three)
{
	one = 0;
	two = 0;
	three = 0;
	for (int i = 0; i < id.size(); ++i)
	{

		string GT;
		string check;


		GT = content.at(id.at(i)).at(2);
		check = content.at(id.at(i)).at(3);
		if (GT == check)
			++one;
		check = content.at(id.at(i)).at(4);
		if (GT == check)
			++two;
		check = content.at(id.at(i)).at(5);
		if (GT == check)
			++three;
	}
	int trueidsize = id.size();

	one = one / trueidsize;

	two = two / trueidsize;

	three = three / trueidsize;
}

vector<int> SRS_Samples_ID(int size)
{
	vector<int> temp;
	int i = 0;
	while ( i < size)
	{
		int randhold;
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib2000(0, 1999);
		randhold = distrib2000(gen);

			temp.push_back(randhold);
			++i;
	}
	return temp;


}
template<typename T>
double getAverage(std::vector<T> const& v) {
	if (v.empty()) {
		return 0;
	}
	return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

vector<vector<string>> remove_missing(vector<vector<string>> clust_list, vector<vector<string>> data_list)
{
	vector<string>clust_temp;
	vector<string>clust_temp2;
	for (int i = 0; i < clust_list.size(); ++i)
	{
		clust_temp.push_back(clust_list.at(i).at(0));
	}
	for (int i = 0; i < clust_list.size(); ++i)
	{
		clust_temp2.push_back(clust_list.at(i).at(1));
	}
	vector<vector<string>> output;
	for (int i = 0; i < data_list.size(); ++i)
	{
		if (count(clust_temp.begin(), clust_temp.end(), data_list.at(i).at(0)) && !count(clust_temp2.begin(), clust_temp2.end(), data_list.at(i).at(0)))//look to see if file name is in the list of 1st files
		{
			output.push_back(clust_list.at(i)); //if found store in output list
		}
		

	}
	return output;
}

vector<vector<string>>clust_dig(string filename)
{
	string line;
	string word;
	vector<string>row;
	vector<vector<string>>content;
	//read in csv
	fstream file(filename, ios::in);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();

			stringstream str(line);

			while (getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else
	{
		cout << filename << "not found";
	}
	return content;
}

int main()
{


		//remove files that are not first in csv
		vector<vector<string>>data_list;
		vector<vector<string>>clust_list;
		clust_list = clust_dig(inputfilename);
		//data_list = clust_dig("Untitled spreadsheet - face_input.csv");
		//clust_list = remove_missing(clust_list, data_list);


		std::cin >> sample_size;
		
		//cout << "runs size:";
		//cin >> run_size;

		vector<float> API_1_acc_avg;
		vector<float> API_2_acc_avg;
		vector<float> API_3_acc_avg;
		vector<float> API_1_SRS_avg;
		vector<float> API_2_SRS_avg;
		vector<float> API_3_SRS_avg;
		//get cluster proptions
		Cluster_Prop_Assign(inputfilename); //sent
		//Cluster_Prop_Assign_Face(clust_list); //img
		float API_1_GT = 0;
		float API_2_GT = 0;
		float API_3_GT = 0;
		vector<int> gt_list;
		for (int i = 0; i < 2000; ++i)
			gt_list.push_back(i);
		getcontent(content);
		API_acc(gt_list, API_1_GT, API_2_GT, API_3_GT, content); //sent
		//API_acc_face(gt_list, data_list, API_1_GT, API_2_GT, API_3_GT); // img


		//store results
		vector<float>API_1_res;
		vector<float>API_2_res;
		vector<float>API_3_res;
		vector<float>SRS_1_res;
		vector<float>SRS_2_res;
		vector<float>SRS_3_res;
		//for each run size
		for (int i = 0; i < run_size; ++i)
		{
			float API_1_acc = 0;
			float API_2_acc = 0;
			float API_3_acc = 0;
			float SRS_1_acc = 0;
			float SRS_2_acc = 0;
			float SRS_3_acc = 0;
			//get cluster samples
			ids = Cluster_Samples_ID(cluster_sizes, cluster_temp, sample_size);



			//get accuracy
			API_acc(ids, API_1_acc, API_2_acc, API_3_acc, content); //sent line
			//API_acc_face(ids, data_list, API_1_acc, API_2_acc, API_3_acc);//img line

			API_1_acc_avg.push_back(API_1_acc);
			API_2_acc_avg.push_back(API_2_acc);
			API_3_acc_avg.push_back(API_3_acc);
			API_1_res.push_back(API_1_acc - API_1_GT);
			API_2_res.push_back(API_2_acc - API_2_GT);
			API_3_res.push_back(API_3_acc - API_3_GT);
			ids.clear();

			ids = SRS_Samples_ID(sample_size);

			//cout << ids.at(1);
			API_acc(ids, SRS_1_acc, SRS_2_acc, SRS_3_acc, content);// sent line
			//cout << ids.size();
			//API_acc_face(ids, data_list, SRS_1_acc, SRS_2_acc, SRS_3_acc);//img line
			API_1_SRS_avg.push_back(SRS_1_acc);
			API_2_SRS_avg.push_back(SRS_2_acc);
			API_3_SRS_avg.push_back(SRS_3_acc);
			SRS_1_res.push_back(SRS_1_acc - API_1_GT);
			SRS_2_res.push_back(SRS_2_acc - API_2_GT);
			SRS_3_res.push_back(SRS_3_acc - API_3_GT);

			//cout << i << endl;
			ids.clear();
		}

		//cout << "sample:" << sample_size << endl;

		//cout << abs((getAverage(API_1_acc_avg)) - API_1_GT) << endl;
		/*
		cout << abs((getAverage(API_2_acc_avg)) - API_2_GT) << endl;
		cout << abs((getAverage(API_3_acc_avg)) - API_3_GT) << endl;
		cout << endl;
		cout << abs((getAverage(API_1_SRS_avg)) - API_1_GT) << endl;
		cout << abs((getAverage(API_2_SRS_avg)) - API_2_GT) << endl;
		cout << abs((getAverage(API_3_SRS_avg)) - API_3_GT) << endl;
		cout << endl;
		*/
		float count_one = 0;
		float count_two = 0;
		float count_three = 0;
		float srs_one = 0;
		float srs_two = 0;
		float srs_three = 0;
		for (int p = 0; p < API_1_acc_avg.size(); ++p)
		{
			count_one += (abs(API_1_acc_avg.at(p) - API_1_GT));
			count_two += (abs(API_2_acc_avg.at(p) - API_2_GT));
			count_three += (abs(API_3_acc_avg.at(p) - API_3_GT));
			srs_one += (abs(API_1_SRS_avg.at(p) - API_1_GT));
			srs_two += (abs(API_2_SRS_avg.at(p) - API_2_GT));
			srs_three += (abs(API_3_SRS_avg.at(p) - API_3_GT));
		}
		//cout << count_one / API_1_acc_avg.size() << endl;
		//cout << count_two / API_1_acc_avg.size() << endl;
		//cout << count_three / API_1_acc_avg.size() << endl;

		//cout << srs_one / API_1_acc_avg.size() << endl;
		//cout << srs_two / API_1_acc_avg.size() << endl;
		//cout << srs_three / API_1_acc_avg.size() << endl;
		cout << getAverage(API_1_acc_avg) << endl;
		cout << getAverage(API_2_acc_avg) << endl;
		cout << getAverage(API_3_acc_avg) << endl;
		//cout << srs_two / API_1_acc_avg.size() << endl;
		//cout << srs_three / API_1_acc_avg.size() << endl;

			cout << ((API_1_GT)) << endl;
		cout << ((API_2_GT)) << endl;
		cout << ((API_3_GT)) << endl;


		cout << endl;
		//cout << "API 1 GT diff Min: " << endl;
		// << *min_element(API_1_res.begin(), API_1_res.end()) << endl;
		//cout << "API 1 GT diff Max: " << endl;
		//cout << *max_element(API_1_res.begin(), API_1_res.end()) << endl;
		//cout << "API 1 GT diff Average: " << endl;
		//cout << abs(getAverage(API_1_res)) << endl;
		//cout << endl;
		//cout << "API 2 GT diff Min: " << endl;
		//cout << *min_element(API_2_res.begin(), API_2_res.end()) << endl;
		//cout << "API 2 GT diff Max: " << endl;
		//cout << *max_element(API_2_res.begin(), API_2_res.end()) << endl;
		//cout << "API 2 GT diff Average: " << endl;
		//cout << abs(getAverage(API_2_res)) << endl;
		//cout << endl;
		//cout << "API 3 GT diff Min: " << endl;
		//cout << *min_element(API_3_res.begin(), API_3_res.end()) << endl;
		//cout << "API 3 GT diff Max: " << endl;
		//cout << *max_element(API_3_res.begin(), API_3_res.end()) << endl;
		//cout << "API 3 GT diff Average: " << endl;
		//cout << abs(getAverage(API_3_res)) << endl;
		//cout << endl;
		//cout << "SRS 1 GT diff Min: " << endl;
		//cout << *min_element(SRS_1_res.begin(), SRS_1_res.end()) << endl;
		//cout << "SRS 1 GT diff Max: " << endl;
		//cout << *max_element(SRS_1_res.begin(), SRS_1_res.end()) << endl;
		//cout << "SRS 1 GT diff Average: " << endl;
		//cout << abs(getAverage(SRS_1_res)) << endl;
		//cout << endl;
		//cout << "SRS 2 GT diff Min: " << endl;
		//cout << *min_element(SRS_2_res.begin(), SRS_2_res.end()) << endl;
		//cout << "SRS 2 GT diff Max: " << endl;
		//cout << *max_element(SRS_2_res.begin(), SRS_2_res.end()) << endl;
		//cout << "SRS 2 GT diff Average: " << endl;
		//cout << abs(getAverage(SRS_2_res)) << endl;
		//cout << endl;
		//cout << "SRS 3 GT diff Min: " << endl;
		//cout << *min_element(SRS_3_res.begin(), SRS_3_res.end()) << endl;
		//cout << "SRS 3 GT diff Max: " << endl;
		//cout << *max_element(SRS_3_res.begin(), SRS_3_res.end()) << endl;
		//cout << "SRS 3 GT diff Average: " << endl;
		//cout << abs(getAverage(SRS_3_res)) << endl;
		//cout << endl;

	
}




