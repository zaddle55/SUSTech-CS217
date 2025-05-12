#include <iostream>

#include "mcv.decl.h"
using namespace std;
using namespace mcv;

int main() {
    
    // 1. Read an image(png, jpg, bmp, tga)
    pMat img1 = imread<3>("./img/sustech.jpg");     // a RGB image(template parameter specifies the number of channels)
    cout << "Size of img1: " << img1.shape() << endl;

    // 2. Write an image(png, jpg, bmp, tga)
    imwrite("./img/out1.png", img1);    // a different image format, depending on the file extension name

    // 3. Convert to grayscale
    pMat img1gray = im2gray(img1);      // This will be a faster operation
    imwrite("./img/out2.png", img1gray);

    // 4. Convert to YCbCr
    pMat img1ycbcr = cvtColor(img1, color_conversion::RGB2YCbCr); // or RGB2HSL/RGB2YUV/RGB2HSV/...
    imwrite("./img/out3.png", img1ycbcr);

    // 5. Draw a line
    pMat img1line = line(img1, Line(0, 0, 100, 400, 5.0), color("ff8080")); // Now there will be a red line
    imwrite("./img/out4.png", img1line);

    // 6. Draw a circle
    pMat img1circle = circle(img1, Circle(100, 200, 20), color("ff8080")); // Now there will be a red circle
    imwrite("./img/out5.png", img1circle);

    // 7. clip a region
    pMat img1clip = clip(img1, Rect(10, 20, 160, 300));
    imwrite("./img/out6.png", img1clip);

    // 8. mix two images
    pMat imgmix = mix(img1, img1circle, 0.5); // mix two images with alpha = 0.5
    imwrite("./img/out7.png", imgmix);

    // or you can do it by yourself
    pMat img1mix = img1 / 2 + img1circle / 2; // mix two images
    imwrite("./img/out8.png", img1mix);

    // 9. Rotate
    pMat img1rotate = rotate(img1, 15);
    imwrite("./img/out9.png", img1rotate);

    // 10. Scale
    pMat img1scale = scale(img1, 0.7, 0.3); // produce a wider image
    imwrite("./img/out10.png", img1scale);

    // 11. Translate
    pMat img1translate = translate(img1, 100, 200);
    imwrite("./img/out11.png", img1translate);

    // 12. Adjust brightness
    pMat img1bright = adjustBright(img1, 40, true); // increase brightness
    imwrite("./img/out12.png", img1bright);

    // or you can do it by yourself, but it is not a saturation operation
    pMat img1bright2 = img1 + 40; // increase brightness
    imwrite("./img/out13.png", img1bright2);

    // 13. DFT
    pMat img1dft = dft(img1); // DFT
    imwrite("./img/out14.png", img1dft);

    // 14. high pass filter
    FreqMat imgfreq = dftNoNormalize(img1); // You may need to get original frequency domain
    processHighpass(imgfreq, 25); // cutoff = 10
    pMat img1highpass = idft(imgfreq); // IDFT
    imwrite("./img/out15.png", img1highpass); // high pass means edges will be enhanced

    // 15. Box filter
    auto box = getBoxFilter<5>(); // 5x5 box filter
    pMat img1box = conv2d(img1, box); // Convolution
    imwrite("./img/out16.png", img1box);

    // 16. Gaussian filter
    auto gauss = getGaussianKernel<5>(1.2); // 5x5 Gaussian filter
    pMat img1gauss = conv2d(img1, gauss); // Convolution
    imwrite("./img/out17.png", img1gauss);

    // 17. Sobel filter
    auto sobel = getSobelKernel<5>(); // 5x5 Sobel filter
    pMat img1sobel = conv2d(img1gray, sobel); // Convolution
    imwrite("./img/out18.png", img1sobel);

    // 18. Laplacian filter
    auto laplacian = getLaplacianKernel<5>(); // 5x5 Laplacian filter
    pMat img1laplacian = conv2d(img1, laplacian); // Convolution
    imwrite("./img/out19.png", img1laplacian);

    // 19. ASCII Art
    asciiArt(img1, 150); // ASCII Art, you can specify the width or height
    cout << "\n\n" << endl;
    asciiArt(img1, 150, -1, "@#&$% "); // you can also specify the charset for display
}