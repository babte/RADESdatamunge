#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <map>
using namespace std;

// babette.dobrich@cern.ch, 2016
// The idea here is the following. In a dummy csv file we search the entry of T10 with the closest timestamp to a predefined timestamp
// The dummy CSV file contains the UNIX timestamp in the first column, the T10 value in the second

long long find_t10(int timestamp, map<int, long long> m )
{
  map<int,long long>::iterator greater = m.upper_bound(timestamp);
  // First check whether we are at the end of beginning of all available timestamps
  if ( greater == m.end() )
  {
    // if you asked for exactly the last element return that
    if ( m.rbegin()->first == timestamp )
    {
      return m.rbegin()->second;
    }
    else
    {
      // you are asking for a timestamp beyond the covered range
      cout << "ERROR, ERRROR, ERRROR\n";
      return 0;
    }
  }
  else if ( greater == m.begin() )
  {
    // you are at or before the earliest timestamp, so return that
    cout << "ERROR, ERRROR, ERRROR\n";
    return 0;
  }
  else
  {
    // we have either exactly a timestamp in the map or are between two timestamps in the map
    // since timestamp is not in the map "greater" will be the first key larger than timestamp
    // find the key before this one
    map<int,long long>::iterator lesser = greater;
    --lesser;

    // return the value in the map whose key is the closest to timestamp
    // we know all timestamps are positive so this comparison is easy :-)
    if ( (timestamp - lesser->first) < (greater->first - timestamp) )
    {
      return lesser->second;
    }
    else
    {
      return greater->second;
    }
  }

  // how did we get here?
  cout << "ERROR, ERRROR, ERRROR\n";
  return 0;
}

// compute the mean value of T10
int main()
{

  cout << "start" << endl;

  // read in your data
  ifstream  data("dummy2.csv");
  map<int,long long> m;
  string line;
  while(getline(data,line))
  {
    stringstream   lineStream(line);
    string         cell;
    vector<string> result;
    while(getline(lineStream,cell,','))
    {
      result.push_back(cell.c_str());
    }

    if (result.size()==2)
    {
      // strip the last three digits from the string
      result[0].erase(result[0].length()-3);
      m[stoi(result[0])] = stoll(result[1]);
    }
    else
    {
      cout << "ERROR, ERRROR, ERRROR\n";
      return 1;
    }

  }

  // show that it is sensible
  for( map<int,long long>::iterator itr = m.begin(); itr != m.end(); itr++ )
  {
    cout << itr->first << " -> " << itr->second << '\n';
  }

  // let's test our find function
  vector<int> timestamps;
  timestamps.push_back(1463292000); // <-- before beginning
  timestamps.push_back(1463292010);
  timestamps.push_back(1463292378);
  timestamps.push_back(1463292132);
  timestamps.push_back(1463292095);
  timestamps.push_back(1470895189);
  timestamps.push_back(1470895190);
  for( vector<int>::iterator itr = timestamps.begin(); itr != timestamps.end(); ++itr )
  {
    cout << "find value " << *itr << '\n';
    long long t10 = find_t10(*itr, m );
    cout << "T10 for " << *itr << " is: " << t10 << '\n';
  }

  return 0;
}
