#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;

int binarySearch(int arr[], int left, int right, int x)
{
    if (right >= left)
    {
        int index = (right + left) / 2;
        cout << "We traverse on index: " << index << endl;
        if (arr[index] < x)
        {
            return binarySearch(arr, index + 1, right, x);
        }
        else if (arr[index] > x)
        {
            return binarySearch(arr, left, index - 1, x);
        }
        else
            return index;
    }
    return -1;
}
int interpolationSearch(int arr[], int left, int right, int x)
{
    if (left <= right && arr[left] <= x && arr[right] >= x)
    {
        int pos = left + ((x - arr[left]) * (right - left)) / (arr[right] - arr[left]);
        cout << "We traverse on index: " << pos << endl;
        if (arr[pos] == x)
        {
            return pos;
        }
        else if (arr[pos] < x)
        {
            return interpolationSearch(arr, pos + 1, right, x);
        }
        else
        {
            return interpolationSearch(arr, left, pos - 1, x);
        }
    }
    return -1;
}
int jumpSearch(int arr[], int x, int n)
{
    int k = 0;       // index block
    int m = sqrt(n); // block size
    // index phan tu dau tien cua block "k*m" phai nho hon so phan tu "n"
    while (k * m < n)
    {
        cout << k * m << " "; // show first index of the block
        // condition: x > block 'k' va x <= block 'k+1'
        // special note: add case m*m< n and still not find yet.
        if (arr[k * m] < x && ((k + 1) * m >= n || x <= arr[(k + 1) * m]))
        {
            // cases without in last block.
            if (n > (k + 1) * m)
            {
                cout << ((k + 1) * m) << " ";
                // x= block 'k+1'
                if (x == arr[(k + 1) * m])
                {
                    return (k + 1) * m;
                }
            }

            // traverse linear block 'k'
            int i = 0;
            for (i = k * m; i < min((k + 1) * m, n); i++)
            {
                if (i != k * m)
                    cout << i << " ";
                if (arr[i] == x)
                    return i;
            }
            //[option] yeu cau de bai
            if (i == n)
                cout << n << " ";
            return -1;
        }
        k++;
    }
    return -1;
}
bool findPairs(int arr[], int n, pair<int, int> &pair1, pair<int, int> &pair2)
{
    // TODO: If there are two pairs satisfy the condition,
    // TODO: assign their values to pair1, pair2 and return true. Otherwise, return false.
    vector<pair<int,int>> pairList;
    for (int i = 0; i < n-1; i++)
    {
        for (int t = i + 1; t < n; t++)
        {
            pair<int,int> pa;
            pa.first=arr[i];
            pa.second=arr[t];
            pairList.push_back(pa);
        }
    }
    map<int,int> mp;
    
    for (int i=0;i<pairList.size();i++)
    {
        pair<int,int> pa;
        pa.first=i;
        pa.second=pairList[i].first+pairList[i].second;
        mp.insert(pa);
    }
    for (int i=0;i<pairList.size()-1;i++)
    {
        for (int j=i+1;j<pairList.size();j++)
        {
            if (mp[i]==mp[j])
            {
                pair1=pairList[i];
                pair2=pairList[j];
                return true;
            }
        }
    }
    return false;
}
int main()
{
    int arr[] = {3, 4, 7, 1, 2, 9, 8};
    int n = sizeof arr / sizeof arr[0];
    pair<int, int> pair1, pair2;
    findPairs(arr, n, pair1, pair2);
}