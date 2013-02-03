#include "mesh.h"

Mesh::Mesh() : tracer::shape("Mesh") {
}

Mesh::Mesh(char *_filename) : tracer::shape("Mesh"), name(_filename) {
  loadScene(filename);
  calculateBoundingBoxes();
}

void Mesh::clear() {
  vertices.clear();
  normals.clear();
  texcoords.clear();
  faces.clear();
  groups.clear();
  shapes.clear();
  materials.clear();
//  textures.clear();
  groupMap.clear();
  materialMap.clear();
  textureMap.clear();
}

void Mesh::loadMaterials(std::string filename) {
  char tmpbuf[1024];
  std::ifstream in(filename.c_str());
  std::string token;
  Material material("");
  name = filename;
  
  std::cout << "Loading materials from '" << filename << "'" << std::endl;

  while (in >> token) {
    if (token == "newmtl") {
      if (material.name != "") {
	materials.push_back(material);
	materialMap[material.name] = (int)materials.size() - 1;
      } 
      
      material = Material("");
      in >> material.name;
    } else if (token == "Ka") {
      in >> material.Ka[0] >> material.Ka[1] >> material.Ka[2];
    } else if (token == "Kd") {
      in >> material.Kd[0] >> material.Kd[1] >> material.Kd[2];
    } else if (token == "Ks") {
      in >> material.Ks[0] >> material.Ks[1] >> material.Ks[2];
    } else if (token == "d") {
      in >> material.d;
    } else if (token == "Ns") {
      in >> material.Ns;
    } else if (token == "map_Kd") {
      std::string filename;
      in >> filename;
      /*
      if (textureMap.find(filename) == textureMap.end()) {
	textures.push_back(Texture(filename));
	textureMap[filename] = (int)textures.size() - 1;
	textures[textureMap[filename]].loadBmp(filename);
      }
      
      material.texture_id = textureMap[filename];
      */
    } else {
      in.getline(tmpbuf, 1023);
    }
  }

  in.close();
  
  materials.push_back(material);
  materialMap[material.name] = (int)materials.size() - 1;
}



void Mesh::loadScene(std::string filename) {
  char tmpbuf[1024];
  std::ifstream in(filename.c_str());
  std::string token;
  int shape = -1, material = -1;

  std::cout << "Loading mesh from '" << filename << "'" << std::endl;
  
  while (in >> token) {
    if (token == "g") {
      std::string groupName, shapeName;
      
      in >> groupName >> shapeName;
      
      shape = addShape(groupName, shapeName);
    } else if (token == "mtllib") {
      in >> token;
      
      loadMaterials(token);
    } else if (token == "usemtl") {
      in >> token;
      
      material = materialMap[token];
      shapes[shape].material_id = material;
    } else if (token == "v") {
      vertex v;
      
      in >> v.x >> v.y >> v.z;
      
      addVertex(v);
    } else if (token == "vt") {
      vertex v;
      
      in >> v.x >> v.y;
      v.z = 0.0;
      
      addTexCoords(v);
    } else if (token == "vn") {
      vertex n;
      
      in >> n.x >> n.y >> n.z;
      
      addNormal(n);
    } else if (token == "f") {
      face f;
      
      while ((in.peek() < 'a' || in.peek() > 'z') && in.peek() != '\n') {
	char c;
	vstruct v;
	std::string p;
	
	in >> v.v;
	v.v--;
	
	c = in.get();
	
	if (in.peek() != '/') {
	  in >> v.vn;
	  v.vn--;
	}
	
	c = in.get();
	
	if (in.peek() != '/' && in.peek() != '\t' && in.peek() != ' ' && in.peek() != '\n' && in.peek() != '\r') {
	  in >> v.vt;
	  v.vt--;
	}
	
	f.vstructs.push_back(v);
      }
      
      shapes[shape].addFace(addFace(f));
      shapes[shape].material_id = material;
    } else {
      in.getline(tmpbuf, 1023);
    }
  }
  
  in.close();
  calculateBoundingBoxes();

  //  std::cout << *this << std::endl;
}

int Mesh::addVertex(const vertex& v) {
  vertices.push_back(v);

  return (int)vertices.size() - 1;
}

int Mesh::addNormal(const vertex& v) {
  normals.push_back(v);

  return (int)normals.size() - 1;
}

int Mesh::addTexCoords(const vertex& v) {
  texcoords.push_back(v);

  return (int)texcoords.size() - 1;
}

int Mesh::addFace(const face& face) {
  faces.push_back(face);

  return (int)faces.size() - 1;
}

int Mesh::addGroup(std::string name) {
  groups.push_back(Group(name));
  groupMap[name] = (int)groups.size() - 1;

  return (int)groups.size() - 1;
}

int Mesh::addShape(std::string shapeName) {
  shapes.push_back(Shape(shapeName));

  return (int)shapes.size() - 1;
}
			
int Mesh::addShape(int group_nr, std::string shapeName) {
  int ret = addShape(shapeName);
				
  groups[group_nr].addShape(ret);

  return ret;
}

int Mesh::addShape(std::string groupName, std::string shapeName) {
  int group_nr;

  if (groupMap.find(groupName) == groupMap.end()) {
    group_nr = addGroup(groupName);
  } else {
    group_nr = groupMap[groupName];
  }
				
  return addShape(group_nr, shapeName);
}

void Mesh::calculateBoundingBoxes() {
  std::cout << "Calculating bounding box" << std::endl;
  
  for (std::vector<Group>::iterator giter = groups.begin(); giter != groups.end(); giter++) {
    AABox aaBox;
    Group *group = &(*giter);

    for (std::vector<int>::iterator siter = group->shapes.begin(); siter != group->shapes.end(); siter++) {
      Shape *shape = &shapes[*siter];

      for (std::vector<int>::iterator fiter = shape->faces.begin(); fiter != shape->faces.end(); fiter++) {
	face *f = &faces[*fiter];

	for (std::vector<vstruct>::iterator viter = f->vstructs.begin(); viter != f->vstructs.end(); viter++) {
	  aaBox.grow(vertices[viter->v]);
	}
      }
    }

    group->aaBox = aaBox;
  }
}

void Mesh::intersect(const tracer::ray& r, tracer::intersection_collection& icoll)  {
  //std::cout << "intersect";
  //std::cout.flush();

  for (std::vector<Group>::const_iterator giter = groups.begin(); giter < groups.end(); giter++) {
    //std::cout << " " << giter->name;
    //std::cout.flush();

    for (std::vector<int>::const_iterator siter = giter->shapes.begin(); siter != giter->shapes.end(); siter++) {
      const Shape *s = &shapes[*siter];
      //Material m = materials[s->material_id];

      //std::cout << " " << s->name;
      //std::cout.flush();

      for (std::vector<int>::const_iterator fiter = s->faces.begin(); fiter != s->faces.end(); fiter++) {
	const face *f = &(faces[*fiter]);

	//std::cout << " " << f->;
	//std::cout.flush();

	vertex v1 = vertices[f->vstructs[0].v];
	vertex n1 = normals[f->vstructs[0].vn];
	for (int i=2;i<(int)f->vstructs.size(); i++) {
	  vertex v2 = vertices[f->vstructs[i-1].v], v3 = vertices[f->vstructs[i].v], n2 = normals[f->vstructs[i-1].vn], n3 = normals[f->vstructs[i].vn];

	  tracer::vector p0(v1.x, v1.y, v1.z), p1(v2.x, v2.y, v2.z), p2(v3.x, v3.y, v3.z);

	  //std::cout << " " << p0 << " " << p1 << " " << p2 << " " << r.v;
	  //std::cout.flush();

#define EPSILON 0.000001
	  double det,inv_det,u,v,t;

	  /* find vectors for two edges sharing vert0 */
	  tracer::vector edge1 = p1 - p0, edge2 = p2 - p0;

	  /* begin calculating determinant - also used to calculate U parameter */
	  tracer::vector pvec = r.v.cross(edge2);

	  /* if determinant is near zero, ray lies in plane of triangle */
	  det = edge1*pvec;

	  /* calculate distance from vert0 to ray origin */
	  tracer::vector tvec = r.p - p0;
	  inv_det = 1.0/det;

	  tracer::vector qvec = tvec.cross(edge1);
  
	  //std::cout << " A" << std::endl;
	  //std::cout.flush();

	  if (det > EPSILON) {
	    u = tvec*pvec;

	    if (u < 0.0 || u > det)	continue;

	    //calculate V parameter and test bounds 
	    //DOT(dir, qvec);
	    v = r.v*qvec;
	    if (v < 0.0 || u + v > det)	continue;
	  } else if (det < -EPSILON) {
	    // calculate U parameter and test bounds 
	    //*u = DOT(tvec, pvec);
	    u = tvec*pvec;
	    if (u > 0.0 || u < det)	continue;

	    // calculate V parameter and test bounds 
	    //*v = DOT(dir, qvec) ;
	    v = r.v*qvec;
	    if (v > 0.0 || u + v < det)	continue;
	  } else continue;  // ray is parallell to the plane of the triangle 

	  //std::cout << " B" << std::endl;
	  //std::cout.flush();

	  t = edge2*qvec*inv_det;

	  if (t < 0) continue;

	  u *= inv_det;
	  v *= inv_det;
								
	  tracer::vector n;
								
	  n = (u*tracer::vector(n2.x, n2.y, n2.z) + v*tracer::vector(n3.x, n3.y, n3.z) + (1-u-v)*tracer::vector(n1.x, n1.y, n1.z))^1;
	  // n = (edge1^1).cross(edge2^1);

	  tracer::intersection i(r(t), n, t, r.t, tracer::intersection::UNKNOWN, u,v);

	  icoll.insert(icoll.end(), i);								

	  //std::cout << "intersection: " << std::endl;
	  //std::cout.flush();

	}
      }
    }
  }
}
			
int Mesh::inside(const tracer::vector& x, double t) {
  return 0;
}

tracer::shape* Mesh::clone() {
  return new Mesh();
}

std::ostream& Mesh::toStream(std::ostream& out) const {
  out << "#--- Mesh: " << name << " ---" << "\n\n";
  
  out << "#  vertices:" << '\n';
  for (std::vector<vertex>::const_iterator viter = vertices.begin(); viter != vertices.end(); viter++) {
    out << "v " << viter->x << " " << viter->y << " " << viter->z << '\n';
  }
  
  out << "\n# normals:" << '\n';
  for (std::vector<vertex>::const_iterator niter = normals.begin(); niter != normals.end(); niter++) {
    out << "vn " << niter->x << " " << niter->y << " " << niter->z << '\n';
  }
  
  out << "\n# texcoords:" << '\n';
  for (std::vector<vertex>::const_iterator titer = texcoords.begin(); titer != texcoords.end(); titer++) {
    out << "vt " << titer->x << " " << titer->y << '\n';
  }
  out.flush();
  
  out << "\n# groups:" << '\n';
  for (std::vector<Group>::const_iterator giter = groups.begin(); giter != groups.end(); giter++) {
    out << "#  " << giter->name << ", bounding box: <" << giter->aaBox.minX << ", " << giter->aaBox.minY << ", " << giter->aaBox.minZ << "> - <" << giter->aaBox.maxX << ", " << giter->aaBox.maxY << ", " << giter->aaBox.maxZ << ">\n";
    
    for (std::vector<int>::const_iterator siter = giter->shapes.begin(); siter != giter->shapes.end(); siter++) {
      const Shape *shape = &shapes[*siter];
      out << "g " << giter->name << ' ' << shape->name << '\n';
      if (shape->material_id != -1) {
	out << "usemtl " << shape->material_id << '\n';
      }

      out.flush();
      
      for (std::vector<int>::const_iterator fiter = shape->faces.begin(); fiter != shape->faces.end(); fiter++) {
	const face *f = &(faces[*fiter]);
	
	out << "f ";
	for (std::vector<vstruct>::const_iterator viter = f->vstructs.begin(); viter != f->vstructs.end(); viter++) {
	  out << viter->v + 1;
	  out << '/';
	  if (viter->vn != -1) out << viter->vn + 1;
	  out << '/';
	  if (viter->vt != -1) out << viter->vt + 1;
	  out << ' ';
	}

	out << '\n';
	out.flush();
      }			
    }
  }
  
  return out;
}

std::istream& Mesh::fromStream(std::istream& in) {
  in >> name >> name;
  
  this->loadScene(name);
  
  while (in.get() != '}');

  return in;
}
