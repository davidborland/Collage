///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        OBJObjectAO.cpp
//
// Author:      David Borland
//
// Description: Class for loading and rendering geometry from a .obj file, including an 
//              .xml file containing per-vertex ambient occlusion information from AOGen
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "OBJObjectAO.h"

#include "Utilities.h"

#include <fstream>


OBJObjectAO::OBJObjectAO() : OBJObject() {
    diffusion = false;

    DepthSortOn();

    noise = new PerlinNoise();

    dilateGeometry = false;
}
OBJObjectAO::~OBJObjectAO() {
    delete noise;
}


bool OBJObjectAO::LoadAO(const std::string& fileName) {
    std::ifstream file(fileName.c_str());
    if (file.fail()) {
        std::cout << "OBJObjectAO::LoadAO() : Couldn't open " << fileName.c_str() << std::endl;
        return false;
    }

    std::cout << "OBJObjectAO::LoadAO() : Loading " << fileName.c_str() << std::endl;

    std::string s;
    std::vector<std::string> tokens;

    aoValues.resize(verts.size());

    int index;
    while (!file.eof()) {
        getline(file, s);
        tokens = Utilities::Tokenize(s, " \n");

        for (int i = 0; i < (int)tokens.size(); i++) {
            if (tokens[i].find("idx=") != std::string::npos) {
                int beg = tokens[i].find('"') + 1;
                int end = tokens[i].rfind('"') - 1;
                std::string value = tokens[i].substr(beg, end - beg + 1);

                index = atoi(value.c_str());
            }
            else if (tokens[i].find("occ=") != std::string::npos) {
                int beg = tokens[i].find('"') + 1;
                int end = tokens[i].rfind('"') - 1;
                std::string value = tokens[i].substr(beg, end - beg + 1);

                float ao = (float)atof(value.c_str());

                aoValues[index] = ao;
            }
        }
    }

    file.close();


/*    
    // Write the values to a simple text file
    std::string outFileName = fileName + ".txt";
    std::ofstream outFile(outFileName.c_str());
    if (outFile.fail()) {
        std::cout << "OBJObjectAO::LoadAO() : Couldn't open " << outFileName.c_str() << std::endl;
        return false;
    }

    for (int i = 0; i < (int)aoValues.size(); i++) {
        outFile << aoValues[i] << std::endl;
    }
    outFile.close();
*/
    std::cout << OtsuThreshold(aoValues) << std::endl;



    // Compute anisotropic diffusion of the ambient occlusion values
    // via the Perona-Malik method
  
    // Number of iterations.  More iterations == more smoothing.
    // 100 seems to be a reaonable value
    int numIter = 100; 

    // Diffusion coefficient.  Smaller values == stronger edge effect
    // [0.01, 1.0] seems to be a reasonable range
    double K = 1.0;  

    aoDiffusion.resize(aoValues.size());
    for (int i = 0; i < (int)aoDiffusion.size(); i++) {
        aoDiffusion[i] = aoValues[i];
    }

    for (int iter = 0; iter < numIter; iter++) {
        std::vector<float> aoTemp;
        aoTemp.resize(verts.size());
        std::vector<int> count;
        count.resize(verts.size());
        for (int i = 0; i < (int)aoTemp.size(); i++) {
            aoTemp[i] = 0.0;
            count[i] = 0;
        }

        for (int i = 0; i < (int)groups.size(); i++) {
            Group* group = groups[i];
            for (int j = 0; j < (int)group->NumSubGroups(); j++) {
                SubGroup* subGroup = group->GetSubGroup(j); 
                for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                    Face* face = subGroup->GetFace(k);
                    for (int v = 0; v < face->NumVertices(); v++) {
                        int n = face->GetVertexIndex(v);
                        for (int v2 = 0; v2 < 2; v2++) {
                            int n2;
                            if (v2 == 0) n2 = v == face->NumVertices() - 1 ? face->GetVertexIndex(0) : 
                                                                             face->GetVertexIndex(v + 1);
                            else n2 = v == 0 ? face->GetVertexIndex(face->NumVertices() - 1) :
                                               face->GetVertexIndex(v - 1);

                            double g = aoDiffusion[n2] - aoDiffusion[n];
                            double c = 1.0 / (1.0 + (g / K) * (g / K));
                            aoTemp[n] += (float)(g * c);
                            count[n]++;                
                        }
                    }
                }
            }
        }

        for (int i = 0; i < (int)aoTemp.size(); i++) {
            aoTemp[i] = aoDiffusion[i] + aoTemp[i] / count[i];
            aoDiffusion[i] = aoTemp[i];
        }
    }

/*
    // Write the values to a simple text file
    std::string outFileName2 = fileName + "_smoothed.txt";
    std::ofstream outFile2(outFileName2.c_str());
    if (outFile2.fail()) {
        std::cout << "OBJObjectAO::LoadAO() : Couldn't open " << outFileName2.c_str() << std::endl;
        return false;
    }

    for (int i = 0; i < (int)aoDiffusion.size(); i++) {
        outFile2 << aoDiffusion[i] << std::endl;
    }
    outFile2.close();
*/   
    std::cout << OtsuThreshold(aoDiffusion) << std::endl;
/*
    // Normalize
    float maxVal = 0.0;
    float minVal = 1.0;
    for (int i = 0; i < (int)aoDiffusion.size(); i++) {
        if (aoDiffusion[i] > maxVal) maxVal = aoDiffusion[i];
        if (aoDiffusion[i] < minVal) minVal = aoDiffusion[i];
    }

    for (int i = 0; i < (int)aoDiffusion.size(); i++) {
        aoDiffusion[i] = (aoDiffusion[i] - minVal) / (maxVal - minVal);
    }
*/

    return true;
}


bool OBJObjectAO::SavePVC(const std::string& vrmlInFileName,
                          const std::string& pvcOutFileName) {
    // XXX : HUGE hack to use per vertex color from VRML file.  I feel dirty...

    // Open the VRML file                                          
    std::ifstream inFile(vrmlInFileName.c_str());
    if (inFile.fail()) {
        std::cout << "OBJObjectAO::SavePVC() : Couldn't open " << vrmlInFileName.c_str() << std::endl;
        return false;
    }

    std::cout << "OBJObjectAO::SavePVC() : Loading " << vrmlInFileName.c_str() << std::endl;

    // Store the vertices and vertex colors from the VRML file
    // XXX: Also using vrmlVerts to store sphere centers, which is an ugly hack.
    std::vector<Vec3> vrmlVerts;
    std::vector<Vec3> vrmlVertexColors;

    std::string s;
    std::vector<std::string> tokens;

    // Parse the VRML file
    bool readingPoints = false;
    bool readingColors = false;
    while (!inFile.eof()) {
        getline(inFile, s);
        tokens = Utilities::Tokenize(s, " \n");

        if (tokens.size() == 0) continue;

        if (tokens[0] == "point" && tokens[1] == "[") {
            readingPoints = true;
        }
        else if (tokens[0] == "color" && tokens[1] == "[") {
            readingColors = true;
        }        
        else if (readingPoints) {
            if (tokens[0] == "]") {
                readingPoints = false;   
            }
            else {
                std::string x = tokens[0];
                std::string y = tokens[1];
                std::string z = tokens[2];
                Vec3 point(atof(x.c_str()), atof(y.c_str()), atof(z.c_str()));

                vrmlVerts.push_back(point);
            }
        }
        else if (readingColors) {
            if (tokens[0] == "]") {
                readingColors = false;
            }
            else {
                std::string r = tokens[0];
                std::string g = tokens[1];
                std::string b = tokens[2];
                Vec3 color(atof(r.c_str()), atof(g.c_str()), atof(b.c_str()));

                vrmlVertexColors.push_back(color);
            }
        }
/*
        // XXX: THIS IS A HORRIBLE HACK TO USE SPHERE CENTERS, AND WILL PROBABLY
        //      CRASH WHEN LOADING NORMAL GEOMETRY
        else if (tokens[0] == "translation") {
            std::string x = tokens[1];
            std::string y = tokens[2];
            std::string z = tokens[3];
            Vec3 point(atof(x.c_str()), atof(y.c_str()), atof(z.c_str()));

            vrmlVerts.push_back(point);
        }
        else if (tokens[0] == "material") {
            std::string r = tokens[4];
            std::string g = tokens[5];
            std::string b = tokens[6];
            Vec3 color(atof(r.c_str()), atof(g.c_str()), atof(b.c_str()));

            vrmlVertexColors.push_back(color);
        }
*/
    }
    
    inFile.close(); 

    if (vrmlVerts.size() != vrmlVertexColors.size()) {
        std::cout << "OBJObjectAO::SavePVC() : Number of VRML vertices and colors does not match." << std::endl;
        return false;
    }


    std::cout << "OBJObjectAO::SavePVC() : Associating colors";

    // Search the VRML vertices for the closet vertex and use the vertex color 
    // associated with it.  This assumes the VRML and OBJ file have the same
    // transformation.
    for (int i = 0; i < (int)verts.size(); i++) {
        if (i % (verts.size() / 20) == 0) std::cout << ".";
        double closest = verts[i].Distance(vrmlVerts[0]);
        int closeIndex = 0;
        for (int j = 1; j < (int)vrmlVerts.size(); j++) {
            double distance = verts[i].Distance(vrmlVerts[j]);
            if (distance < closest) {
                closest = distance;
                closeIndex = j;
            }
        }
        vertexColors.push_back(vrmlVertexColors[closeIndex]);
    }
    std::cout << std::endl;


    std::cout << "OBJObjectAO::SavePVC() : Saving " << pvcOutFileName.c_str() << std::endl;

    // Save the vertex colors
    std::ofstream outFile(pvcOutFileName.c_str());
    if (outFile.fail()) {
        std::cout << "OBJObjectAO::SavePVC() : Couldn't open " << pvcOutFileName.c_str() << std::endl;
        return false;
    }
   
    for (int i = 0; i < (int)vertexColors.size(); i++) {
        outFile << vertexColors[i].X() << " " <<
                   vertexColors[i].Y() << " " <<
                   vertexColors[i].Z() << std::endl;
    }

    outFile.close();

    return true;
}


bool OBJObjectAO::LoadPVC(const std::string& fileName) {
    // XXX : HUGE hack to use per vertex color from VRML file.  I feel dirty...

    // Open the VRML file                                          
    std::ifstream file(fileName.c_str());
    if (file.fail()) {
        std::cout << "OBJObjectAO::LoadPVC() : Couldn't open " << fileName.c_str() << std::endl;
        return false;
    }

    std::cout << "OBJObjectAO::LoadPVC() : Loading " << fileName.c_str() << std::endl;

    std::string s;
    std::vector<std::string> tokens;

    // Load the per vertex color data
    while (!file.eof()) {
        getline(file, s);
        tokens = Utilities::Tokenize(s, " \n");

        if (tokens.size() != 3) continue;
        
        std::string r = tokens[0];
        std::string g = tokens[1];
        std::string b = tokens[2];        

        Vec3 color(atof(r.c_str()), atof(g.c_str()), atof(b.c_str()));

        
if (color.Y() > color.X() && color.Y() > color.Z()) color = Vec3(0.75, 0.75, 0.75);

        vertexColors.push_back(color);
    }
    
    file.close(); 

    if (verts.size() != vertexColors.size()) {
        std::cout << "OBJObjectAO::LoadPVC() : Number of vertices does not match number of vertex colors" << std::endl;
        return false;
    }

    return true;
}


bool OBJObjectAO::SaveVTK(const std::string& outVTKFileName) {
    std::cout << "OBJObjectAO::SavePVC() : Saving " << outVTKFileName.c_str() << std::endl;

    // Open the file
    std::ofstream outFile(outVTKFileName.c_str());
    if (outFile.fail()) {
        std::cout << "OBJObjectAO::SaveVTK() : Couldn't open " << outVTKFileName.c_str() << std::endl;
        return false;
    }

    // Write header info
    outFile << "# vtk DataFile Version 2.0" << std::endl;
    outFile << "AOOM data" << std::endl;
    outFile << "ASCII" << std::endl;
    outFile << "DATASET POLYDATA" << std::endl;

    // Write points
    outFile << "POINTS " << verts.size() << " float" << std::endl;
    for (int i = 0; i < (int)verts.size(); i++) {
        outFile << verts[i].X() << " " << verts[i].Y() << " " << verts[i].Z() << std::endl;
    }
    outFile << std::endl;

    // Write polygons
    int faceCount = 0;
    int faceVertCount = 0;
    for (int i = 0; i < (int)groups.size(); i++) {
        Group* group = groups[i];
        for (int j = 0; j < (int)group->NumSubGroups(); j++) {
            SubGroup* subGroup = group->GetSubGroup(j); 
            faceCount += subGroup->NumFaces();
            for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                Face* face = subGroup->GetFace(k);
                faceVertCount += face->NumVertices() + 1;
            }
        }
    }
    outFile << "POLYGONS " << faceCount << " " << faceVertCount << std::endl;
    for (int i = 0; i < (int)groups.size(); i++) {
        Group* group = groups[i];
        for (int j = 0; j < (int)group->NumSubGroups(); j++) {
            SubGroup* subGroup = group->GetSubGroup(j);
            for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                Face* face = subGroup->GetFace(k);
                outFile << face->NumVertices() << " ";
                for (int v = 0; v < face->NumVertices(); v++) {
                    outFile << face->GetVertexIndex(v);
                    if (v == face->NumVertices() - 1) outFile << std::endl;
                    else outFile << " ";
                }
            }
        }
    }
    outFile << std::endl;

/*
    // Write normals
    outFile << "NORMALS _normals float" << std::endl;    
    for (int i = 0; i < (int)vertexNormals.size(); i++) {
        outFile << vertexNormals[i].X() << " " << vertexNormals[i].Y() << " " << vertexNormals[i].Z() << std::endl;
    }
    outFile << std::endl;
*/

    // Write ambient occlusion data
    outFile << "POINT_DATA " << aoValues.size() << std::endl;
    outFile << "SCALARS ambient_occlusion float 2" << std::endl;
    outFile << "LOOKUP_TABLE default" << std::endl;
    for (int i = 0; i < (int)aoValues.size(); i++) {
        outFile << aoValues[i] << " " << aoDiffusion[i] << std::endl;
    }
    outFile << std::endl;

/*
    // Write ambient occlusion diffusion data
    outFile << "POINT_DATA " << aoDiffusion.size() << std::endl;
    outFile << "SCALARS ambient_occlusion_diffusion float 1" << std::endl;
    outFile << "LOOKUP_TABLE default" << std::endl;
    for (int i = 0; i < (int)aoDiffusion.size(); i++) {
        outFile << aoDiffusion[i] << std::endl;
    } 
*/

    outFile.close();

    return true;
}

bool OBJObjectAO::SaveOBJMTL(const std::string& filePrefix) {
    std::string objName = filePrefix + ".obj";
    std::string mtlName = filePrefix + ".mtl";

    // Determine colors
    std::vector<Vec3> colors;
    std::vector<std::string> mtlNames;
    for (int i = 0; i < (int)vertexColors.size(); i++) {
        bool found = false;
        for (int j = 0; j < (int)colors.size(); j++) {
            if (colors[j] == vertexColors[i]) {
                found = true;
                break;
            }
        }

        if (!found) {
            colors.push_back(vertexColors[i]);
            char name[256];
            sprintf_s(name, "mtl%d", mtlNames.size() + 1);
            mtlNames.push_back(name);
        }
    }

    // Save .mtl file
    std::ofstream mtlFile(mtlName.c_str());
    if (mtlFile.fail()) {
        std::cout << "OBJObjectAO::SaveOBJMTL() : Couldn't open " << mtlName.c_str() << std::endl;
        return false;
    }

    // Just keep the relative path now
    size_t slash = mtlName.rfind("/");
    if (slash != std::string::npos) {
        mtlName.erase(0, slash + 1);
    }
    
    for (int i = 0; i < (int)colors.size(); i++) {
        mtlFile << "newmtl " << mtlNames[i] << std::endl;
        mtlFile << "Ka " << colors[i].X() / 10.0 << " " <<
                            colors[i].Y() / 10.0 << " " <<
                            colors[i].Z() / 10.0 << std::endl; 
        mtlFile << "Kd " << colors[i].X() << " " <<
                            colors[i].Y() << " " <<
                            colors[i].Z() << std::endl; 
        mtlFile << std::endl;
    }

    mtlFile.close();

    // Save .obj file
    std::ofstream objFile(objName.c_str());
    if (objFile.fail()) {
        std::cout << "OBJObjectAO::SaveOBJMTL() : Couldn't open " << objName.c_str() << std::endl;
        return false;
    }

    objFile << "mtllib " << mtlName << std::endl << std::endl;

    for (int i = 0; i < (int)verts.size(); i++) {
        objFile << "v " << verts[i].X() << " " <<
                           verts[i].Y() << " " <<
                           verts[i].Z() << std::endl;
    }
    for (int i = 0; i < (int)vertexNormals.size(); i++) {
        objFile << "vn " << vertexNormals[i].X() << " " <<
                            vertexNormals[i].Y() << " " <<
                            vertexNormals[i].Z() << std::endl;
    }
    objFile << std::endl;

    objFile << "g grp1" << std::endl;

    for (int m = 0; m < (int)mtlNames.size(); m++) {
        objFile << "usemtl " << mtlNames[m] << std::endl;
        for (int i = 0; i < (int)groups.size(); i++) {
            Group* group = groups[i];
            for (int j = 0; j < (int)group->NumSubGroups(); j++) {
                SubGroup* subGroup = group->GetSubGroup(j);
                for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                    Face* face = subGroup->GetFace(k);
                    if (vertexColors[face->GetVertexIndex(0)] == colors[m]) {
                        objFile << "f ";
                        for (int v = 0; v < face->NumVertices(); v++) {
                            objFile << face->GetVertexIndex(v) + 1 << "//" << face->GetVertexIndex(v) + 1;
                            if (v == face->NumVertices() - 1) objFile << std::endl;
                            else objFile << " ";
                        } 
                    }
                }
            }
        }
    }

    return true;
}


void OBJObjectAO::DiffusionOn() {
    diffusion = true;
}

void OBJObjectAO::DiffusionOff() {
    diffusion = false;
}

/*
void OBJObjectAO::CreateTexture2D() {
    const int texDim = 256;
    const int texWidth = texDim;
    const int texHeight = texDim;
    float* image = new float[texWidth * texHeight];

    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            Vec2 pos(i, j);
            Vec2 center(texWidth * 0.5, texHeight * 0.5);
            Vec2 diff = pos - center;
            float val = (float)diff.Magnitude();

            image[i * texWidth + j] = val / texDim < 0.25;
        }
    }


    glGenTextures(1, &texture2D);    
    glBindTexture(GL_TEXTURE_2D, texture2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, texWidth, texHeight, GL_LUMINANCE, GL_FLOAT, image);


    delete [] image;
}
*/


void OBJObjectAO::CreateTexture3D() {
    const int texDim = 256;
    const int texWidth = texDim;
    const int texHeight = texDim;
    const int texDepth = texDim;
    float* image = new float[texWidth * texHeight * texDepth];

    float min = 1.0;
    float max = 0.0;
    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            for (int k = 0; k < texDepth; k++) {
                bool useNoise = true;
                float val;
                if (useNoise) {
                    double texScale = 0.8;
                    val = (float)noise->Noise((double)i * texScale, 
                                              (double)j * texScale, 
                                              (double)k * texScale);
                }
                else {
                    Vec3 pos(i, j, k);
                    Vec3 center(texWidth * 0.5, texHeight * 0.5, texDepth * 0.5);
                    Vec3 diff = pos - center;
                    val = (float)diff.Magnitude();
                }

                image[i * texWidth * texHeight + j * texHeight + k] = val;
                if (val < min) min = val;
                if (val > max) max = val;
            }
        }
    }
    // Normalize to [0, 1]
    for (int i = 0; i < texWidth * texHeight * texDepth; i++) {
        image[i] = (image[i] - min) / (max - min);
    }


    glGenTextures(1, &texture3D);    
    glBindTexture(GL_TEXTURE_3D, texture3D);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, texWidth, texHeight, texDepth, 0, GL_LUMINANCE, GL_FLOAT, image);

    delete [] image;
}


void OBJObjectAO::SetAOVariableLocations(int aoOpacity, int aoColor) {
    aoOpacityLocation = aoOpacity;
    aoColorLocation = aoColor;
}


void OBJObjectAO::DilateGeometry(bool dilate) {
    dilateGeometry = dilate;
}


void OBJObjectAO::ExpandPoints(double expansion) {
	for (int i = 0; i < (int)verts.size(); i++) {
        verts[i] += vertexNormals[i] * expansion * pow(1.0 - aoDiffusion[i], 1);
    }	
}


float OBJObjectAO::OtsuThreshold(const std::vector<float>& values) {
    int size = values.size();

    // Create histogram
    int histogram[256];
    memset(histogram, 0, 256 * sizeof(int));

    for (int i = 0; i < (int)values.size(); i++) {
        histogram[(int)(values[i] * 255.0)]++;
    }


    // Compute Otsu threshold
    float  w = 0;                // first order cumulative
    float  u = 0;                // second order cumulative
    float  uT = 0;               // total mean level

    int    k = 255;              // maximum histogram index
    int    threshold = 0;        // optimal threshold value

    float  histNormalized[256];  // normalized histogram values

    float  work1, work2;		// working variables
    double work3 = 0.0;


    // Create normalised histogram values
    // (size=image width * image height)
    for (int I=1; I<=k; I++) 
    histNormalized[I-1] = histogram[I-1]/(float)size;


    // Calculate total mean level
    for (int I=1; I<=k; I++) 
    uT+=(I*histNormalized[I-1]);


    // Find optimal threshold value
    for (int I=1; I<k; I++) {
        w+=histNormalized[I-1];
        u+=(I*histNormalized[I-1]);
        work1 = (uT * w - u);
        work2 = (work1 * work1) / ( w * (1.0f-w) );
        if (work2>work3) work3=work2;
    }


    // Convert the final value to an integer
    return (float)(1.0 - (sqrt(work3) / 255.0));
}


void OBJObjectAO::RenderGeometry() {  
    if (dilateGeometry) {
        RenderDilated();
        return;
    }




    // In case there is no ambient occlusion data
    if (aoValues.size() != verts.size()) {
        aoValues.resize(verts.size());
        for (int i = 0; i < (int)aoValues.size(); i++) aoValues[i] = 0.0;
    }
    if (aoDiffusion.size() != verts.size()) {
        aoDiffusion.resize(verts.size());
        for (int i = 0; i < (int)aoDiffusion.size(); i++) aoDiffusion[i] = 0.0;
    }

    if (depths.size() == 0) return;

    SubGroup* subGroup = depths[0].subGroup;
    subGroup->GetMaterial()->SetupRender();
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, texture3D);
    for (int i = 0; i < (int)depths.size(); i++) {   
        if (depths[i].subGroup != subGroup) {
            subGroup = depths[i].subGroup;
            subGroup->GetMaterial()->SetupRender();
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_3D);
            glBindTexture(GL_TEXTURE_3D, texture3D);
        }
        Face* face = depths[i].face;
        if (wireFrame) {
            glBegin(GL_LINES);
        }
        else {
            glBegin(GL_POLYGON);
        }

        // Face normal
        if (!smoothShading || face->NumVertexNormals() == 0) {
            glNormal3f((GLfloat)face->GetNormal().X(),
                       (GLfloat)face->GetNormal().Y(),
                       (GLfloat)face->GetNormal().Z());
        }

        // Draw each vertex
        for (int v = 0; v < face->NumVertices(); v++) {
            // Vertex normals
            if (v < face->NumVertexNormals() && smoothShading) {
                int index = face->GetVertexNormalIndex(v);
                glNormal3f((GLfloat)vertexNormals[index].X(), 
                           (GLfloat)vertexNormals[index].Y(), 
                           (GLfloat)vertexNormals[index].Z());
            }

            // Texture coordinates
            if (v < face->NumTextureCoords()) {
                int index = face->GetTextureCoordIndex(v);
                glTexCoord3f((GLfloat)(textureCoords[index].X() * subGroup->GetTextureScale().X()),
                             (GLfloat)(textureCoords[index].Y() * subGroup->GetTextureScale().Y()),
                             (GLfloat)(textureCoords[index].Z() * subGroup->GetTextureScale().Z()));

                printf("%f\n", textureCoords[index].X() * subGroup->GetTextureScale().X());
            }



            // Ambient occlusion
/*
            float ao;
            float red;
            if (diffusion) {
                ao = aoDiffusion[face->GetVertexIndex(v)];
                red = aoValues[face->GetVertexIndex(v)];
            }
            else {
                ao = aoValues[face->GetVertexIndex(v)];
                red = ao;
            }
*/

            float ao = aoValues[face->GetVertexIndex(v)];
            float aoSmooth = aoDiffusion[face->GetVertexIndex(v)];

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            // HACK:
//            glTexCoord3f(red, red, red);

            // Color 
            if (face->GetVertexIndex(v) < (int)vertexColors.size()) {
                Vec3 colorValue = vertexColors[face->GetVertexIndex(v)];
//                glColor4f((float)colorValue.X(), (float)colorValue.Y(), (float)colorValue.Z(), ao);
//                glColor4f(red, (float)colorValue.Y(), (float)colorValue.Z(), ao);
                glColor3f((float)colorValue.X(), (float)colorValue.Y(), (float)colorValue.Z());
            }
            else {
//                glColor4f(1.0, 1.0, 1.0, ao);
                glColor3f(1.0, 1.0, 1.0);
            }

            // Position
            int index = face->GetVertexIndex(v);
            glVertex3f((GLfloat)verts[index].X(), 
                       (GLfloat)verts[index].Y(), 
                       (GLfloat)verts[index].Z());
        }
        glEnd();
    }
}


void OBJObjectAO::RenderDilated() {  
    double dilation = 0.2;

    float dilatedAO = 1.0;


    // In case there is no ambient occlusion data
    if (aoValues.size() != verts.size()) {
        aoValues.resize(verts.size());
        for (int i = 0; i < (int)aoValues.size(); i++) aoValues[i] = 0.0;
    }
    if (aoDiffusion.size() != verts.size()) {
        aoDiffusion.resize(verts.size());
        for (int i = 0; i < (int)aoDiffusion.size(); i++) aoDiffusion[i] = 0.0;
    }

    // XXX: Don't actually need depth sorting, but it will work
    if (depths.size() == 0) return;



    for (int i = 0; i < (int)depths.size(); i++) {   
        Face* face = depths[i].face;
    
        std::vector<Vec3> dilatedVerts(face->NumVertices());
        std::vector<Vec3> erodedVerts(face->NumVertices());

        // Set up vertices
        for (int v = 0; v < face->NumVertices(); v++) {
            Vec3 vertex = verts[face->GetVertexIndex(v)];
            Vec3 normal = vertexNormals[face->GetVertexNormalIndex(v)];

            dilatedVerts[v] = vertex + normal * dilation;
            erodedVerts[v] = vertex - normal * dilation;
        }

/*
        // Draw original face
        glBegin(GL_POLYGON);
        for (int v = 0; v < face->NumVertices(); v++) {
            int index = face->GetVertexIndex(v);

            // Ambient occlusion
            float ao = aoValues[index];
            float aoSmooth = aoDiffusion[index];

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            // Vertex
            glVertex3f((GLfloat)verts[index].X(), (GLfloat)verts[index].Y(), (GLfloat)verts[index].Z());
        }
        glEnd();
*/

        // Draw dilated wedges
        glBegin(GL_POLYGON);
        for (int v = 0; v < face->NumVertices(); v++) {
            int v2 = v == face->NumVertices() - 1 ? 0 : v + 1;

            int index1 = face->GetVertexIndex(v);
            int index2 = face->GetVertexIndex(v2);

            float ao;
            float aoSmooth;


            // Original vertex 1
            ao = aoValues[index1];
            aoSmooth = aoDiffusion[index1];

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            glVertex3f((GLfloat)verts[index1].X(), (GLfloat)verts[index1].Y(), (GLfloat)verts[index1].Z());


            // Original vertex 2
            ao = aoValues[index2];
            aoSmooth = aoDiffusion[index2];

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            glVertex3f((GLfloat)verts[index2].X(), (GLfloat)verts[index2].Y(), (GLfloat)verts[index2].Z());


            // Dilated vertices            
//            glVertexAttrib1f(aoOpacityLocation, dilatedAO);
//            glVertexAttrib1f(aoColorLocation, dilatedAO);
            glVertexAttrib1f(aoOpacityLocation, aoDiffusion[index1] + (1.0f - aoDiffusion[index1]) * dilatedAO);
            glVertexAttrib1f(aoColorLocation, aoValues[index1] + (1.0f - aoValues[index1]) * dilatedAO);

            glVertex3f((GLfloat)dilatedVerts[v].X(), (GLfloat)dilatedVerts[v].Y(), (GLfloat)dilatedVerts[v].Z());

//            glVertexAttrib1f(aoOpacityLocation, dilatedAO);
//            glVertexAttrib1f(aoColorLocation, dilatedAO);
            glVertexAttrib1f(aoOpacityLocation, aoDiffusion[index2] + (1.0f - aoDiffusion[index2]) * dilatedAO);
            glVertexAttrib1f(aoColorLocation, aoValues[index2] + (1.0 - aoValues[index2]) * dilatedAO);

            glVertex3f((GLfloat)dilatedVerts[v2].X(), (GLfloat)dilatedVerts[v2].Y(), (GLfloat)dilatedVerts[v2].Z());
        }
        glEnd();

/*
        // Draw dilated cap        
        glBegin(GL_POLYGON);
        for (int v = 0; v < (int)dilatedVerts.size(); v++) {
            // Ambient occlusion
//            glVertexAttrib1f(aoOpacityLocation, dilatedAO);
//            glVertexAttrib1f(aoColorLocation, dilatedAO);

            int index = face->GetVertexIndex(v);

            // Ambient occlusion
            float ao = aoValues[index] + (1.0f - aoValues[index]) * dilatedAO;
            float aoSmooth = aoDiffusion[index] + (1.0f - aoDiffusion[index]) * dilatedAO;

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            // Vertex
            glVertex3f((GLfloat)dilatedVerts[v].X(), (GLfloat)dilatedVerts[v].Y(), (GLfloat)dilatedVerts[v].Z());
        }
        glEnd();
*/

        // Draw eroded wedges
        glBegin(GL_POLYGON);
        for (int v = 0; v < face->NumVertices(); v++) {
            int v2 = v == face->NumVertices() - 1 ? 0 : v + 1;

            int index1 = face->GetVertexIndex(v);
            int index2 = face->GetVertexIndex(v2);

            float ao;
            float aoSmooth;


            // Original vertex 1
            ao = aoValues[index1];
            aoSmooth = aoDiffusion[index1];

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            glVertex3f((GLfloat)verts[index1].X(), (GLfloat)verts[index1].Y(), (GLfloat)verts[index1].Z());


            // Original vertex 2
            ao = aoValues[index2];
            aoSmooth = aoDiffusion[index2];

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            glVertex3f((GLfloat)verts[index2].X(), (GLfloat)verts[index2].Y(), (GLfloat)verts[index2].Z());


            // Eroded vertices            
//            glVertexAttrib1f(aoOpacityLocation, dilatedAO);
//            glVertexAttrib1f(aoColorLocation, dilatedAO);
            glVertexAttrib1f(aoOpacityLocation, aoDiffusion[index1] + (1.0f - aoDiffusion[index1]) * dilatedAO);
            glVertexAttrib1f(aoColorLocation, aoValues[index1] + (1.0f - aoValues[index1]) * dilatedAO);

            glVertex3f((GLfloat)erodedVerts[v].X(), (GLfloat)erodedVerts[v].Y(), (GLfloat)erodedVerts[v].Z());

//            glVertexAttrib1f(aoOpacityLocation, dilatedAO);
//            glVertexAttrib1f(aoColorLocation, dilatedAO);
            glVertexAttrib1f(aoOpacityLocation, aoDiffusion[index2] + (1.0f - aoDiffusion[index2]) * dilatedAO);
            glVertexAttrib1f(aoColorLocation, aoValues[index2] + (1.0f - aoValues[index2]) * dilatedAO);

            glVertex3f((GLfloat)erodedVerts[v2].X(), (GLfloat)erodedVerts[v2].Y(), (GLfloat)erodedVerts[v2].Z());
        }
        glEnd();

/*
        // Draw eroded cap        
        glBegin(GL_POLYGON);
        for (int v = 0; v < (int)erodedVerts.size(); v++) {
            // Ambient occlusion
//            glVertexAttrib1f(aoOpacityLocation, dilatedAO);
//            glVertexAttrib1f(aoColorLocation, dilatedAO);

            int index = face->GetVertexIndex(v);

            // Ambient occlusion
            float ao = aoValues[index] + (1.0f - aoValues[index]) * dilatedAO;
            float aoSmooth = aoDiffusion[index] + (1.0f - aoDiffusion[index]) * dilatedAO;

            if (diffusion) {
                glVertexAttrib1f(aoOpacityLocation, aoSmooth);
                glVertexAttrib1f(aoColorLocation, ao);
            }
            else {
                glVertexAttrib1f(aoOpacityLocation, ao);
                glVertexAttrib1f(aoColorLocation, aoSmooth);
            }

            // Vertex
            glVertex3f((GLfloat)erodedVerts[v].X(), (GLfloat)erodedVerts[v].Y(), (GLfloat)erodedVerts[v].Z());
        }
        glEnd();
*/
    }  
}