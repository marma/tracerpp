#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../tracer.h"

class Mesh : public tracer::shape {
 public:
  // inner classes
  class vstruct {
  public:
    vstruct() : v(-1),vt(-1),vn(-1) {
    }
    int v,vt,vn;
  };
  
  class vertex {
  public:
    double x, y, z, s, t, shadow_s, shadow_t;
  };
  
  class face {
  public:
    std::vector<vstruct> vstructs;
  };
  
  class AABox {
  public:
    AABox() : empty(1) {
    }
    
    int contains(const AABox& box) {
      return	minX <= box.minX &&
	maxX >= box.maxX &&
	minY <= box.minY &&
	maxY >= box.maxY &&
	minZ <= box.minZ &&
	maxZ >= box.maxZ;
    }
    
    void grow(const AABox& box) {
      if (empty) {
	*this = box;
      } else {
	if (box.minX < minX) minX = box.minX;
	if (box.maxX > maxX) maxX = box.maxX;
	
	if (box.minY < minY) minY = box.minY;
	if (box.maxY > maxY) maxY = box.maxY;
	
	if (box.minZ < minZ) minZ = box.minZ;
	if (box.maxZ > maxZ) maxZ = box.maxZ;
      }
    }
    
    int contains(const vertex& v) {
      return !empty && (v.x >= minX && v.x <= maxX) && (v.y >= minY && v.y <= maxY) && (v.z >= minZ && v.z <= maxZ);
    }
    
    void grow(const vertex& v) {
      if (empty) {
	//					std::cout << "creating bounding box\n";
	minX = maxX = v.x;
	minY = maxY = v.y;
	minZ = maxZ = v.z;
	
	empty = 0;
      } else {
	if (v.x < minX) minX = v.x;
	if (v.x > maxX) maxX = v.x;
	if (v.y < minY) minY = v.y;
	if (v.y > maxY) maxY = v.y;
	if (v.z < minZ) minZ = v.z;
	if (v.z > maxZ) maxZ = v.z;
      }
    }
    
    double minX, maxX, minY, maxY, minZ, maxZ;
    int empty;
  };  

  class Material {
  public:
    Material(std::string _name) : name(_name), texture_id(-1) {
      Ka[0] = Ka[1] = Ka[2] = 0.0;
      Kd[0] = Kd[1] = Kd[2] = 0.75;
      Ks[0] = Ks[1] = Ks[2] = 0.0;
      d = 1;
      Ns = 0.0;
    }
    
    std::string name;
    int material_id, texture_id, shadow_id;
    friend class Scene;
    double Ka[3],Kd[3],Ks[3],d,Ns;
  };
  
  class Shape {
  public:
    Shape(std::string _name) : name(_name) {
    }
    
    void addFace(int face_nr) {
      faces.push_back(face_nr);
    }
    
    friend class Scene;
    friend std::ostream& operator << (std::ostream& out, const Scene& scene);
    int shape_id;
    std::string name;
    std::vector<int> faces;
    int shadowMap_id;
    int material_id;
  };
  
  class Group {
  public:
    Group(std::string _name) : name(_name) {
    }
    
    int addShape(int shape_nr) {
      shapes.push_back(shape_nr);
      
      return (int)shapes.size() - 1;
    }
    
    int group_id;
    std::string name;
    Group *parent;
    AABox aaBox;
    std::vector<int> shapes;
  }; 

  // constructors and destructors
  Mesh();
  Mesh(char *_filename);

  // methods
  virtual void intersect(const tracer::ray& r, tracer::intersection_collection& icoll);
  virtual int inside(const tracer::vector& x, double t);
  virtual shape *clone();
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  void clear();
  void loadMaterials(std::string filename);
  void loadScene(std::string filename);
  int addVertex(const vertex& v);
  int addNormal(const vertex& v);
  int addTexCoords(const vertex& v);
  int addFace(const face& face);
  int addGroup(std::string name);
  int addShape(std::string shapeName);
  int addShape(std::string groupName, std::string shapeName);
  int addShape(int group_nr, std::string shapeName);
  void calculateBoundingBoxes();

 protected:
  // data members
  std::string name, filename;
  std::vector<vertex> vertices, normals, texcoords;
  std::vector<face> faces;
  std::vector<Group> groups;
  std::vector<Shape> shapes;
  std::vector<Material> materials;
  //std::vector<Texture> textures;
  std::map<std::string, int> groupMap;
  std::map<std::string, int> materialMap;
  std::map<std::string, int> textureMap;
  
  Group *rootGroup;
};

#endif
