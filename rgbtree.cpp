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
  //how to construct an rgbtree with averaged color RGBAPixel??
 
  //loop over key in the map and put into a vector<RGBAPixel> - tree
  for(auto i=photos.begin(); i!=photos.end(); i++){
    tree.push_back(i->first);
  }

  int lo = 0;
  int hi = tree.size()-1;
  int d = 0; 
 
  rgbtreeHelper(lo, hi, d);

}

//helper function
void rgbtree::rgbtreeHelper(int lo, int hi, int d){
 
  if(lo >= hi){
    return;
  }
  if(lo<hi){
    quickSelect(lo, hi, (lo+hi)/2, d);  //level%3 = d?  
    d = (d+1)%3;  //update dimension
    int lo_right = (lo+hi)/2 + 1;
    int hi_left = (lo+hi)/2 - 1;
    rgbtreeHelper(lo_right, hi, d);
    rgbtreeHelper(lo, hi_left, d);
  }

}


RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{

/* your code here! */
  return findNearestNeighborHelper(query, 0, tree.size()-1, 0);
}


//recursive helper fuction:
RGBAPixel rgbtree::findNearestNeighborHelper(RGBAPixel query, int start, int end, int d) const
{

  // recursive helper function needed
  if(start == end){
    return tree[start];
  }

  RGBAPixel currBest; //root upper bound

  if(start<end){
    int mid = (start+end)/2;
    currBest = tree[mid]; //curr best root
    int currBestDistance = distance(query, currBest);  //what's our base case??????  is query the root????(tree is unsorted array)

    RGBAPixel subBest = currBest;  //subtree
    if(smallerByDim(query, tree[mid], d)){  //determine which split dimension to compare
      if(start<mid){
         subBest = findNearestNeighborHelper(query, start, mid-1, (d+1)%3); //left
      }
    }else{
       subBest = findNearestNeighborHelper(query, mid+1, end, (d+1)%3); //right
    }

    int subBestDistance = distance(query, subBest);

    //if (currBestDistance < subBestDistance){currBest = subBest; currBestDistance = subBestDistance;}
    if (subBestDistance < currBestDistance){currBest = subBest; currBestDistance = subBestDistance;}

    //the other side of the tree

    RGBAPixel outBest = currBest;
    if(distToSplit(query, tree[mid], d) < currBestDistance){  //can I do this?
      if (smallerByDim(query, tree[mid], d)){
        outBest = findNearestNeighborHelper(query, mid+1, end, (d+1)%3);
      } else {
        if(mid > start){
          outBest = findNearestNeighborHelper(query, start, mid-1, (d+1)%3);
        }
      }

    int outBestDistance = distance(query, outBest);

    //if (currBestDistance < outBestDistance){currBest = outBest; currBestDistance = outBestDistance;}
    if (outBestDistance < currBestDistance){currBest = outBest; currBestDistance = outBestDistance;}

    return currBest;
    }

  }
  return currBest;
}


//helper function for squared distance
int rgbtree::distance(RGBAPixel r1, RGBAPixel r2) const
{
return (r1.r-r2.r)*(r1.r-r2.r) + (r1.g-r2.g)*(r1.g-r2.g) + (r1.b-r2.b)*(r1.b-r2.b);
}



bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{

/* your code here! */

  // how to track level in order to desicde which dimention to compare?? //dim+1%3
  if(curDim%3 == 0){
    if(first.r <= second.r){
      return true;
    }else{
      return false;
    }
  }

  if(curDim%3 == 1){
    if(first.g <= second.g){
      return true;
    }else{
      return false;
    }
  }

  if(curDim%3 == 2){
    if(first.b <= second.b){
      return true;
    }else{
      return false;
    }
  }

  return true; /////////////
}


/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{

/* your code here! */
  // int lo = start;
  // int hi = end;
  // if(lo<hi){ //use if - base case
  //   int newstart = partition(start, hi, d);
  //   if(k<(start+end)/2){
  //     hi = newstart-1;
  //     quickSelect(start, hi, k, d);
  //   }else if(k>(start+end)/2){
  //     lo = newstart+1;
  //     quickSelect(lo, hi, k, d);
  //   }else{
  //     return;
  //   }
  // }
  int lo = start;
  int hi = end;
  if(lo<hi){ //use if - base case
    int p = partition(start, hi, d);
    if(k<p){
      hi = p-1;
      quickSelect(lo, hi, k, d);
    }else if(k>p){
      lo = p+1;
      quickSelect(lo, hi, k, d);
    }else{
      return;
    }
  }
}


//haven't built the kdtree aray in quick select
//only found the median

/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    // this is the partition code from HW2 it doesn't work!!
    // int p = lo;
    // for( int i=lo+1; i <= hi; i++ )
    //   if( tree[i] < tree[lo] ) { p++; swap(tree[p], tree[i]); }
    // swap(tree[lo], tree[p]);
    // return p;

    int p = lo;
    for( int i=lo+1; i <= hi; i++ )
    if(smallerByDim(tree[i],tree[lo],d)) { p++; swap(tree[p], tree[i]); }
    swap(tree[lo], tree[p]);
    return p;

    //return p with all the items before p less than tree[p].
    //and with all the items after p more than tree[p]
}


/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{

  /* your code here! */


  // how to check the other side? splitting plane through point curr, in the given dimension.
  if(dimension%3 == 0){
    return pow((query.r-curr.r), 2);
  }

  if(dimension%3 == 1){
    return pow((query.g-curr.g), 2);
  }

  if(dimension%3 == 2){
    return pow((query.b-curr.b), 2);
  }
  //unsigned integer convert(int)
  return -1;  //////
}

