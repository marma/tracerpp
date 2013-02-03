#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace tracer {
	namespace mesh {

		typedef short WORD;
		typedef long LONG;
		typedef int DWORD;

		typedef struct BITMAPFILEHEADER { // bmfh 
			WORD    bfType; 
			DWORD   bfSize; 
			WORD    bfReserved1; 
			WORD    bfReserved2; 
			DWORD   bfOffBits; 
		} BITMAPFILEHEADER; 

		typedef struct BITMAPINFOHEADER { // bmih 
			DWORD  biSize; 
			LONG   biWidth; 
			LONG   biHeight; 
			WORD   biPlanes; 
			WORD   biBitCount;
			DWORD  biCompression; 
			DWORD  biSizeImage; 
			LONG   biXPelsPerMeter; 
			LONG   biYPelsPerMeter; 
			DWORD  biClrUsed; 
			DWORD  biClrImportant; 
		} BITMAPINFOHEADER; 

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

		class Texture {
		public:
			Texture(std::string _name) : name(_name), data(NULL) {}

			~Texture() {
		//		if (data != NULL) free(data);
			}

			void loadRaw(std::string filename, int _width, int _height, int offset = 0) {
				height = _height;
				width = _width;
				data = malloc(3*width*height);

				FILE *file = fopen(filename.c_str(), "r");
				fseek(file, offset, SEEK_CUR);
				fread(data, 3, width*height, file);
				fclose(file);
			}

			void loadBmp(std::string filename) {
				BITMAPFILEHEADER bmfh;
				BITMAPINFOHEADER bmih;

				FILE *file = fopen(filename.c_str(), "r");

				fread((void*)&bmfh, sizeof(BITMAPFILEHEADER), 1, file);		
				fread((void*)&bmih, sizeof(BITMAPINFOHEADER), 1, file);		

				fclose(file);

				loadRaw(filename, bmih.biWidth, bmih.biHeight, bmfh.bfOffBits);
			}

			int texture_id;
			std::string name;
			int width, height;
			void *data;
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

		class Scene : public tracer::shape {
		public:
			Scene() : tracer::shape("tracer::mesh::Scene") {
			}

			Scene(std::string filename) : tracer::shape("Mesh"), name(filename) {
				loadScene(filename);
				calculateBoundingBoxes();
			}

			void clear() {
				vertices.clear();
				normals.clear();
				texcoords.clear();
				faces.clear();
				groups.clear();
				shapes.clear();
				materials.clear();
				textures.clear();
				groupMap.clear();
				materialMap.clear();
				textureMap.clear();
			}

			void loadMaterials(std::string filename) {
				char tmpbuf[1024];
				std::ifstream in(filename.c_str());
				std::string token;
				Material material("");
				name = filename;

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

						if (textureMap.find(filename) == textureMap.end()) {
							textures.push_back(Texture(filename));
							textureMap[filename] = (int)textures.size() - 1;
							textures[textureMap[filename]].loadBmp(filename);
						}

						material.texture_id = textureMap[filename];
					} else {
						in.getline(tmpbuf, 1023);
					}
				}

				materials.push_back(material);
				materialMap[material.name] = (int)materials.size() - 1;
			}

			void loadScene(std::string filename) {
				char tmpbuf[1024];
				std::ifstream in(filename.c_str());
				std::string token;
				int shape = -1, material = -1;

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
			}

			int addVertex(const vertex& v) {
				vertices.push_back(v);

				return (int)vertices.size() - 1;
			}

			int addNormal(const vertex& v) {
				normals.push_back(v);

				return (int)normals.size() - 1;
			}

			int addTexCoords(const vertex& v) {
				texcoords.push_back(v);

				return (int)texcoords.size() - 1;
			}

			int addFace(const face& face) {
				faces.push_back(face);

				return (int)faces.size() - 1;
			}

			int addGroup(std::string name) {
				groups.push_back(Group(name));
				groupMap[name] = (int)groups.size() - 1;

				return (int)groups.size() - 1;
			}

			int addShape(std::string shapeName) {
				shapes.push_back(Shape(shapeName));

				return (int)shapes.size() - 1;
			}
			
			int addShape(int group_nr, std::string shapeName) {
				int ret = addShape(shapeName);
				
				groups[group_nr].addShape(ret);

				return ret;
			}

			int addShape(std::string groupName, std::string shapeName) {
				int group_nr;

				if (groupMap.find(groupName) == groupMap.end()) {
					group_nr = addGroup(groupName);
				} else {
					group_nr = groupMap[groupName];
				}
				
				return addShape(group_nr, shapeName);
			}

			void calculateBoundingBoxes() {
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

			void createLightMaps() {
			}


			virtual void intersect(const ray& r, intersection_collection& icoll)  {

				for (std::vector<Group>::const_iterator giter = groups.begin(); giter < groups.end(); giter++) {
					for (std::vector<int>::const_iterator siter = giter->shapes.begin(); siter != giter->shapes.end(); siter++) {
						const Shape *s = &shapes[*siter];
						Material m = materials[s->material_id];

						for (std::vector<int>::const_iterator fiter = s->faces.begin(); fiter != s->faces.end(); fiter++) {
							const face *f = &(faces[*fiter]);

							vertex v1 = vertices[f->vstructs[0].v];
							vertex n1 = normals[f->vstructs[0].vn];
							for (int i=2;i<(int)f->vstructs.size(); i++) {
								vertex v2 = vertices[f->vstructs[i-1].v], v3 = vertices[f->vstructs[i].v], n2 = normals[f->vstructs[i-1].vn], n3 = normals[f->vstructs[i].vn];

								tracer::vector p0(v1.x, v1.y, v1.z), p1(v2.x, v2.y, v2.z), p2(v3.x, v3.y, v3.z);

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
  
								if (det > EPSILON) {
									u = tvec*pvec;

									if (u < 0.0 || u > det)	continue;

									//calculate V parameter and test bounds 
									//DOT(dir, qvec);
									v = r.v*qvec;
									if (v < 0.0 || u + v > det)	continue;
								} else if(det < -EPSILON) {
									// calculate U parameter and test bounds 
									//*u = DOT(tvec, pvec);
									u = tvec*pvec;
									if (u > 0.0 || u < det)	continue;

									// calculate V parameter and test bounds 
									//*v = DOT(dir, qvec) ;
									v = r.v*qvec;
									if (v > 0.0 || u + v < det)	continue;
								} else continue;  // ray is parallell to the plane of the triangle 

								t = edge2*qvec*inv_det;

								if (t < 0) continue;

								u *= inv_det;
								v *= inv_det;
								
								tracer::vector n;
								
								n = (u*tracer::vector(n2.x, n2.y, n2.z) + v*tracer::vector(n3.x, n3.y, n3.z) + (1-u-v)*tracer::vector(n1.x, n1.y, n1.z))^1;
								//	n = (edge1^1).cross(edge2^1);

								tracer::intersection i(r(t), n, t, r.t, tracer::intersection::UNKNOWN, u,v);

								icoll.insert(icoll.end(), i);								
							}
						}

					}
				}

			}
			
			virtual int inside(const vector& x, double t) {
				return 0;
			}

			virtual shape* clone() {
				return new Scene();
			}

			virtual std::ostream& toStream(std::ostream& out) const {
				return out << "name " << name;
			}
			
			virtual std::istream& fromStream(std::istream& in) {
				in >> name >> name;

				this->loadScene(name);

				return in;
			}
			


			friend std::ostream& operator << (std::ostream& out, const Scene& scene) {
				out << "#--- Scene: " << scene.name << " ---" << "\n\n";

				out << "#  vertices:" << '\n';
				for (std::vector<vertex>::const_iterator viter = scene.vertices.begin(); viter != scene.vertices.end(); viter++) {
					out << "v " << viter->x << " " << viter->y << " " << viter->z << '\n';
				}

				out << "\n# normals:" << '\n';
				for (std::vector<vertex>::const_iterator niter = scene.normals.begin(); niter != scene.normals.end(); niter++) {
					out << "vn " << niter->x << " " << niter->y << " " << niter->z << '\n';
				}

				out << "\n# texcoords:" << '\n';
				for (std::vector<vertex>::const_iterator titer = scene.texcoords.begin(); titer != scene.texcoords.end(); titer++) {
					out << "vt " << titer->x << " " << titer->y << '\n';
				}
				out.flush();

				out << "\n# groups:" << '\n';
				for (std::vector<Group>::const_iterator giter = scene.groups.begin(); giter != scene.groups.end(); giter++) {
					out << "#  " << giter->name << ", bounding box: <" << giter->aaBox.minX << ", " << giter->aaBox.minY << ", " << giter->aaBox.minZ << "> - <" << giter->aaBox.maxX << ", " << giter->aaBox.maxY << ", " << giter->aaBox.maxZ << ">\n";
					
					for (std::vector<int>::const_iterator siter = giter->shapes.begin(); siter != giter->shapes.end(); siter++) {
						const Shape *shape = &scene.shapes[*siter];
						out << "g " << giter->name << ' ' << shape->name << '\n';
						out << "usemtl " << shape->material_id << '\n';
						out.flush();

						for (std::vector<int>::const_iterator fiter = shape->faces.begin(); fiter != shape->faces.end(); fiter++) {
							const face *f = &(scene.faces[*fiter]);

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

		private:
			std::string name, filename;
			std::vector<vertex> vertices, normals, texcoords;
			std::vector<face> faces;
			std::vector<Group> groups;
			std::vector<Shape> shapes;
			std::vector<Material> materials;
			std::vector<Texture> textures;
			std::map<std::string, int> groupMap;
			std::map<std::string, int> materialMap;
			std::map<std::string, int> textureMap;

			Group *rootGroup;

		};
	}
}

#endif
