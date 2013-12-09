///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        OBJObject.h
//
// Author:      David Borland
//
// Description: Class for loading and rendering geometry from a .obj file
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef OBJOBJECT_H
#define OBJOBJECT_H


#include "RenderObject.h"

#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////////////////////


class Material {
public:
    Material(const std::string& materialName);
    ~Material();

    void SetAmbient(float r, float g, float b);
    void SetDiffuse(float r, float g, float b);
    void SetSpecular(float r, float g, float b);

    void SetOpacity(float opacity);
    void SetShininess(float shiny);

    void SetDoSpecular(bool flag);

    bool LoadTexture(const std::string& fileName);
    void SetTextureScale(const Vec3& scale);

    const std::string& GetName();
    const Vec3& GetTextureScale();

    void SetupRender();

private:
    std::string name;

    float ambient[4];
    float diffuse[4];
    float specular[4];

    float opacity;
    float shininess;

    bool doSpecular;

    GLuint texture;
    bool hasTexture;
    Vec3 textureScale;
};


///////////////////////////////////////////////////////////////////////////////////////////////


class Face {
public:
    Face();
    ~Face();

    void SetNormal(const Vec3& norm);
    const Vec3& GetNormal();

    int NumVertices();
    int GetVertexIndex(int i);

    int NumTextureCoords();
    int GetTextureCoordIndex(int i);

    int NumVertexNormals();
    int GetVertexNormalIndex(int i);

    void AddVertexIndex(int index);
    void AddTextureCoordIndex(int index);
    void AddVertexNormalIndex(int index);

private:
    Vec3 normal;
    std::vector<int> vertexIndeces;
    std::vector<int> textureCoordIndeces;
    std::vector<int> vertexNormalIndeces;
};


///////////////////////////////////////////////////////////////////////////////////////////////


class SubGroup {
public:
    SubGroup();
    ~SubGroup();

    int NumFaces();
    Face* GetFace(int i);

    Face* AddFace();

    void SetMaterial(Material* mat);
    Material* GetMaterial();

    Vec3 GetTextureScale();

private:
    std::vector<Face*> faces;
    Material* material;
};


///////////////////////////////////////////////////////////////////////////////////////////////


class Group {
public:
    Group(const std::string& groupName = "default");
    ~Group();

    const std::string& GetName();

    int NumSubGroups();
    SubGroup* GetSubGroup(int i);

    SubGroup* AddSubGroup();

private:
    std::string name;
    std::vector<SubGroup*> subGroups;
};


///////////////////////////////////////////////////////////////////////////////////////////////


// For depth sorting
struct FaceDepth {
    Face* face;
    SubGroup* subGroup;
    float depth;
};


///////////////////////////////////////////////////////////////////////////////////////////////


class OBJObject : public RenderObject {
public:
    OBJObject();
    virtual ~OBJObject();

    bool LoadObject(const std::string& fileName);

    double NormalizeBounds(double scaleBounds = 1.0);

    void SmoothShadingOn();
    void SmoothShadingOff();

    void WireFrameOn();
    void WireFrameOff();

    void ImmediateModeOn();
    void ImmediateModeOff();

    void CullFaceOn();
    void CullFaceOff();

    void SetOpacity(float opacity);    

    void TranslatePoints(const Vec3& vec);
    void RotatePoints(const Quat& quat);
    void ScalePoints(double scaleValue);
	virtual void ExpandPoints(double expansion);

    void SetCameraPosition(const Vec3& position);
    
    void DepthSortOn();
    void DepthSortOff();
    void DepthSort();

protected:
    Group* currentGroup;
    SubGroup* currentSubGroup;
    Material* currentMaterial;

    std::vector<Group*> groups;
    std::vector<Material*> materials;

    std::vector<Vec3> verts;
    std::vector<Vec3> textureCoords;
    std::vector<Vec3> vertexNormals;

    static bool initializedDevIL;

    bool smoothShading;
    bool wireFrame;
    bool immediateMode;
    bool cullFace;

    GLuint displayList;
    bool displayListCurrent;

    Vec3 cameraPosition;   

    bool depthSort;
    std::vector<FaceDepth> depths;

    bool ParseMtl(const std::string& fileName);

    std::string ResolveFileName(const std::string& parent, const std::string& child);

    void AddGroup(const std::string& name);
    void AddMaterial(const std::string& name);

    void AddVertex(const Vec3& position);
    void AddTextureCoord(const Vec3& coord);
    void AddVertexNormal(const Vec3& normal);

    void CalculateFaceNormals();
    void CalculateVertexNormals();

    virtual void PreRender();
    virtual void DoRender();
    virtual void PostRender();

    virtual void RenderGeometry();
};


#endif