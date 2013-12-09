///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        PerlinNoise.h
//
// Author:      David Borland
//
// Description: Implements Ken Perlin's improved noise algorithm, as described here:
//              http://mrl.nyu.edu/~perlin/noise/
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


class PerlinNoise {
public:
    PerlinNoise();

    double Noise(double x, double y, double z);

protected:
    int p[512];
    static int permutation[];

    double Fade(double t);
    double Lerp(double t, double a, double b);
    double Grad(int hash, double x, double y, double z);
};