
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos){

/* your code here! */
  PNG result = PNG(target.width()*TILESIZE, target.height()*TILESIZE);

  for (unsigned j = 0; j < target.height(); j++) {
    for (unsigned i = 0; i < target.width(); i++) {
      RGBAPixel * p = target.getPixel(i,j);
      RGBAPixel n = ss.findNearestNeighbor(*p);
      PNG thumb = PNG();
      thumb.readFromFile(photos[n]);
      int a = 0;
      int b = 0;
      for (unsigned h = j * TILESIZE; h < j*TILESIZE + 30; h++) {
        for (unsigned w = i *TILESIZE; w < i*TILESIZE + 30; w++)
        {
          *result.getPixel(w,h) = *thumb.getPixel(a,b);
          a++;
        }
        b++;
        a=0;
      }
      }
    }
  return result;
}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map < RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
        PNG curr; curr.readFromFile(entry.path());
				int red = 0;
      	int blue = 0;
      	int green = 0;
      	for(int x = 0; x < 30; x++) {
          for(int y = 0; y < 30; y++) {
            RGBAPixel * temp = curr.getPixel(x,y);
            red += temp->r;
            blue += temp->b;
            green += temp->g;
          }
        }
      	red = red / 900;
      	green = green / 900;
				blue = blue / 900;
      	thumbs[RGBAPixel(red,green,blue,255)] = entry.path(); 
    }
    return thumbs;
}


