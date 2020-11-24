
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

    //loop original photo
    PNG newImage(target.width()*TILESIZE, target.height()*TILESIZE);

// for(unsigned int i = 0; i<target.width(); i++){
//     for(unsigned int j = 0; j<target.height(); j++){
//         RGBAPixel* temp_target = target.getPixel(i, j); //target pixel pointer to replace with the new thumbail images
        
//         //RGBAPixel bestnbr = ss.findNearestNeighbor(*temp_target); //assuming I found the best neighbor
    
//         //picture.readFromFile(photos[ss.findNearestNeighbor(*temp_target)]); //open() //as png and fill the pixel
//         PNG picture; 
//         picture.readFromFile(photos[ss.findNearestNeighbor(*temp_target)]); 
 
//         for(unsigned int n = 0; n<picture.width(); n++){
//             for(unsigned int m = 0; m<picture.height(); m++){
//                 //RGBAPixel * temp = newImage.getPixel(n,m);
//                *(newImage.getPixel((i*TILESIZE+n),(j*TILESIZE+m))) = *(picture.getPixel(n,m));
//             }
//         } 
//     }
// }

    for(unsigned int i = 0; i<target.width(); i++){
        for(unsigned int j = 0; j<target.height(); j++){
            RGBAPixel* temp_target = target.getPixel(i, j); //target pixel pointer to replace with the new thumbail images
        
            //RGBAPixel bestnbr = ss.findNearestNeighbor(*temp_target); //assuming I found the best neighbor
    
            //picture.readFromFile(photos[ss.findNearestNeighbor(*temp_target)]); //open() //as png and fill the pixel
            PNG picture; 
            picture.readFromFile(photos[ss.findNearestNeighbor(*temp_target)]); 
 
            for(unsigned int n = 0; n<picture.width(); n++){
                for(unsigned int m = 0; m<picture.height(); m++){
                    //RGBAPixel * temp = newImage.getPixel(n,m);
                    *(newImage.getPixel((i*TILESIZE+n),(j*TILESIZE+m))) = *(picture.getPixel(n,m));
                }
            } 
        }
    }

    // for(unsigned int i = 0; i<target.width(); i++){
    //     for(unsigned int j = 0; j<target.height(); j++){
    //         RGBAPixel * temp_target = target.getPixel(i, j); //target pixel pointer to replace with the new thumbail images
        
    //         RGBAPixel bestnbr = ss.findNearestNeighbor(*temp_target); //assuming I found the best neighbor
    
    //         string thumbnail =  photos[bestnbr];  //not helpful

    return newImage;

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

    map<RGBAPixel, string> thumbs;
    //vector<vector<RGBAPixel>> data;  //vector<vector<pixel>>
    //RGBAPixel * imageData = new RGBAPixel[curr.width() * curr.height()]; //array of pixels

    for (const auto & entry : fs::directory_iterator(path)) { //entry is each of thumnail image
        PNG curr; curr.readFromFile(entry.path()); //entry path is the string


        /* your code here */
        RGBAPixel avg;
        int r = 0;
        int g = 0; 
        int b = 0;
        //get vector<vector<pixel>> for each the 30*30 image
        for(unsigned int i = 0; i < curr.width(); i++){
            vector<RGBAPixel> vertical;
            for(unsigned int j = 0; j < curr.height(); j++){
                RGBAPixel * temp = curr.getPixel(i, j);
                // vertical.push_back(*temp);
                // imageData[k] = data[i][j];
                r = temp->r + r;
                g = temp->g + g;
                b = temp->b + b;

            }
            //avg = RGBAPixel(r/(curr.width()*curr.height()), g/(curr.width()*curr.height()), b/(curr.width()*curr.height()));
            //thumbs[avg] = entry.path();
        }
        avg = RGBAPixel(r/(curr.width()*curr.height()), g/(curr.width()*curr.height()), b/(curr.width()*curr.height()));
        thumbs[avg] = entry.path();
    }
    return thumbs;
}


