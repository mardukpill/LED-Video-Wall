#include "canvas.h"
#include <opencv2/opencv.hpp>

int main() {
    //Create a virtual canvas
    VirtualCanvas vCanvas(cv::Size(3000, 3000));

    //Create elements
    Element elem1("img.jpg", 1, cv::Point(100, 100));
    Element elem2("img2.jpg", 2, cv::Point(500, 700));


    // Add elements to the canvas
    vCanvas.addElementToCanvas(elem1);
    vCanvas.addElementToCanvas(elem2);



    cv::imshow("Canvas Before Removal", vCanvas.getPixelMatrix());
    cv::waitKey(0);

    // Remove one element
    vCanvas.removeElementFromCanvas(elem1);


    printf("Show removal\n");
    cv::imshow("Canvas After Removal", vCanvas.getPixelMatrix());
    cv::waitKey(0);

    return 0;
}
