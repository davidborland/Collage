///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        OBJObjectAO.h
//
// Author:      David Borland
//
// Description: Class for loading and rendering geometry from a .obj file, including an 
//              .xml file containing per-vertex ambient occlusion information from AOGen
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef OBJOBJECTAO_H
#define OBJOBJECTAO_H


#include "OBJObject.h"

#include "PerlinNoise.h"


class OBJObjectAO : public OBJObject {
public:
    OBJObjectAO();
    virtual ~OBJObjectAO();

    // Load Ambient Occlusion data
    bool LoadAO(const std::string& fileName); 

    // Save/Load per vertex color data
    bool SavePVC(const std::string& vrmlInFileName,
                 const std::string& pvcOutFileName);
    bool LoadPVC(const std::string& fileName);

    // Save to vtk file format
    bool SaveVTK(const std::string& fileName);

    // Save to OBJ with material file for colors
    bool SaveOBJMTL(const std::string& filePrefix);

    // Use diffused ambient occlusion values
    void DiffusionOn();
    void DiffusionOff();

//    void CreateTexture2D();
    void CreateTexture3D();

    void SetAOVariableLocations(int aoOpacity, int aoColor);

    void DilateGeometry(bool dilate);

	virtual void ExpandPoints(double expansion);

protected:
    std::vector<float> aoValues;
    std::vector<float> aoDiffusion;
    std::vector<Vec3> vertexColors;

    int diffusion;

//    GLuint texture2D;
    GLuint texture3D;
    PerlinNoise* noise;

    int aoOpacityLocation;
    int aoColorLocation;

    bool dilateGeometry;

    float OtsuThreshold(const std::vector<float>& values);

    virtual void RenderGeometry();
    virtual void RenderDilated();
};


#endif