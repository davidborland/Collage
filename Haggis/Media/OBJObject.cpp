///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        OBJObject.cpp
//
// Author:      David Borland
//
// Description: Class for loading geometry from a .obj file
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "OBJObject.h"

#include "Utilities.h"

#include <IL/il.h>
#include <IL/ilu.h>

#include <fstream>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////////////////////


Material::Material(const std::string& materialName) : name(materialName) {
    ambient[0] = 0.1f;
    ambient[1] = 0.1f;
    ambient[2] = 0.1f;
    ambient[3] = 1.0f;

    diffuse[0] = 0.5f;
    diffuse[1] = 0.5f;
    diffuse[2] = 0.5f;
    diffuse[3] = 1.0f;

    specular[0] = 1.0f;
    specular[1] = 1.0f;
    specular[2] = 1.0f;
    specular[3] = 1.0f;

    shininess = 10.0f;

    doSpecular = false;

    hasTexture = false;
    textureScale.Set(1.0, 1.0, 1.0);
}

Material::~Material() {
    glDeleteTextures(1, &texture); 
}


void Material::SetAmbient(float r, float g, float b) {
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
}

void Material::SetDiffuse(float r, float g, float b) {
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
}

void Material::SetSpecular(float r, float g, float b) {
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
}

void Material::SetOpacity(float opacity) {
    ambient[3] = opacity;
    diffuse[3] = opacity;
    specular[3] = opacity;
}

void Material::SetShininess(float shiny) {
    shininess = shiny;
}


void Material::SetDoSpecular(bool flag) {
    doSpecular = flag;
}


bool Material::LoadTexture(const std::string& fileName) {
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);

    if (!ilLoadImage(fileName.c_str())) {
        std::cout << "Material::LoadTexture() : Loading " << fileName.c_str() << " failed" << std::endl;

        ilDeleteImages(1, &image);

        return false;
    }


    // Get image info
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);
    int ilPixelFormat = ilGetInteger(IL_IMAGE_FORMAT);


    // Check the pixel format
    if (ilPixelFormat == IL_LUMINANCE) {
        ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    }
    else if (ilPixelFormat == IL_RGB) {
        ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

        // Looks like non-alpha images are flipped in y?
        iluFlipImage();
    }
    else if (ilPixelFormat == IL_RGBA) {
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    }
    else {
        // Something ain't right...
        std::cout << "Material::LoadTexture() : Invalid number of components" << std::endl;

        ilDeleteImages(1, &image);   

        return false;
    }


    // Create the texture
    glGenTextures(1, &texture);    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Create the texture
    if (ilPixelFormat == IL_LUMINANCE) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, ilGetData());
    }   
    else if (ilPixelFormat == IL_RGB) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    }
    else if (ilPixelFormat == IL_RGBA) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
    }

    ilDeleteImages(1, &image);

    hasTexture = true;

    return true;
}


void Material::SetTextureScale(const Vec3& scale) {
    textureScale = scale;
}


const std::string& Material::GetName() {
    return name;
}   


void Material::SetupRender() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    if (doSpecular) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
    }
    else {
        float black[3] = { 0.0, 0.0, 0.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
    }

    if (hasTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    if (ambient[3] < 1.0 || diffuse[3] < 1.0 || specular[3] < 1.0) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);
}


const Vec3& Material::GetTextureScale() {
    return textureScale;
}


///////////////////////////////////////////////////////////////////////////////////////////////


Face::Face() {
    normal = Vec3(0.0, 1.0, 0.0);
}

Face::~Face() {
}


void Face::SetNormal(const Vec3& norm) {
    normal = norm;
}

const Vec3& Face::GetNormal() {
    return normal;
}


int Face::NumVertices() {
    return (int)vertexIndeces.size();
}

int Face::GetVertexIndex(int i) {
    return vertexIndeces[i];
}


int Face::NumTextureCoords() {
    return (int)textureCoordIndeces.size();
}

int Face::GetTextureCoordIndex(int i) {
    return textureCoordIndeces[i];
}


int Face::NumVertexNormals() {
    return (int)vertexNormalIndeces.size();
}

int Face::GetVertexNormalIndex(int i) {
    return vertexNormalIndeces[i];
}


void Face::AddVertexIndex(int index) {
    vertexIndeces.push_back(index);
}

void Face::AddTextureCoordIndex(int index) {
    textureCoordIndeces.push_back(index);
}

void Face::AddVertexNormalIndex(int index) {
    vertexNormalIndeces.push_back(index);
}


///////////////////////////////////////////////////////////////////////////////////////////////


SubGroup::SubGroup() {
    material = NULL;
}

SubGroup::~SubGroup() {
}


int SubGroup::NumFaces() {
    return (int)faces.size();
}

Face* SubGroup::GetFace(int i) {
    return faces[i];
}


Face* SubGroup::AddFace() {
    faces.push_back(new Face());
    return faces.back();
}


void SubGroup::SetMaterial(Material* mat) {
    material = mat;
}

Material* SubGroup::GetMaterial() {
    return material;
}


Vec3 SubGroup::GetTextureScale() {
    if (material) return material->GetTextureScale();
    else return Vec3(1.0, 1.0, 1.0);
}


///////////////////////////////////////////////////////////////////////////////////////////////


Group::Group(const std::string& groupName) : name(groupName) {
}

Group::~Group() {
    for (int i = 0; i < (int)subGroups.size(); i++) {
        delete subGroups[i];
    }
}


const std::string& Group::GetName() {
    return name;
}


int Group::NumSubGroups() {
    return (int)subGroups.size();
}

SubGroup* Group::GetSubGroup(int i) {
    return subGroups[i];
}


SubGroup* Group::AddSubGroup() {
    subGroups.push_back(new SubGroup());
    return subGroups.back();
}



///////////////////////////////////////////////////////////////////////////////////////////////


bool DepthGreaterThan(const FaceDepth& face1, const FaceDepth& face2) {
    return face1.depth > face2.depth;
}


///////////////////////////////////////////////////////////////////////////////////////////////


bool OBJObject::initializedDevIL = false;


OBJObject::OBJObject() : RenderObject() {
    currentGroup = NULL;
	currentSubGroup = NULL;
    currentMaterial = NULL;

    smoothShading = true;
    wireFrame = false;
    immediateMode = false;
    cullFace = true;

    displayListCurrent = false;

    depthSort = false;
}

OBJObject::~OBJObject() {
    for (int i = 0; i < (int)groups.size(); i++) {
        delete groups[i];
    }

    for (int i = 0; i < (int)materials.size(); i++) {
        delete materials[i];
    }

    glDeleteLists(displayList, 1);
}


bool OBJObject::LoadObject(const std::string& fileName) {
    if (!initializedDevIL) {       
       ilInit();
       initializedDevIL = true;
    }


	std::ifstream file(fileName.c_str());
    if (file.fail()) {
        std::cout << "OBJObject::LoadObject() : Couldn't open " << fileName.c_str() << std::endl;
        return false;
    }

    std::cout << "OBJObject::LoadObject() : Loading " << fileName.c_str() << std::endl;

    std::string mtlFileName;

	std::string s;
    std::vector<std::string> tokens;

    int numFaces = 0;

	while (!file.eof()) {
        getline(file, s);
        tokens = Utilities::Tokenize(s, " \n");

        if (tokens.size() > 0) {
		    if (tokens[0] == "#") {
			    // Comment
		    }
		    else if (tokens[0] == "mtllib") {
			    // Material library
                mtlFileName = ResolveFileName(fileName, tokens[1]);

                if (!ParseMtl(mtlFileName)) {
                    std::cout << "OBJObject::LoadObject() : Error loading " << tokens[1] << std::endl;
                }
		    }
            else if (tokens[0] == "g") {
                // Group               
                if (tokens.size() == 1) {
                    AddGroup("");
                }
                else if (tokens.size() == 2) {
                    AddGroup(tokens[1]);
                }
                else {
                    std::cout << "OBJObject::LoadObject() : Invalid group " << s << std::endl;
                }
            }
            else if (tokens[0] == "v") {
                if (tokens.size() == 4) {
                    AddVertex(Vec3(atof(tokens[1].c_str()), 
                                   atof(tokens[2].c_str()),
                                   atof(tokens[3].c_str())));
                }
                else {
                    std::cout << "OBJObject::LoadObject() : Invalid vertex : " << s << std::endl;
                }
            }           
            else if (tokens[0] == "vt") {
                if (tokens.size() == 3) {
                    AddTextureCoord(Vec3(atof(tokens[1].c_str()), 
                                           atof(tokens[2].c_str()),
                                           0.0));
                }
                else if (tokens.size() == 4) {
                    AddTextureCoord(Vec3(atof(tokens[1].c_str()), 
                                           atof(tokens[2].c_str()),
                                           atof(tokens[3].c_str())));
                }
                else {
                    std::cout << "OBJObject::LoadObject() : Invalid texture vertex : " << s << std::endl;
                }
            }   
            else if (tokens[0] == "vn") {
                if (tokens.size() == 4) {
                    AddVertexNormal(Vec3(atof(tokens[1].c_str()), 
                                         atof(tokens[2].c_str()),
                                         atof(tokens[3].c_str())));
                }
                else {
                    std::cout << "OBJObject::LoadObject() : Invalid vertex normal : " << s << std::endl;
                }
            }
            else if (tokens[0] == "usemtl") {
                if (tokens.size() == 2) {
                    std::string materialName = tokens[1];
                    bool found = false;
                    for (int i = 0; i < (int)materials.size(); i++) {
                        if (materials[i]->GetName() == materialName) {
                            if (!currentGroup) {
                                AddGroup("default");
                            }
                            currentSubGroup = currentGroup->AddSubGroup();
                            currentSubGroup->SetMaterial(materials[i]);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        std::cout << "OBJObject::LoadObject() : Could not find material : " << s << std::endl;
                    }
                }
                else {
                    std::cout << "OBJObject::LoadObject() : Invalid material : " << s << std::endl;
                }
            }
            else if (tokens[0] == "f") {
                if (tokens.size() >= 4) {
                    if (!currentGroup) {
                        AddGroup("default");
                    }
                    Face* face = currentSubGroup->AddFace();
                    numFaces++;
                    for (int i = 1; i < (int)tokens.size(); i++) {
                        // Count number of forward slashes
                        size_t pos = -1;
                        int count = 0;
                        while ((pos = tokens[i].find("/", pos + 1)) != std::string::npos) count++;
    
                        if (count > 0) {
                            // Tokenize based on forward slashes
                            std::vector<std::string> vertexTokens;
                            vertexTokens = Utilities::Tokenize(tokens[i], "/");

                            // Determine what information is here
                            if (count == 1) {
                                if (vertexTokens.size() == 2) {
                                    int vertexIndex = atoi(vertexTokens[0].c_str());
                                    int textureCoordIndex = atoi(vertexTokens[1].c_str());

                                    if (vertexIndex < 1) vertexIndex += (int)verts.size();
                                    else vertexIndex--;

                                    if (textureCoordIndex < 1) textureCoordIndex += (int)textureCoords.size();
                                    else textureCoordIndex--;

                                    face->AddVertexIndex(vertexIndex);
                                    face->AddTextureCoordIndex(textureCoordIndex);
                                }
                                else {
                                    std::cout << "OBJObject::LoadObject() : Invalid face : " << s << std::endl;
                                }
                            }
                            else if (count == 2) {
                                if (vertexTokens.size() == 2) {  
                                    int vertexIndex = atoi(vertexTokens[0].c_str());
                                    int vertexNormalIndex = atoi(vertexTokens[1].c_str());

                                    if (vertexIndex < 1) vertexIndex += (int)verts.size();
                                    else vertexIndex--;

                                    if (vertexNormalIndex < 1) vertexNormalIndex += (int)vertexNormals.size();
                                    else vertexNormalIndex--;

                                    face->AddVertexIndex(vertexIndex);
                                    face->AddVertexNormalIndex(vertexNormalIndex);
                                }
                                else if (vertexTokens.size() == 3) { 
                                    int vertexIndex = atoi(vertexTokens[0].c_str());
                                    int textureCoordIndex = atoi(vertexTokens[1].c_str());
                                    int vertexNormalIndex = atoi(vertexTokens[2].c_str());

                                    if (vertexIndex < 1) vertexIndex += (int)verts.size();
                                    else vertexIndex--;
                                                                        
                                    if (textureCoordIndex < 1) textureCoordIndex += (int)textureCoords.size();
                                    else textureCoordIndex--;

                                    if (vertexNormalIndex < 1) vertexNormalIndex += (int)vertexNormals.size();
                                    else vertexNormalIndex--;

                                    face->AddVertexIndex(vertexIndex);
                                    face->AddTextureCoordIndex(textureCoordIndex);
                                    face->AddVertexNormalIndex(vertexNormalIndex);
                                }
                                else {
                                    std::cout << "OBJObject::LoadObject() : Invalid face : " << s << std::endl;
                                }
                            }
                            else {
                                std::cout << "OBJObject::LoadObject() : Invalid face : " << s << std::endl;
                            }
                        }
                        else {
                            // No forward slashes, only vertexint vertexIndex = atoi(vertexTokens[0].c_str());
                            int vertexIndex = atoi(tokens[i].c_str());

                            if (vertexIndex < 1) vertexIndex += (int)verts.size();
                            else vertexIndex--;

                            face->AddVertexIndex(vertexIndex);
                        }
                    }
                }
                else {
                    std::cout << "OBJObject::LoadObject() : Invalid face: " << s << std::endl;
                }
            }
        }
	}

    file.close();

    std::cout << "OBJObject::LoadObject() : Number of faces = " << numFaces << std::endl;

    // Calculate face normals
    std::cout << "OBJObject::LoadObject() : Calculating face normals...";
    CalculateFaceNormals();
    std::cout << "done." << std::endl;


    // If needed, calculate vertex normals
    if ((int)vertexNormals.size() == 0) {
        std::cout << "OBJObject::LoadObject() : Calculating vertex normals...";
        CalculateVertexNormals();
        std::cout << "done." << std::endl << std::endl;
    }

    // Make sure everone has a material
    srand(1);
    for (int i = 0; i < (int)groups.size(); i++) {
        Group* group = groups[i];
        for (int j = 0; j < group->NumSubGroups(); j++) {
            SubGroup* subGroup = group->GetSubGroup(j);
            if (!subGroup->GetMaterial()) {
                Material* material = new Material(group->GetName());
                materials.push_back(material); 

                float r = (float)rand() / (float)RAND_MAX;
                float g = (float)rand() / (float)RAND_MAX;
                float b = (float)rand() / (float)RAND_MAX;

                material->SetAmbient(r, g, b);
                material->SetDiffuse(r, g, b);
                material->SetSpecular(1.0f, 1.0f, 1.0f);
                material->SetShininess(50.0f);
                material->SetDoSpecular(true);
                material->SetOpacity(1.0f);

                subGroup->SetMaterial(material);
            }
        }
    }

	return true;
}


double OBJObject::NormalizeBounds(double scaleBounds) {    
    Vec3 min;
    Vec3 max;

    if (verts.size() > 0) {
        min = verts[0];
        max = verts[0];
    }

    for (int i = 1; i < (int)verts.size(); i++) {
        if (verts[i].X() < min.X()) min.X() = verts[i].X();
        if (verts[i].Y() < min.Y()) min.Y() = verts[i].Y();
        if (verts[i].Z() < min.Z()) min.Z() = verts[i].Z();

        if (verts[i].X() > max.X()) max.X() = verts[i].X();
        if (verts[i].Y() > max.Y()) max.Y() = verts[i].Y();
        if (verts[i].Z() > max.Z()) max.Z() = verts[i].Z();
    }

    double x = max.X() - min.X();
    double y = max.Y() - min.Y();
    double z = max.Z() - min.Z();

    double s = x > y ? x : y;
    s = s > z ? s : z;
    s = 1.0 / s;
    s *= scaleBounds;

    Vec3 center(min.X() + x * 0.5,
                min.Y() + y * 0.5,
                min.Z() + z * 0.5);

    for (int i = 0; i < (int)verts.size(); i++) {
        verts[i] -= center;
        verts[i] *= s;
    }

    displayListCurrent = false;


    std::cout << center << std::endl;
    std::cout << s << std::endl;

    return s;
}


void OBJObject::SmoothShadingOn() {
    smoothShading = true;
    displayListCurrent = false;
}

void OBJObject::SmoothShadingOff() {
    smoothShading = false;
    displayListCurrent = false;
}


void OBJObject::WireFrameOn() {
    wireFrame = true;
    displayListCurrent = false;
}

void OBJObject::WireFrameOff() {
    wireFrame = false;
    displayListCurrent = false;
}


void OBJObject::ImmediateModeOn() {
    immediateMode = true;
}

void OBJObject::ImmediateModeOff() {
    immediateMode = false;
}


void OBJObject::CullFaceOn() {
    cullFace = true;
}

void OBJObject::CullFaceOff() {
    cullFace = false;
}


void OBJObject::SetOpacity(float opacity) {
    for (int i = 0; i < (int)groups.size(); i++) {
        Group* group = groups[i];
        for (int j = 0; j < group->NumSubGroups(); j++) {
            group->GetSubGroup(j)->GetMaterial()->SetOpacity(opacity);
        }
    }
    displayListCurrent = false;
}


void OBJObject::TranslatePoints(const Vec3& vec) {
    for (int i = 0; i < (int)verts.size(); i++) {
        verts[i] += vec;
    }
}

void OBJObject::RotatePoints(const Quat& quat) {
    for (int i = 0; i < (int)verts.size(); i++) {
        verts[i] = quat * verts[i];
    }

    for (int i = 0; i < (int)vertexNormals.size(); i++) {
        vertexNormals[i] = quat * vertexNormals[i];
    }

    CalculateFaceNormals();
}

void OBJObject::ScalePoints(double scaleValue) {
    for (int i = 0; i < (int)verts.size(); i++) {
        verts[i] *= scaleValue;
    }
}

void OBJObject::ExpandPoints(double expansion) {
	for (int i = 0; i < (int)verts.size(); i++) {
        verts[i] += vertexNormals[i] * expansion;
    }	
}


void OBJObject::SetCameraPosition(const Vec3& position) {
    cameraPosition = position;
}


void OBJObject::DepthSortOn() {
    depthSort = true;
}

void OBJObject::DepthSortOff() {
    depthSort = false;
}


bool OBJObject::ParseMtl(const std::string& fileName) {
	std::ifstream file(fileName.c_str());
    if (file.fail()) {
        std::cout << "OBJObject::ParseMtl() : Couldn't open " << fileName.c_str() << std::endl;
        return false;
    }

    std::cout << "OBJObject::ParseMtl() : Loading " << fileName.c_str() << std::endl;

	std::string s;
    std::vector<std::string> tokens;

	while (!file.eof()) {
        getline(file, s);
        tokens = Utilities::Tokenize(s, " \n");

        if (tokens.size() > 0) {
		    if (tokens[0] == "#") {
			    // Comment
		    }
		    else if (tokens[0] == "newmtl") {
			    // New material
                if (tokens.size() == 2) {
                    AddMaterial(tokens[1]);
                }
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid material name : " << s << std::endl;
                } 
		    }
            else if (tokens[0] == "Ka") {
                // Ambient
                if (tokens.size() == 4) {                    
                    currentMaterial->SetAmbient((float)atof(tokens[1].c_str()),
                                                (float)atof(tokens[2].c_str()),
                                                (float)atof(tokens[3].c_str()));
                }   
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid ambient color : " << s << std::endl;
                }
            }
            else if (tokens[0] == "Kd") {
                // Diffuse
                if (tokens.size() == 4) {                    
                    currentMaterial->SetDiffuse((float)atof(tokens[1].c_str()),
                                                (float)atof(tokens[2].c_str()),
                                                (float)atof(tokens[3].c_str()));
                }   
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid diffuse color : " << s << std::endl;
                }
            }
            else if (tokens[0] == "Ks") {
                // Specular
                if (tokens.size() == 4) {                    
                    currentMaterial->SetSpecular((float)atof(tokens[1].c_str()),
                                                 (float)atof(tokens[2].c_str()),
                                                 (float)atof(tokens[3].c_str()));
                }   
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid specular color : " << s << std::endl;
                }
            }
            else if (tokens[0] == "d") {
                // Opacity
                if (tokens.size() == 2) {
                    currentMaterial->SetOpacity((float)atof(tokens[1].c_str()));
                }
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid opacity : " << s << std::endl;
                }
            }
            else if (tokens[0] == "Tr") {
                // Opacity
                if (tokens.size() == 2) {
                    currentMaterial->SetOpacity((float)atof(tokens[1].c_str()));
                }
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid opacity : " << s << std::endl;
                }
            }
            else if (tokens[0] == "Ns") {
                // Shininess
                if (tokens.size() == 2) {
                    currentMaterial->SetShininess((float)atof(tokens[1].c_str()));
                }
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid shininess : " << s << std::endl;
                }
            }
            else if (tokens[0] == "illum") {
                // Illumination model (specular or not)
                if (tokens.size() == 2) {
                    int illumination = atoi(tokens[1].c_str());

                    if (illumination == 1) {
                        currentMaterial->SetDoSpecular(false);
                    }
                    else if (illumination == 2) {
                        currentMaterial->SetDoSpecular(true);
                    }
                    else {
                        std::cout << "OBJObject::ParseMtl() : Invalid illumination model : " << s << std::endl;
                    }
                }
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid illumination model : " << s << std::endl;
                }
            }
            else if (tokens[0] == "map_Kd") {
                // Texture map
                if (tokens.size() == 2) {
                    std::string textureFileName = ResolveFileName(fileName, tokens[1]);
                    if (!currentMaterial->LoadTexture(textureFileName)) {
                        std::cout << "OBJObject::ParseMtl() : Could not load texture : " << textureFileName << std::endl;
                    }
                }
                else if (tokens.size() == 6) {
                    if (tokens[1] == "-s") {
                        std::string textureFileName = ResolveFileName(fileName, tokens[5]);
                        if (!currentMaterial->LoadTexture(textureFileName)) {
                            std::cout << "OBJObject::ParseMtl() : Could not load texture : " << textureFileName << std::endl;
                        }

                        currentMaterial->SetTextureScale(Vec3(atof(tokens[2].c_str()),
                                                              atof(tokens[3].c_str()),
                                                              atof(tokens[4].c_str())));                 
                    }
                    else {
                        std::cout << "OBJObject::ParseMtl() : Invalid texture parameter: " << s << std::endl;
                    }
                }
                else {
                    std::cout << "OBJObject::ParseMtl() : Invalid texture : " << s << std::endl;
                }
            }
        }
    }

    file.close();

    return true;
}


std::string OBJObject::ResolveFileName(const std::string& parent, const std::string& child) {
    std::string p = parent;
    std::string c = child;

    bool relative = false;
    if (c[0] == '.' && (c[1] == '/' || c[1] == '\\')) {
        c = c.substr(2);
        relative = true;
    }
    else if (c.find('/') == std::string::npos && c.find('\\') == std::string::npos) {
        relative = true;
    }

    if (relative) {
        // Relative path
        std::string path;

        size_t found = p.rfind('/');
        if (found != std::string::npos) {
            path = p.substr(0, found + 1);
        }
        else {
            found = p.rfind('\\');
            if (found != std::string::npos) {
                path = p.substr(0, found + 1);
            }
        }

        c = path + c;
    }

    return c;
}


void OBJObject::AddGroup(const std::string& name) {
    for (int i = 0; i < (int)groups.size(); i++) {
        if (name == groups[i]->GetName()) {
            currentGroup = groups[i];
            return;
        }
    }

    // Didn't find it, so add it
    groups.push_back(new Group(name));
    currentGroup = groups.back();
    currentSubGroup = currentGroup->AddSubGroup();
}


void OBJObject::AddMaterial(const std::string& name) {
    for (int i = 0; i < (int)materials.size(); i++) {
        if (name == materials[i]->GetName()) {
            currentMaterial = materials[i];
            return;
        }
    }

    // Didn't find it, so add it
    materials.push_back(new Material(name));
    currentMaterial = materials.back();
}


void OBJObject::AddVertex(const Vec3& v) {
    verts.push_back(v);
}

void OBJObject::AddTextureCoord(const Vec3& vt) {
    textureCoords.push_back(vt);
}

void OBJObject::AddVertexNormal(const Vec3& vn) {
    vertexNormals.push_back(vn);
}


void OBJObject::CalculateFaceNormals() {
    for (int i = 0; i < (int)groups.size(); i++) {
        Group* group = groups[i];
        for (int j = 0; j < (int)group->NumSubGroups(); j++) {
            SubGroup* subGroup = group->GetSubGroup(j);
            for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                Face* face = subGroup->GetFace(k);

                if (face->NumVertices() >= 3) {
                    Vec3 p0, p1, p2;
                    Vec3 v0, v1;
                    Vec3 normal;

                    p0 = verts[face->GetVertexIndex(0)];
                    p1 = verts[face->GetVertexIndex(1)];
                    p2 = verts[face->GetVertexIndex(2)];

                    v0 = p0 - p1;
                    v1 = p2 - p1;

                    normal = v1 * v0;
                    normal.Normalize();

                    face->SetNormal(normal);
                }
                else {
                    std::cout << "OBJObject::CalculateFaceNormals() : group " << group->GetName() << ", subgroup " << j << ", face " << k << " has < 3 vertices." << std::endl;
                }
            }
        }
    }
}

void OBJObject::CalculateVertexNormals() {
    vertexNormals.resize(verts.size());

    for (int i = 0; i < (int)groups.size(); i++) {
        Group* group = groups[i];
        for (int j = 0; j < (int)group->NumSubGroups(); j++) {
            SubGroup* subGroup = group->GetSubGroup(j);
            for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                Face* face = subGroup->GetFace(k);

                for (int v = 0; v < (int)face->NumVertices(); v++) {
                    // Add face normals, to be normalized later
                    vertexNormals[face->GetVertexIndex(v)] += face->GetNormal();

                    face->AddVertexNormalIndex(face->GetVertexIndex(v));
                }
            }
        }
    }

    for (int i = 0; i < (int)vertexNormals.size(); i++) {
        vertexNormals[i].Normalize();
    }
}

void OBJObject::PreRender() {
    glEnable(GL_LIGHTING);

    if (cullFace) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
}

void OBJObject::DoRender() {
    // Push the modelview matrix stack
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if (!immediateMode && !displayListCurrent) {
        // Need new display list
        if (glIsList(displayList) == GL_TRUE) {
            glDeleteLists(displayList, 1);
        }

        displayList = glGenLists(1);

        glNewList(displayList, GL_COMPILE);
        RenderGeometry();
        glEndList();

        displayListCurrent = true;
    }


    glTranslated(position.X(), position.Y(), position.Z());
    double angle;
    Vec3 axis;
    quaternion.GetAngleAxis(angle, axis);    
    glRotated(Quat::RadiansToDegrees(angle), axis.X(), axis.Y(), axis.Z());
    glScaled(scale, scale, scale);


    if (!immediateMode) {
        glCallList(displayList);
    }
    else {
        RenderGeometry();
    }

    // Pop the modelview matrix stack
    glPopMatrix();
}

void OBJObject::PostRender() {
    glDisable(GL_TEXTURE_2D);
}


void OBJObject::RenderGeometry() {
    if (depthSort) {    
        SubGroup* subGroup = depths[0].subGroup;
        subGroup->GetMaterial()->SetupRender();
        glEnable(GL_BLEND);
        for (int i = 0; i < (int)depths.size(); i++) { 
            if (depths[i].subGroup != subGroup) {
                subGroup = depths[i].subGroup;
                subGroup->GetMaterial()->SetupRender();
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
    else {
        for (int i = 0; i < (int)groups.size(); i++) {
            Group* group = groups[i];
            for (int j = 0; j < (int)group->NumSubGroups(); j++) {
                SubGroup* subGroup = group->GetSubGroup(j);
                subGroup->GetMaterial()->SetupRender();
                for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                    Face* face = subGroup->GetFace(k);
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
        }
    }
}

void OBJObject::DepthSort() {
    if (depths.size() == 0) { 
        for (int i = 0; i < (int)groups.size(); i++) {
            Group* group = groups[i];
            for (int j = 0; j < (int)group->NumSubGroups(); j++) {
                SubGroup* subGroup = group->GetSubGroup(j);
                for (int k = 0; k < (int)subGroup->NumFaces(); k++) {
                    FaceDepth depth;
                    depth.face = subGroup->GetFace(k);
                    depth.subGroup = subGroup;

                    depths.push_back(depth);
                }
            }
        }
    }

    for (int i = 0; i < (int)depths.size(); i++) {
        Face* face = depths[i].face;

        Vec3 center(0, 0, 0);
        for (int v = 0; v < face->NumVertices(); v++) {
            Vec3 pos = verts[face->GetVertexIndex(v)];

            pos *= scale;
            pos = quaternion * pos;
            pos += position;

            center += pos;
        }
        center *= 1.0 / face->NumVertices();
        depths[i].depth = (float)center.Distance(cameraPosition);
        depths[i].face = face;
    }

    std::sort(depths.begin(), depths.end(), DepthGreaterThan);
}