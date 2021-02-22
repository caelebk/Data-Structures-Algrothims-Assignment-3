/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{

/* your code here! */
  
  map<RGBAPixel,string> temp = photos;

  
  map<RGBAPixel, string>::iterator it;
  for (it = temp.begin(); it != temp.end(); it++) {
      tree.push_back(it->first);
  }
  build(0, tree.size()-1, 0);
  
}

void rgbtree::build(int start, int end, int d){
    if (start >= end) return;
    int median = (start + end) / 2;
    quickSelect(start, end, median, d);
    build(start, median-1, (d+1) % 3);
    build(median+1, end, (d+1) % 3);
}

RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{

/* your code here! */

  RGBAPixel nearest = tree[(tree.size()-1)/2];
  findHelper(query, nearest, 0, tree.size()-1, 0);
  return nearest;
}

int rgbtree::findHelper(const RGBAPixel &query, RGBAPixel &nearest, int start, int end, int dimension) const {	
  if (start >= end) { 
    return start;
  } else {
    map<int, int> temp;
    pair<int, int> root;
    int median = (start+end)/2;
    nearest = tree[median];
    int nearestdist = dist(query, tree[median]);

    root.first = dist(query, tree[median]);
    root.second = median;
    temp.insert(root);

    int insubindex;
    if(smallerByDim(query, tree[median], dimension))
      insubindex = findHelper(query, nearest, start, median-1, (dimension+1)%3);
    else 
      insubindex = findHelper(query, nearest, median+1, end, (dimension+1)%3);

    pair<int, int> insubtree;
    insubtree.first = dist(query, tree[insubindex]);
    insubtree.second = insubindex;
    temp.insert(insubtree);

    nearestdist = min(insubtree.first, root.first);
    nearest = tree[temp[nearestdist]];

    int outsubindex;
    if(distToSplit(query, tree[median], dimension) < nearestdist) {

      if(smallerByDim(query, tree[median], dimension))
        outsubindex = findHelper(query, nearest, median+1, end, (dimension+1)%3);
      else 
        outsubindex = findHelper(query, nearest, start, median-1, (dimension+1)%3);
      
      pair<int, int> outsubtree;
      outsubtree.first = dist(query, tree[outsubindex]);
      outsubtree.second = outsubindex;
      temp.insert(outsubtree);
      
      nearestdist = min(outsubtree.first, nearestdist);
      nearest = tree[temp[nearestdist]];
    } 
    return temp[nearestdist];
  }
}


bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{
  /* your code here! */
	if(curDim == 0){
    return first.r <= second.r;
  } else if (curDim == 1) {
    return first.g <= second.g;
  } 
  return first.b <= second.b;
}

/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{

/* your code here! */


  if(start >= end) return;
  int p = partition(start, end, d);
  if(k == p){
    return;
  } else if (k > p) {
    quickSelect(p+1, end, k, d);
  } else {
    quickSelect(start, p-1, k, d);
  }
  
}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    // this is the partition code from HW2 it doesn't work!!
    
    int p = lo;
    for( int i=lo+1; i <= hi; i++ )
      // tree[i] < tree[lo]
      if( smallerByDim(tree[i], tree[lo], d) ) { 
        p++; 
        swap(tree[p], tree[i]); 
      }
    swap(tree[lo], tree[p]);
    return p;
}


/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{

/* your code here! */
  if(dimension == 0) {
      return (query.r - curr.r) * (query.r - curr.r);
  } else if (dimension == 1) {
      return (query.g - curr.g) * (query.g - curr.g);
  } else {
      return (query.b - curr.b) * (query.b - curr.b);
  }

}
/**
 * Helper function to help determine distance
 */
int rgbtree::dist(const RGBAPixel& query, const RGBAPixel& curr) const{
	return (query.r - curr.r)*(query.r - curr.r)  + (query.g - curr.g)*(query.g - curr.g) + (query.b - curr.b)*(query.b - curr.b);
}



