package net.geckotales.jtrace.shapes;

import java.util.*;
import javax.vecmath.*;
import net.geckotales.jtrace.*;

public class Triangle implements Shape {
    Point3d p1 = new Point3d(), p2 = new Point3d(), p3 = new Point3d();
    Vector3d n1 = null, n2 = null, n3 = null;
    
    public Triangle(Point3d p1, Point3d p2, Point3d p3) {
        this.p1.set(p1);
        this.p2.set(p2);
        this.p3.set(p3);
    }
    
    public Triangle(Point3d p1, Vector3d n1, Point3d p2, Vector3d n2, Point3d p3, Vector3d n3) {
        this.p1.set(p1);
        this.p2.set(p2);
        this.p3.set(p3);
        this.n1 = new Vector3d(n1);
        this.n2 = new Vector3d(n2);
        this.n3 = new Vector3d(n3);
    }
    
    public boolean inside(Point3d p, double t) {
        return false;
    }
    
    public Set<Intersection> intersect(Ray r) {
        Set<Intersection> intersections = new HashSet<Intersection>();
        
        return intersections;
    }
}
