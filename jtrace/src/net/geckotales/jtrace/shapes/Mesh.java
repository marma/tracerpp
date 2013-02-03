package net.geckotales.jtrace.shapes;

import java.util.*;
import net.geckotales.jtrace.*;
import javax.vecmath.*;

public class Mesh implements Shape {
    protected Vector<Point3d> points = new Vector<Point3d>();
    protected Vector<Vector3d> normals = new Vector<Vector3d>();
    protected Vector<Polygon> polygons = new Vector<Polygon>();    
    
    public Mesh() {
    }
    
    public void addPolygon(Polygon p) {
        
    }
    
    public boolean inside(Point3d p, double t) {
        // @todo fix this ...
        return false;
    }
    
    public Set<Intersection> intersect(Ray r) {
        Set<Intersection> intersections = new HashSet<Intersection>();
        
        return intersections;
    }

    public class Vertex {
        int point = -1, normal = -1;
        public void setPoint(int i)  { point = i; }
        public void setNormal(int i) { normal = i; }
        public int getPoint()        { return point; }
        public int getNormal()       { return normal; }
    }
    
    public interface Polygon {
        public Vertex[] getVertices();
    }
    
    public class Triangle implements Polygon {
        protected Vertex[] vertices= new Vertex[3];
        
        public Triangle(Vertex v1, Vertex v2, Vertex v3) {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
        }
        
        public Vertex[] getVertices() { 
            return vertices;
        }
    }
    
    public class Quad implements Polygon {
        protected Vertex[] vertices= new Vertex[4];
        
        public Quad(Vertex v1, Vertex v2, Vertex v3, Vertex v4) {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
        }
        
        public Vertex[] getVertices() { 
            return vertices;
        }
    }
    /*
    public class GenPolygon implements Polygon {
        
    }
     */
}
