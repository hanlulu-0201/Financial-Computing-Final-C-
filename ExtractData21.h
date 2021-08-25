#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include<map>
#include "curl/curl.h"
#include "Stock.h"

using namespace std;


struct MemoryStruct {
	char* memory;
	size_t size;
	//char array
};

void* myrealloc(void* ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocting
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;
	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	//continue to allocate memory
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		sprintf_s(time, "%lld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}

int ExtractIWB(string startT, string endT, vector<double>& IWB, vector<string>& DATE)
{
	string startTime = getTimeinSeconds(startT+ "T16:00:00");
	string endTime = getTimeinSeconds(endT+ "T16:00:00");
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	// declaration of a pointer to an curl object
	CURL* handle;
	// result of the whole process
	CURLcode result;
	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);
	// curl_easy_init() returns a CURL easy handle
	handle = curl_easy_init();
	// if everything's all right with the easy handle...
	if (handle)
	{
		string sCookies, sCrumb;

		string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
		string symbol = "IWB";
		string urlB = "?period1=";
		string urlC = "&period2=";
		string urlD = "&interval=1d&events=history";
		string url = urlA + symbol + urlB + startTime + urlC + endTime;
		//convert to C 
		const char* cURL = url.c_str();
		curl_easy_setopt(handle, CURLOPT_URL, cURL);

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
		result = curl_easy_perform(handle);
		if (result != CURLE_OK)
		{
			// if errors have occurred, tell what is wrong with result
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result));
			return 1;
		}
		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		getline(sData, line);
		while (getline(sData, line)) {
			sDate = line.substr(0, line.find_first_of(','));
			//first common
			line.erase(line.find_last_of(','));
			sValue = line.substr(line.find_last_of(',') + 1);
			dValue = strtod(sValue.c_str(), NULL);//string to double
			//push back sDate and dValue to vectors Date and Value
			IWB.push_back(dValue);
			DATE.push_back(sDate);
			//cout << sDate << " " << std::fixed << ::setprecision(6)<< dValue << endl;
			//only keep dValue and sDate
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);
	// release resources acquired by curl_global_init()
	curl_global_cleanup();
	cout << "Extract IWB Done" << endl;
	return 0;
}


int ExtractStock(map<string, Stock>& StockList)
{
	int count = 0;
	int length = StockList.size();
	//string startTime = getTimeinSeconds("2021-01-01T16:00:00");
	//string endTime = getTimeinSeconds("2021-12-31T16:00:00");
	//vector<string> symbolList;
	//populateSymbolVector(symbolList);
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	CURL* handle;
	CURLcode result;
	curl_global_init(CURL_GLOBAL_ALL);
	handle = curl_easy_init();
	if (handle)
	{
		string sCookies, sCrumb;
		for (auto itr = StockList.begin(); itr != StockList.end(); itr++)
		{
			free(data.memory);
			data.memory = NULL;
			data.size = 0;

			string startTime = getTimeinSeconds(itr->second.Get_StartDate() + "T16:00:00");
			string endTime = getTimeinSeconds(itr->second.Get_EndDate() + "T16:00:00");

			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
			string symbol = itr->first;
			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history";
			string url = urlA + symbol + urlB + startTime + urlC + endTime;
			//convert to C 
			const char* cURL = url.c_str();
			curl_easy_setopt(handle, CURLOPT_URL, cURL);

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
			result = curl_easy_perform(handle);
			if (result != CURLE_OK)
			{
				// if errors have occurred, tell what is wrong with result
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(result));
				return 1;
			}
			stringstream sData;
			sData.str(data.memory);
			string sValue; //adjprice
			double dValue = 0;
			vector<double> tempvector;
			string line;
			getline(sData, line);
			while (getline(sData, line)) {
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',') + 1);
				dValue = strtod(sValue.c_str(), NULL);//string to double
				tempvector.push_back(dValue);
				//only keep dValue
			}
			itr->second.Set_AdjClose(tempvector);
			count += 1;
			if (count % (length / 10) == 0)
				cout << "- - - - - downloading " << (count * 1.0 / length) * 100 << "% - - - - -" << endl;
		}
		free(data.memory);
		data.memory = NULL;
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);
	// release resources acquired by curl_global_init()
	curl_global_cleanup();
	return 0;
}

